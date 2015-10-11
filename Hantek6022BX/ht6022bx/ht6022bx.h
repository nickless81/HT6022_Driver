#ifndef HT6022BX_H
#define HT6022BX_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <QList>
#include "ht6022bx_global.h"
//#include "firmware6022bx.h"
class HT6022BXSHARED_EXPORT HT6022bx : public QObject
{
    Q_OBJECT
public:
    explicit HT6022bx(QObject *parent = 0);
    ~HT6022bx();
    //Firmware6022BX *Fw;
private:
    HT6022_ErrorTypeDef DeviceInit(void);
    void DeviceExit(void);
    QList<HT6022BX_Info> *HantekDevices;
signals:
    void deviceConnected(HT6022_ErrorTypeDef errorCode);
    void deviceReady(HT6022_ErrorTypeDef errorCode);
    void sendDevicesInfo(QList<HT6022BX_Info> *DeviceName);
public slots:
    void searchDevice(QString *DeviceName);
    void FirmwareInstall(QString *DeviceName);
    void getDevicesInfo();
};

#endif // HT6022BX_H
