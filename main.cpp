#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "httpcache.h"
#include "toolbox.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_ANDROID_VOLUME_KEYS", "1");

    QGuiApplication app(argc, argv);
    app.setOrganizationName("DesBwa");
    app.setOrganizationDomain("desbwa.org");
    app.setApplicationName("PhereoRoll3D");

    Toolbox toolbox;

    QQmlApplicationEngine engine;
    engine.setNetworkAccessManagerFactory(new HttpCache_NAMF);
    engine.rootContext()->setContextProperty("toolbox", &toolbox);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
