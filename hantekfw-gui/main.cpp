#include <QApplication>
#include <QQmlApplicationEngine>
#include "ht6022bx.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject     *GuiHantek  = engine.rootObjects().first();
    HT6022bx    *hantek     = new HT6022bx();

    QObject::connect(GuiHantek,SIGNAL(qmlGetDevicesInfo()),hantek, SLOT(getDevicesInfo()));
    QObject::connect(GuiHantek,SIGNAL(qmlSearchDevices(QString)),hantek, SLOT(searchDevice(QString)));
    QObject::connect(GuiHantek,SIGNAL(qmlInstallFW(QString)),hantek, SLOT(FirmwareInstall(QString)));

    QObject::connect(hantek,SIGNAL(enableDownload()),GuiHantek, SLOT(enableDownload()));

    return app.exec();
}
/*

    //void deviceConnected(HT6022_ErrorTypeDef errorCode);
    void deviceConnected(int errorCode);
    void deviceReady(HT6022_ErrorTypeDef errorCode);
    void sendDevicesInfo(QList<HT6022BX_Info> *DeviceName);
    void libError(HT6022_ErrorTypeDef errorCode);
    void enableDownload();

 */
