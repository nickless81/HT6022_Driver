#include "ht6022bx.h"


HT6022bx::HT6022bx(QObject *parent) : QObject(parent)
{
    qDebug("HT6022bx::HT6022bx()");
    HT6022BX_Info BE;
    BE.Name         =   "6022BE";
    BE.FwSize       =   HT6022_FIRMWARE_SIZE;
    BE.IdProduct    =   HT6022BE_MODEL;
    BE.IdVendor     =   HT6022_VENDOR;
    BE.Firmware     =   HT6022_Firmware;
    BE.IdVendorFW   =   HT6022_VENDOR_FW;
    HT6022BX_Info BL;
    BL.Name         =   "6022BL";
    BL.FwSize       =   HT6022BL_FIRMWARE_SIZE;
    BL.IdProduct    =   HT6022BL_MODEL;
    BL.IdVendor     =   HT6022_VENDOR;
    BL.IdVendorFW   =   HT6022_VENDOR_FW;
    BL.Firmware =   HT6022BL_Firmware;
    this->HantekDevices = new QList<HT6022BX_Info>;
    this->HantekDevices->append(BE);
    this->HantekDevices->append(BL);
}
HT6022bx::~HT6022bx()
{
}
void HT6022bx::FirmwareInstall(const QString &DeviceName)
{
    qDebug() << "HT6022bx::FirmwareInstall(" << DeviceName << ")";
    Q_UNUSED(DeviceName);
    HT6022BX_Info *device;
    for (int i = 0; i < this->HantekDevices->size(); ++i) {
        if (this->HantekDevices->at(i).Name == DeviceName)
        {
            qDebug() << "Found Device Availabe on Library: Hantek" << DeviceName;
            device      = new HT6022BX_Info();
            *device     = this->HantekDevices->at(i);
        }
    }
    if(device != NULL)
    {
        qDebug("\n\tDebugind device instance");
        qDebug() << "Name:"         << device->Name;
        qDebug() << "IdVendor:"     << device->IdVendor;
        qDebug() << "IdProduct:"    << device->IdProduct;
        qDebug() << "FwSize:"       << device->FwSize;
    }
    HT6022_ErrorTypeDef errorCode;
    errorCode = DeviceInit();
    if(errorCode<0){
        qWarning("USB Library not initialized");
        emit deviceReady(errorCode);
        return;
    }
    libusb_device_handle  *Dev_handle;
    Dev_handle = libusb_open_device_with_vid_pid(NULL, device->IdVendor, device->IdProduct);
    if (Dev_handle == 0)
    {
        qWarning("USB Library cannot open device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        Dev_handle = libusb_open_device_with_vid_pid(NULL, device->IdVendorFW, device->IdProduct);
        if (Dev_handle != 0)
        {
            qDebug() << "Device Hantek"<< device->Name << "with Firmware installed connected";
            qDebug("Device Hantek IdVendor: 0x%04X IdProduct 0x%04X Connected and Ready to Used", device->IdVendorFW, device->IdProduct);
            emit deviceReady(HT6022_SUCCESS);
            return;
        }
        emit deviceReady(HT6022_ERROR_NO_DEVICE);
        return;
     }
    if(libusb_kernel_driver_active(Dev_handle, 0) == 1)
    {
        if(libusb_detach_kernel_driver(Dev_handle, 0) != 0)
        {
            qWarning("USB Library cannot detach_kernel_driver for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            libusb_close(Dev_handle);
            this->DeviceExit();
            emit deviceReady(HT6022_ERROR_OTHER);
            return;
        }
    }
    if(libusb_claim_interface(Dev_handle, 0) < 0)
    {
        libusb_close(Dev_handle);
        this->DeviceExit();
        qWarning("USB Library cannot claim_interface for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        emit deviceReady(HT6022_ERROR_OTHER);
        return;
    }
    unsigned int    Size;
    unsigned int    Value;
    if(!device->Firmware)
    {
        qWarning() << "Firmware NULL for device "<< DeviceName;
        emit deviceReady(HT6022_ERROR_OTHER);
    }
    while (device->FwSize)
    {
        Size  = *device->Firmware + ((*(device->Firmware + 1))<<0x08);
        device->Firmware = device->Firmware + 2;
        Value = *device->Firmware + ((*(device->Firmware + 1))<<0x08);
        device->Firmware = device->Firmware + 2;
        if (libusb_control_transfer (   Dev_handle,
                                        HT6022_FIRMWARE_REQUEST_TYPE,
                                        HT6022_FIRMWARE_REQUEST,
                                        Value,
                                        HT6022_FIRMWARE_INDEX,
                                        device->Firmware,
                                        Size,
                                        0) != (int)Size)
        {
            libusb_release_interface(Dev_handle, 0);
            libusb_close(Dev_handle);
            this->DeviceExit();
            qWarning("USB Library cannot contrl_transfer the Firmware for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            emit deviceReady(HT6022_ERROR_OTHER);
            return;
        }
        device->Firmware = device->Firmware + Size;
        device->FwSize--;
    }
    libusb_release_interface(Dev_handle, 0);
    libusb_close(Dev_handle);
    this->DeviceExit();
    qDebug() << "Hantek Device Connected: " << DeviceName;
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
void HT6022bx::searchDevice(const QString &DeviceName)
{
    qDebug() << "HT6022bx::searchDevice("<< DeviceName << ")";
    int IdProduct;
    int IdVendor;
    int IdVendorFW;
    HT6022_ErrorTypeDef errorCode;
    errorCode = DeviceInit();
    if(errorCode<0){
        qWarning("USB Library not initialized");
        emit deviceConnected(errorCode);
        return;
    }
    for (int i = 0; i < this->HantekDevices->size(); ++i) {
        if (this->HantekDevices->at(i).Name == DeviceName)
        {
            IdProduct   = this->HantekDevices->at(i).IdProduct;
            IdVendor    = this->HantekDevices->at(i).IdVendor;
            IdVendorFW  = this->HantekDevices->at(i).IdVendorFW;
        }
    }
    libusb_device_handle  *Dev_handle;
    Dev_handle = libusb_open_device_with_vid_pid(NULL, IdVendor, IdProduct);
    if (Dev_handle == 0)
    {
        qWarning("No Device without Firmware Connected");
        Dev_handle = libusb_open_device_with_vid_pid(NULL, IdVendorFW, IdProduct);
        if (Dev_handle != 0)
        {
            qDebug() << "Device Hantek"<< DeviceName << "with Firmware installed connected";
            qDebug("Device Hantek IdVendor: 0x%04X IdProduct 0x%04X Connected and Ready to Used", IdVendorFW, IdProduct);
            emit deviceConnected(HT6022_SUCCESS);
            return;
        }
        emit deviceConnected(HT6022_ERROR_NO_DEVICE);
        return;
    }
    libusb_close(Dev_handle);
    this->DeviceExit();
    qDebug() << "Hantek Device Connected: " << DeviceName;
    emit deviceConnected(HT6022_SUCCESS);
    return;
}
void HT6022bx::getDevicesInfo()
{
    qDebug("HT6022bx::getDevicesInfo()");
    if(!this->HantekDevices){
        qCritical("Hantek Devices List is not initialized");
        emit libError(HT6022_ERROR_NO_DEVICELIST);
    }
    this->printDevices();
    emit sendDevicesInfo(this->HantekDevices);
    return;
}
void HT6022bx::printDevices(){
    qDebug("HT6022bx::printDevices()");
    for (int i = 0; i < this->HantekDevices->size(); ++i) {
        qDebug() << "Model:\t" << this->HantekDevices->at(i).Name;
        qDebug("IdProduct: \t0x%04X",this->HantekDevices->at(i).IdProduct);
        qDebug("IdVendor: \t0x%04X" ,this->HantekDevices->at(i).IdVendor);
        qDebug("FwSize: \t%u"       ,this->HantekDevices->at(i).FwSize);
    }
}
