#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "video_player_controller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/vision_orange.png"));

    VideoPlayerController player;
    QQmlApplicationEngine engine;

    engine.addImageProvider("video", player.frameProvider());
    engine.rootContext()->setContextProperty("videoPlayer", &player);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
