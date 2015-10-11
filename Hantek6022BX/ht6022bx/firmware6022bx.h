#ifndef FIRMWARE6022BX_H
#define FIRMWARE6022BX_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QList>
#include "ht6022bx_global.h"

class Firmware6022BX : public QObject
{
    Q_OBJECT
public:
    explicit Firmware6022BX(QObject *parent = 0);
    ~Firmware6022BX();
private:
    HT6022_ErrorTypeDef DeviceInit(void);
    void DeviceExit(void);
    QList<HT6022BX_Info> *HantekDevices;
signals:
    void deviceConnected();
    void deviceReady(HT6022_ErrorTypeDef errorCode);
    void sendDevicesInfo(QList<HT6022BX_Info> *DeviceName);
public slots:
    void searchDevice(QString *DeviceName);
    void FirmwareInstall(QString *DeviceName);
    void getDevicesInfo();
};

#endif // FIRMWARE6022BX_H
