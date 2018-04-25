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
    qmlRegisterType<QAndroidMediaPlayer>("com.kdab.android", 1, 0, "AndroidMediaPlayer");
    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
