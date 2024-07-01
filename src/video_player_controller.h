#ifndef __VIDEO_PLAYER_CONTROLLER_H__
#define __VIDEO_PLAYER_CONTROLLER_H__

#include <thread>
#include <memory>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <QObject>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>

extern "C" 
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/hwcontext.h>
#include <libavutil/imgutils.h>
}

#include "video_frame_provider.h"

class VideoPlayerController : public QThread 
{
    Q_OBJECT

public:
    explicit VideoPlayerController(QObject *parent = nullptr);
    ~VideoPlayerController();

    Q_INVOKABLE void play(const QString &url);
    Q_INVOKABLE void stop();

    VideoFrameProvider* frameProvider() const;

    void decode();

signals:
    void frameReady();
    void errorOccurred(const QString &errorString);

protected:
    void run() override;

private:
    QString                      url_;
    bool                         playing_;
    QMutex                       mutex_;
    VideoFrameProvider           *frame_provider_;
    AVFormatContext              *format_context_;
    AVCodecContext               *codec_context_;
    AVFrame                      *frame_;
    AVPacket                     *packet_;
    SwsContext                   *sws_context_;
    AVBufferRef                  *hw_device_ctx_;
    std::shared_ptr<std::thread> decode_thread_;
    std::queue<AVPacket*>        packet_queue_;
    double                       frame_rate_;
    std::mutex                   packet_queue_mutex_;
    std::condition_variable      packet_queue_condition_;

    void freeResources();
};

#endif // __VIDEO_PLAYER_CONTROLLER_H__
