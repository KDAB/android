#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "qandroidmediaplayer.h"
#include "qsurfacetexture.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Register our QML type
    qmlRegisterType<QSurfaceTexture>("com.kdab.android", 1, 0, "SurfaceTexture");

    // Create a player
    QAndroidMediaPlayer player;

    QQmlApplicationEngine engine;

    // Set the player
    engine.rootContext()->setContextProperty("_mediaPlayer", &player);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
