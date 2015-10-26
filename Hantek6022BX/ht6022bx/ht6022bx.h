#ifndef HT6022BX_H
#define HT6022BX_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <QList>
#include "ht6022bx_global.h"
#include "usbwrapper.h"
#include "libusbwrapper.h"
class HT6022BXSHARED_EXPORT HT6022bx : public QObject
{
    Q_OBJECT
public:
    explicit HT6022bx(QObject *parent = 0);
    ~HT6022bx();
private:
    QList<HT6022BX_Info>            *HantekDevices;
    HT6022BX_ControlTransfer        *IR1;
    HT6022BX_ControlTransfer        *IR2;
    HT6022BX_ControlTransfer        *SR;
    HT6022BX_ControlTransfer        *SetCalLevel;
    HT6022BX_ControlTransfer        *GetCalLevel;
    HT6022BX_ControlTransfer        *ReadControl;
    UsbWrapper                      *UsbDriver;
public:
    HT6022BX_ErrorTypeDef DeviceInit    ();
    void DeviceExit                     ();
    void printDevices                   ();
    HT6022BX_ErrorTypeDef deviceOpen    (const unsigned int index);
    void deviceClose                    (int interface);
    HT6022BX_ErrorTypeDef readData      (const unsigned int index,unsigned char* CH1, unsigned char* CH2,HT6022BX_DataSizeTypeDef DataSize,unsigned int  Timeout);
    HT6022BX_ErrorTypeDef readDataMulti (const unsigned int index,unsigned char* CH1, unsigned char* CH2,HT6022BX_DataSizeTypeDef DataSize,unsigned int  Timeout);
    HT6022BX_ErrorTypeDef setXXX        (const unsigned int index);
    HT6022BX_ErrorTypeDef setCalValues  (const unsigned int index,unsigned char* CalValues,HT6022BX_CVSizeTypeDef CVSize);
    HT6022BX_ErrorTypeDef getCalValues  (const unsigned int index,unsigned char* CalValues,HT6022BX_CVSizeTypeDef CVSize);
    HT6022BX_ErrorTypeDef setSR         (const unsigned int index,HT6022BX_SRTypeDef SR);
    HT6022BX_ErrorTypeDef setCH1IR      (const unsigned int index,HT6022BX_IRTypeDef IR);
    HT6022BX_ErrorTypeDef setCH2IR      (const unsigned int index,HT6022BX_IRTypeDef IR);

signals:
    void deviceConnected(HT6022BX_ErrorTypeDef errorCode);
    void deviceReady    (HT6022BX_ErrorTypeDef errorCode);
    void sendDevicesInfo(QList<HT6022BX_Info> *DeviceName);
    void libError       (HT6022BX_ErrorTypeDef errorCode);
    void enableDownload ();
    void downloadFinnish();
public slots:
    void searchDevice           (const QString &DeviceName);
    void searchDeviceIndex      (const unsigned int index);
    void FirmwareInstall        (const QString &DeviceName);
    void FirmwareInstallIndex   (const unsigned int index);
    void getDevicesInfo();
};

#endif // HT6022BX_H
