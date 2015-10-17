#ifndef HT6022BX_H
#define HT6022BX_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <QList>
#include "ht6022bx_global.h"
class HT6022BXSHARED_EXPORT HT6022bx : public QObject
{
    Q_OBJECT
public:
    explicit HT6022bx(QObject *parent = 0);
    ~HT6022bx();
private:
    HT6022_ErrorTypeDef DeviceInit(void);
    void DeviceExit(void);
    QList<HT6022BX_Info> *HantekDevices;
    void printDevices();
signals:
    //void deviceConnected(HT6022_ErrorTypeDef errorCode);
    void deviceConnected(HT6022_ErrorTypeDef errorCode);
    void deviceReady(HT6022_ErrorTypeDef errorCode);
    void sendDevicesInfo(QList<HT6022BX_Info> *DeviceName);
    void libError(HT6022_ErrorTypeDef errorCode);
    void enableDownload();
    void downloadFinnish();
public slots:
    void searchDevice(const QString &DeviceName);
    void FirmwareInstall(const QString &DeviceName);
    void getDevicesInfo();
};

#endif // HT6022BX_H
