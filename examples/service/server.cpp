#include <QCoreApplication>
#include "rep_pingpong_source.h"

class PingPong : public PingPongSource {
public slots:
    // PingPongSource interface
    void ping(const QString &msg) override {
        emit pong(msg + " from server");
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:replica")));
    PingPong pingPongServer;
    srcNode.enableRemoting(&pingPongServer);

    return app.exec();
}
