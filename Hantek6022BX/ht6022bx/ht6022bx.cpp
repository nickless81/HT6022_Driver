#include "ht6022bx.h"


HT6022bx::HT6022bx(QObject *parent) : QObject(parent)
{
    HT6022BX_Info BE;
    BE.Name     =   "6022BE";
    BE.FwSize   =   HT6022_FIRMWARE_SIZE;
    BE.IdProduct=   HT6022BE_MODEL;
    BE.IdVendor =   HT6022_FW_VENDOR;
    BE.Firmware =   HT6022_Firmware;
    HT6022BX_Info BL;
    BL.Name     =   "6022BL";
    BL.FwSize   =   HT6022BL_FIRMWARE_SIZE;
    BL.IdProduct=   HT6022BL_MODEL;
    BL.IdVendor =   HT6022_FW_VENDOR;
    BL.Firmware =   HT6022BL_Firmware;
    this->HantekDevices->append(BE);
    this->HantekDevices->append(BL);
    this->DeviceInit();
}
HT6022bx::~HT6022bx()
{
    this->DeviceExit();
}
void HT6022bx::FirmwareInstall(QString *DeviceName)
{
    qDebug() << "HT6022bx::FirmwareInstall(" << DeviceName << ")";
    Q_UNUSED(DeviceName);
    int IdProduct;
    int IdVendor;
    int FwSize;
    unsigned char*  Firmware;
    for (int i = 0; i < this->HantekDevices->size(); ++i) {
        if (this->HantekDevices->at(i).Name == *DeviceName)
        {
            IdProduct   = this->HantekDevices->at(i).IdProduct;
            IdVendor    = this->HantekDevices->at(i).IdVendor;
            FwSize      = this->HantekDevices->at(i).FwSize;
            Firmware    = this->HantekDevices->at(i).Firmware;
        }
    }
    libusb_device_handle  *Dev_handle;
    Dev_handle = libusb_open_device_with_vid_pid(NULL, IdVendor, IdProduct);
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
HT6022_ErrorTypeDef HT6022bx::DeviceInit()
{
    qDebug("HT6022bx::DeviceInit()");
    if (libusb_init(NULL) == 0)
    {
        return HT6022_SUCCESS;
    }else{
        return HT6022_ERROR_OTHER;
    }
    return HT6022_SUCCESS;
}
void HT6022bx::DeviceExit()
{
    qDebug("HT6022bx::DeviceExit()");
    libusb_exit(NULL);
    return;
}
void HT6022bx::searchDevice(QString *DeviceName)
{
    qDebug() << "HT6022bx::searchDevice("<< DeviceName << ")";
    int IdProduct;
    int IdVendor;
    for (int i = 0; i < this->HantekDevices->size(); ++i) {
        if (this->HantekDevices->at(i).Name == *DeviceName)
        {
            IdProduct   = this->HantekDevices->at(i).IdProduct;
            IdVendor    = this->HantekDevices->at(i).IdVendor;
        }
    }
    libusb_device_handle  *Dev_handle;
    Dev_handle = libusb_open_device_with_vid_pid(NULL, IdVendor, IdProduct);
    if (Dev_handle == 0)
    {
        emit deviceConnected(HT6022_ERROR_NO_DEVICE);
        return;
     }
    libusb_close(Dev_handle);
    emit deviceConnected(HT6022_SUCCESS);
    return;
}
void HT6022bx::getDevicesInfo()
{
    qDebug("HT6022bx::getDevicesInfo()");
    emit sendDevicesInfo(this->HantekDevices);
    return;
}

