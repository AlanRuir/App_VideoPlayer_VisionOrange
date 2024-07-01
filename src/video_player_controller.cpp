#include "video_player_controller.h"
#include <QDebug>
#include <chrono>

VideoPlayerController::VideoPlayerController(QObject *parent)
    : QThread(parent)
    , playing_(false)
    , frame_provider_(new VideoFrameProvider)
    , format_context_(nullptr)
    , codec_context_(nullptr)
    , frame_(nullptr)
    , packet_(nullptr)
    , sws_context_(nullptr)
    , hw_device_ctx_(nullptr) 
    , frame_rate_(0.0)
{
    avformat_network_init();
}

VideoPlayerController::~VideoPlayerController() 
{
    stop();
    freeResources();
    delete frame_provider_;
}

void VideoPlayerController::play(const QString &url)
{
    QMutexLocker locker(&mutex_);
    url_ = url;
    playing_ = true;
    if (!isRunning()) 
    {
        start();
    }
}

void VideoPlayerController::freeResources()
{
    if (decode_thread_ && decode_thread_->joinable())
    {
        decode_thread_->join();
    }

    av_frame_free(&frame_);
    av_packet_free(&packet_);
    avcodec_free_context(&codec_context_);
    avformat_close_input(&format_context_);
    sws_freeContext(sws_context_);
    av_buffer_unref(&hw_device_ctx_);
}

void VideoPlayerController::run() 
{
    format_context_ = avformat_alloc_context();
    AVDictionary *opts = nullptr;
    av_dict_set(&opts, "buffer_size", "2048000", 0); 
    av_dict_set(&opts, "max_delay", "1000000", 0); 
    av_dict_set(&opts, "rtsp_transport", "tcp", 0);

    if (avformat_open_input(&format_context_, url_.toStdString().c_str(), nullptr, &opts) != 0) 
    {
        emit errorOccurred("Unable to open input");
        freeResources();
        return;
    }

    if (avformat_find_stream_info(format_context_, nullptr) < 0) 
    {
        emit errorOccurred("Unable to find stream info");
        freeResources();
        return;
    }

    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < format_context_->nb_streams; i++) 
    {
        if (format_context_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) 
        {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) 
    {
        emit errorOccurred("Unable to find video stream");
        freeResources();
        return;
    }

    AVStream *videoStream = format_context_->streams[videoStreamIndex];
    frame_rate_ = av_q2d(videoStream->r_frame_rate);
    qDebug() << "Frame rate: " << frame_rate_;

    const char* codec_name = nullptr;
    if (videoStream->codecpar->codec_id == AV_CODEC_ID_H264) 
    {
        codec_name = "h264_cuvid";
    } 
    else if (videoStream->codecpar->codec_id == AV_CODEC_ID_HEVC) 
    {
        codec_name = "hevc_cuvid";
    } 
    else 
    {
        emit errorOccurred("Unsupported codec");
        freeResources();
        return;
    }

    AVCodec *codec = avcodec_find_decoder_by_name(codec_name);
    if (!codec) 
    {
        emit errorOccurred("Unable to find CUDA decoder");
        freeResources();
        return;
    }

    codec_context_ = avcodec_alloc_context3(codec);
    if (!codec_context_) 
    {
        emit errorOccurred("Unable to allocate codec context");
        freeResources();
        return;
    }

    if (avcodec_parameters_to_context(codec_context_, videoStream->codecpar) < 0) 
    {
        emit errorOccurred("Unable to copy codec parameters");
        freeResources();
        return;
    }

    if (av_hwdevice_ctx_create(&hw_device_ctx_, AV_HWDEVICE_TYPE_CUDA, nullptr, nullptr, 0) < 0) 
    {
        emit errorOccurred("Unable to create CUDA device context");
        freeResources();
        return;
    }

    codec_context_->hw_device_ctx = av_buffer_ref(hw_device_ctx_);

    if (avcodec_open2(codec_context_, codec, nullptr) < 0) 
    {
        emit errorOccurred("Unable to open codec");
        freeResources();
        return;
    }

    frame_ = av_frame_alloc();
    packet_ = av_packet_alloc();

    QThread::setPriority(QThread::HighPriority);

    decode_thread_ = std::make_shared<std::thread>(&VideoPlayerController::decode, this);

    while (playing_) 
    {
        if (av_read_frame(format_context_, packet_) >= 0) 
        {
            if (packet_->stream_index == videoStreamIndex) 
            {
                {
                    std::lock_guard<std::mutex> lock(packet_queue_mutex_);
                    packet_queue_.push(av_packet_clone(packet_));
                    packet_queue_condition_.notify_one();
                }
            }
            av_packet_unref(packet_);
        }
    }
}

void VideoPlayerController::stop() 
{
    QMutexLocker locker(&mutex_);
    playing_ = false;
    wait();
}

VideoFrameProvider* VideoPlayerController::frameProvider() const 
{
    return frame_provider_;
}

void VideoPlayerController::decode()
{
    while (playing_) 
    {
        AVPacket *currentPacket = nullptr;

        {
            std::unique_lock<std::mutex> lock(packet_queue_mutex_);
            packet_queue_condition_.wait_for(lock, std::chrono::milliseconds(100), [&] { return !packet_queue_.empty(); });

            if (packet_queue_.empty())
            {
                continue;
            }
        }
        
        while (!packet_queue_.empty())
        {
            currentPacket = packet_queue_.front();
            packet_queue_.pop();

            if (avcodec_send_packet(codec_context_, currentPacket) == 0) 
            {
                while (avcodec_receive_frame(codec_context_, frame_) == 0) 
                {
                    if (frame_->format == AV_PIX_FMT_CUDA) 
                    {
                        AVFrame *swFrame = av_frame_alloc();
                        if (!swFrame) 
                        {
                            emit errorOccurred("Unable to allocate software frame_");
                            av_packet_free(&currentPacket);
                            return;
                        }
                        if (av_hwframe_transfer_data(swFrame, frame_, 0) < 0) 
                        {
                            av_packet_free(&currentPacket);
                            av_frame_free(&swFrame);
                            emit errorOccurred("Error transferring the frame_ to system memory");
                            return;
                        }

                        if (!sws_context_) 
                        {
                            sws_context_ = sws_getContext(swFrame->width, swFrame->height, static_cast<AVPixelFormat>(swFrame->format),
                                                        swFrame->width, swFrame->height, AV_PIX_FMT_RGB24,
                                                        SWS_BILINEAR, nullptr, nullptr, nullptr);
                        }

                        QImage img(swFrame->width, swFrame->height, QImage::Format_RGB888);
                        uint8_t *data[1] = { img.bits() };      
                        int linesize[1] = { static_cast<int>(img.bytesPerLine()) };
                        sws_scale(sws_context_, swFrame->data, swFrame->linesize, 0, swFrame->height, data, linesize);

                        frame_provider_->updateFrame(img);

                        emit frameReady();

                        av_frame_free(&swFrame);
                    }
                }
            }
            av_packet_free(&currentPacket);
        }
    }
}
