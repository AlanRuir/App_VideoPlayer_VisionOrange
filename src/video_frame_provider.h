#ifndef __VIDEO_FRAME_PROVIDER_H__
#define __VIDEO_FRAME_PROVIDER_H__

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class VideoFrameProvider : public QQuickImageProvider
{
public:
    VideoFrameProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void updateFrame(const QImage &frame);

private:
    QImage m_frame;
    QMutex m_mutex;
};

#endif // __VIDEO_FRAME_PROVIDER_H__
