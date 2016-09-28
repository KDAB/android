#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>

#include "rep_pingpong_replica.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QAndroidJniObject::callStaticMethod<void>("com/kdab/training/MyService",
                                              "startMyService",
                                              "(Landroid/content/Context;)V",
                                              QtAndroid::androidActivity().object());

    QRemoteObjectNode repNode;
    repNode.connectToNode(QUrl(QStringLiteral("local:replica")));
    QSharedPointer<PingPongReplica> rep(repNode.acquire<PingPongReplica>());
    engine.rootContext()->setContextProperty("pingPong", rep.data());
    bool res = rep->waitForSource();
    Q_ASSERT(res);
    rep->ping("Hello");

    return app.exec();
}
