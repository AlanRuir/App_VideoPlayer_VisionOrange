#include "video_frame_provider.h"

VideoFrameProvider::VideoFrameProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage VideoFrameProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QMutexLocker locker(&m_mutex);

    if (size)
    {
        *size = m_frame.size();
    }

    if (m_frame.isNull()) 
    {
        qDebug() << "No frame available!";
        return QImage();
    }

    return m_frame;
}

void VideoFrameProvider::updateFrame(const QImage &frame)
{
    QMutexLocker locker(&m_mutex);
    m_frame = frame;
}
