#include "firmware6022bx.h"

Firmware6022BX::Firmware6022BX(QObject *parent) : QObject(parent)
{
    HT6022BX_Info BE;
    BE.Name     =   "6022BE";
    BE.FwSize   =   HT6022_FIRMWARE_SIZE;
    BE.IdProduct=   HT6022BE_MODEL;
    BE.Firmware =   HT6022_Firmware;
    HT6022BX_Info BL;
    BL.Name     =   "6022BL";
    BL.FwSize   =   HT6022BL_FIRMWARE_SIZE;
    BL.IdProduct=   HT6022BL_MODEL;
    BL.Firmware =   HT6022BL_Firmware;
    this->HantekDevices->append(BE);
    this->HantekDevices->append(BL);
    this->DeviceInit();
}
Firmware6022BX::~Firmware6022BX()
{
    this->DeviceExit();
}
void Firmware6022BX::FirmwareInstall(QString *DeviceName)
{
    qDebug() << "Firmware6022BX::FirmwareInstall(" << DeviceName << ")";
    Q_UNUSED(DeviceName);
    int IdProduct;
    int FwSize;
    unsigned char*  Firmware;
    for (int i = 0; i < this->HantekDevices->size(); ++i) {
        if (this->HantekDevices->at(i).Name == *DeviceName)
        {
            IdProduct   = this->HantekDevices->at(i).IdProduct;
            FwSize      = this->HantekDevices->at(i).FwSize;
            Firmware    = this->HantekDevices->at(i).Firmware;
        }
    }
    libusb_device_handle  *Dev_handle;
    Dev_handle = libusb_open_device_with_vid_pid(NULL, HT6022_FIRMWARE_VENDOR_ID, IdProduct);
    if (Dev_handle == 0)
    {
        emit deviceReady(HT6022_ERROR_NO_DEVICE);
        return;
     }
    if(libusb_kernel_driver_active(Dev_handle, 0) == 1)
    {
        if(libusb_detach_kernel_driver(Dev_handle, 0) != 0)
        {
            libusb_close(Dev_handle);
            emit deviceReady(HT6022_ERROR_OTHER);
            return;
        }
    }
    if(libusb_claim_interface(Dev_handle, 0) < 0)
    {
        libusb_close(Dev_handle);
        emit deviceReady(HT6022_ERROR_OTHER);
        return;
    }
    unsigned int    Size;
    unsigned int    Value;
    while (FwSize)
    {
        Size  = *Firmware + ((*(Firmware + 1))<<0x08);
        Firmware = Firmware + 2;
        Value = *Firmware + ((*(Firmware + 1))<<0x08);
        Firmware = Firmware + 2;
        if (libusb_control_transfer (   Dev_handle,
                                        HT6022_FIRMWARE_REQUEST_TYPE,
                                        HT6022_FIRMWARE_REQUEST,
                                        Value,
                                        HT6022_FIRMWARE_INDEX,
                                        Firmware,
                                        Size,
                                        0) != (int)Size)
        {
            libusb_release_interface(Dev_handle, 0);
            libusb_close(Dev_handle);
            emit deviceReady(HT6022_ERROR_OTHER);
            return;
        }
        Firmware = Firmware + Size;
        FwSize--;
    }
    libusb_release_interface(Dev_handle, 0);
    libusb_close(Dev_handle);
    emit deviceReady(HT6022_SUCCESS);
    return;
}
HT6022_ErrorTypeDef Firmware6022BX::DeviceInit()
{
    qDebug("Firmware6022BX::DeviceInit()");
    if (libusb_init(NULL) == 0)
    {
        return HT6022_SUCCESS;
    }else{
        return HT6022_ERROR_OTHER;
    }
    return HT6022_SUCCESS;
}
void Firmware6022BX::DeviceExit()
{
    qDebug("Firmware6022BX::DeviceExit()");
    libusb_exit(NULL);
    return;
}
void Firmware6022BX::searchDevice(QString *DeviceName)
{
    Q_UNUSED(DeviceName);
    emit deviceConnected();
    return;
}
void Firmware6022BX::getDevicesInfo()
{
    qDebug("Firmware6022BX::getDevicesInfo()");
    emit sendDevicesInfo(this->HantekDevices);
    return;
}
