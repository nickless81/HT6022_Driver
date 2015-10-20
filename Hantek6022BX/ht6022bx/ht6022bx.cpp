#include "ht6022bx.h"


HT6022bx::HT6022bx(QObject *parent) : QObject(parent)
{
    qDebug("HT6022bx::HT6022bx()");
    HT6022BX_Info BE;
    BE.Name         =   "6022BE";
    BE.FwSize       =   HT6022_FIRMWARE_SIZE;
    BE.IdProduct    =   HT6022BE_MODEL;
    BE.IdVendor     =   HT6022_VENDOR;
    BE.IdVendorFW   =   HT6022_VENDOR_FW;
    BE.Firmware     =   HT6022_Firmware;
    //BE.Device->DeviceHandle = NULL;
    HT6022BX_Info BL;
    BL.Name         =   "6022BL";
    BL.FwSize       =   HT6022BL_FIRMWARE_SIZE;
    BL.IdProduct    =   HT6022BL_MODEL;
    BL.IdVendor     =   HT6022_VENDOR;
    BL.IdVendorFW   =   HT6022_VENDOR_FW;
    BL.Firmware =   HT6022BL_Firmware;
    BL.Firmware     =   HT6022BL_Firmware;
    //BE.Device->DeviceHandle = NULL;
    this->HantekDevices = new QList<HT6022BX_Info>;
    this->HantekDevices->append(BE);
    this->HantekDevices->append(BL);
    this->DeviceList  =   new QList<HT6022BX_DeviceTypeDef>;
    HT6022BX_DeviceTypeDef *tmpDevices;
    for(int i=0;i<HT6022BX_NUMBER_DEVICES;i++)
    {
        tmpDevices = new HT6022BX_DeviceTypeDef;
        this->DeviceList->append(*tmpDevices);
    }
    this->AddressList = new QList<unsigned char>;
    qDebug("HT6022_AddressList");
    if(this->DeviceList->size()!=0)
    {
        qDebug("Size: %u",this->DeviceList->size());
    }else{
        qDebug("this->Devices empty");
    }
    for(int i=0;i<HT6022BL_AddressListSize;i++ )
    {
        this->AddressList->append(0x00);
    }
    /*
    *this->IR1->bmRequestType           =   0x40;
    *this->IR1->bRequest                =   0xE0;
    *this->IR1->wValue                  =   0x00;
    *this->IR1->wIndex                  =   0x00;
    *this->IR1->wLength                 =   0x01;
    *this->IR2->bmRequestType           =   0x40;
    *this->IR2->bRequest                =   0xE1;
    *this->IR2->wValue                  =   0x00;
    *this->IR2->wIndex                  =   0x00;
    *this->IR2->wLength                 =   0x01;
    *this->SR->bmRequestType            =   0x40;
    *this->SR->bRequest                 =   0xE2;
    *this->SR->wValue                   =   0x00;
    *this->SR->wIndex                   =   0x00;
    *this->SR->wLength                  =   0x01;
    *this->SetCalLevel->bmRequestType   =   0xC0;
    *this->SetCalLevel->bRequest        =   0xA2;
    *this->SetCalLevel->wValue          =   0x08;
    *this->SetCalLevel->wIndex          =   0x00;
    *this->GetCalLevel->bmRequestType   =   0x40;
    *this->GetCalLevel->bRequest        =   0xA2;
    *this->GetCalLevel->wValue          =   0x08;
    *this->GetCalLevel->wIndex          =   0x00;
    *this->ReadControl->bmRequestType   =   0x40;
    *this->ReadControl->bRequest        =   0xE3;
    *this->ReadControl->wValue          =   0x00;
    *this->ReadControl->wIndex          =   0x00;
    *this->ReadControl->wLength         =   0x01;
    *this->ReadControl->data            =   0x01;
    */
    this->UsbDriver = new LibUsbWrapper();
}
HT6022bx::~HT6022bx()
{
    delete this->HantekDevices;
    delete this->DeviceList;
    delete this->AddressList;
    delete this->UsbDriver;
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
    HT6022BX_ErrorTypeDef errorCode;
    errorCode = DeviceInit();
    if(errorCode<0){
        qWarning("USB Library not initialized");
        emit deviceReady(errorCode);
        return;
    }
    libusb_device_handle *DeviceHandle;
    DeviceHandle = libusb_open_device_with_vid_pid(NULL, device->IdVendor, device->IdProduct);
    if (DeviceHandle == 0)
    {
        qWarning("USB Library cannot open device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        DeviceHandle = libusb_open_device_with_vid_pid(NULL, device->IdVendorFW, device->IdProduct);
        if (DeviceHandle != 0)
        {
            qDebug() << "Device Hantek"<< device->Name << "with Firmware installed connected";
            qDebug("Device Hantek IdVendor: 0x%04X IdProduct 0x%04X Connected and Ready to Used", device->IdVendorFW, device->IdProduct);
            emit deviceReady(HT6022BX_FW_SUCCESS);
            return;
        }
        emit deviceReady(HT6022BX_ERROR_NO_DEVICE);
        return;
    }

    if(libusb_kernel_driver_active(DeviceHandle, 0) == 1)
    {
        if(libusb_detach_kernel_driver(DeviceHandle, 0) != 0)
        {
            qWarning("USB Library cannot detach_kernel_driver for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            libusb_close(DeviceHandle);
            this->DeviceExit();
            emit deviceReady(HT6022BX_ERROR_OTHER);
            return;
        }
    }
    if(libusb_claim_interface(DeviceHandle, 0) < 0)
    {
        libusb_close(DeviceHandle);
        this->DeviceExit();
        qWarning("USB Library cannot claim_interface for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        emit deviceReady(HT6022BX_ERROR_OTHER);
        return;
    }
    unsigned int    Size;
    unsigned int    Value;
    if(!device->Firmware)
    {
        qWarning() << "Firmware NULL for device "<< DeviceName;
        emit deviceReady(HT6022BX_ERROR_OTHER);
    }
    while (device->FwSize)
    {
        Size  = *device->Firmware + ((*(device->Firmware + 1))<<0x08);
        device->Firmware = device->Firmware + 2;
        Value = *device->Firmware + ((*(device->Firmware + 1))<<0x08);
        device->Firmware = device->Firmware + 2;
        if (libusb_control_transfer (   DeviceHandle,
                                        HT6022_FIRMWARE_REQUEST_TYPE,
                                        HT6022_FIRMWARE_REQUEST,
                                        Value,
                                        HT6022_FIRMWARE_INDEX,
                                        device->Firmware,
                                        Size,
                                        0) != (int)Size)
        {
            libusb_release_interface(DeviceHandle, 0);
            libusb_close(DeviceHandle);
            this->DeviceExit();
            qWarning("USB Library cannot control_transfer the Firmware for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            emit deviceReady(HT6022BX_ERROR_OTHER);
            return;
        }
        device->Firmware = device->Firmware + Size;
        device->FwSize--;
    }
    libusb_release_interface(DeviceHandle, 0);
    libusb_close(DeviceHandle);
    this->DeviceExit();
    qDebug() << "Hantek Device Connected: " << DeviceName;
    emit deviceReady(HT6022BX_SUCCESS);
    return;
}

void HT6022bx::FirmwareInstallIndex(const unsigned int index)
{
    qDebug() << "HT6022bx::FirmwareInstall(" << index << ")";
    HT6022BX_Info *device = new HT6022BX_Info();
    *device     = this->HantekDevices->at(index);
    if(device != NULL)
    {
        qDebug("\n\tDebugind device instance");
        qDebug() << "Name:"         << device->Name;
        qDebug() << "IdVendor:"     << device->IdVendor;
        qDebug() << "IdProduct:"    << device->IdProduct;
        qDebug() << "FwSize:"       << device->FwSize;
    }
    HT6022BX_ErrorTypeDef errorCode;
    errorCode = DeviceInit();
    if(errorCode<0){
        qWarning("USB Library not initialized");
        emit deviceReady(errorCode);
        return;
    }
    libusb_device_handle  *DeviceHandle;
    DeviceHandle = libusb_open_device_with_vid_pid(NULL, device->IdVendor, device->IdProduct);
    if (DeviceHandle == 0)
    {
        qWarning("USB Library cannot open device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        DeviceHandle = libusb_open_device_with_vid_pid(NULL, device->IdVendorFW, device->IdProduct);
        if (DeviceHandle != 0)
        {
            qDebug() << "Device Hantek"<< device->Name << "with Firmware installed connected";
            qDebug("Device Hantek IdVendor: 0x%04X IdProduct 0x%04X Connected and Ready to Used", device->IdVendorFW, device->IdProduct);
            emit deviceReady(HT6022BX_FW_SUCCESS);
            return;
        }
        emit deviceReady(HT6022BX_ERROR_NO_DEVICE);
        return;
     }
    if(libusb_kernel_driver_active(DeviceHandle, 0) == 1)
    {
        if(libusb_detach_kernel_driver(DeviceHandle, 0) != 0)
        {
            qWarning("USB Library cannot detach_kernel_driver for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            libusb_close(DeviceHandle);
            this->DeviceExit();
            emit deviceReady(HT6022BX_ERROR_OTHER);
            return;
        }
    }
    if(libusb_claim_interface(DeviceHandle, 0) < 0)
    {
        libusb_close(DeviceHandle);
        this->DeviceExit();
        qWarning("USB Library cannot claim_interface for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        emit deviceReady(HT6022BX_ERROR_OTHER);
        return;
    }
    unsigned int    Size;
    unsigned int    Value;
    if(!device->Firmware)
    {
        qWarning() << "Firmware NULL for device "<< device->Name;
        emit deviceReady(HT6022BX_ERROR_OTHER);
    }
    while (device->FwSize)
    {
        Size  = *device->Firmware + ((*(device->Firmware + 1))<<0x08);
        device->Firmware = device->Firmware + 2;
        Value = *device->Firmware + ((*(device->Firmware + 1))<<0x08);
        device->Firmware = device->Firmware + 2;
        if (libusb_control_transfer (   DeviceHandle,
                                        HT6022_FIRMWARE_REQUEST_TYPE,
                                        HT6022_FIRMWARE_REQUEST,
                                        Value,
                                        HT6022_FIRMWARE_INDEX,
                                        device->Firmware,
                                        Size,
                                        0) != (int)Size)
        {
            libusb_release_interface(DeviceHandle, 0);
            libusb_close(DeviceHandle);
            this->DeviceExit();
            qWarning("USB Library cannot contrl_transfer the Firmware for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            emit deviceReady(HT6022BX_ERROR_OTHER);
            return;
        }
        device->Firmware = device->Firmware + Size;
        device->FwSize--;
    }
    libusb_release_interface(DeviceHandle, 0);
    libusb_close(DeviceHandle);
    this->DeviceExit();
    qDebug() << "Hantek Device Connected: " << device->Name;
    emit deviceReady(HT6022BX_SUCCESS);
    delete device;
    return;
}
HT6022BX_ErrorTypeDef HT6022bx::DeviceInit()
{
    qDebug("HT6022bx::DeviceInit()");
    if (libusb_init(NULL) == 0)
    {
        return HT6022BX_SUCCESS;
    }else{
        return HT6022BX_ERROR_OTHER;
    }
    return HT6022BX_SUCCESS;
}
void HT6022bx::DeviceExit()
{
    qDebug("HT6022bx::DeviceExit()");
    libusb_exit(NULL);
    //this->UsbDriver->exitDevice();
    return;
}
void HT6022bx::searchDevice(const QString &DeviceName)
{
    qDebug() << "HT6022bx::searchDevice("<< DeviceName << ")";
    int IdProduct;
    int IdVendor;
    int IdVendorFW;
    HT6022BX_ErrorTypeDef errorCode;
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
            qDebug("\n\temitting deviceConnected(%u)\n",HT6022BX_FW_SUCCESS);
            emit deviceConnected(HT6022BX_FW_SUCCESS);
            return;
        }
        emit deviceConnected(HT6022BX_ERROR_NO_DEVICE);
        return;
    }
    libusb_close(Dev_handle);
    this->DeviceExit();
    qDebug() << "Hantek Device Connected: " << DeviceName;
    qDebug("\n\temitting deviceConnected(%u)\n",HT6022BX_DEVICE_CONNECTED);
    emit deviceConnected(HT6022BX_DEVICE_CONNECTED);
    emit enableDownload();
    return;
}
void HT6022bx::searchDeviceIndex(const unsigned int index)
{
    qDebug() << "HT6022bx::searchDeviceIndex("<< index << ")";
    HT6022BX_ErrorTypeDef errorCode;
    errorCode = DeviceInit();
    if(errorCode<0){
        qWarning("USB Library not initialized");
        emit deviceConnected(errorCode);
        return;
    }
    libusb_device_handle  *Dev_handle;
    Dev_handle = libusb_open_device_with_vid_pid(NULL, this->HantekDevices->at(index).IdVendor, this->HantekDevices->at(index).IdProduct);
    if (Dev_handle == 0)
    {
        qWarning("No Device without Firmware Connected");
        Dev_handle = libusb_open_device_with_vid_pid(NULL, this->HantekDevices->at(index).IdVendorFW, this->HantekDevices->at(index).IdProduct);
        if (Dev_handle != 0)
        {
            qDebug() << "Device Hantek"<< this->HantekDevices->at(index).Name << "with Firmware installed connected";
            qDebug("Device Hantek IdVendor: 0x%04X IdProduct 0x%04X Connected and Ready to Used", this->HantekDevices->at(index).IdVendorFW, this->HantekDevices->at(index).IdProduct);
            qDebug("\n\temitting deviceConnected(%u)\n",HT6022BX_FW_SUCCESS);
            emit deviceConnected(HT6022BX_FW_SUCCESS);
            return;
        }
        emit deviceConnected(HT6022BX_ERROR_NO_DEVICE);
        return;
    }
    libusb_close(Dev_handle);
    this->DeviceExit();
    qDebug() << "Hantek Device Connected: " << this->HantekDevices->at(index).Name;
    qDebug("\n\temitting deviceConnected(%u)\n",HT6022BX_DEVICE_CONNECTED);
    emit deviceConnected(HT6022BX_DEVICE_CONNECTED);
    emit enableDownload();
    return;
}
void HT6022bx::getDevicesInfo()
{
    qDebug("HT6022bx::getDevicesInfo()");
    if(!this->HantekDevices){
        qCritical("Hantek Devices List is not initialized");
        emit libError(HT6022BX_ERROR_NO_DEVICELIST);
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
HT6022BX_ErrorTypeDef HT6022bx::deviceOpen(HT6022BX_DeviceTypeDef *Device, const unsigned int index)
{
    qDebug() << "HT6022bx::deviceOpen Hantek:" << this->HantekDevices->at(index).Name;
    struct libusb_device_descriptor desc;
    libusb_device         **DeviceList;
    libusb_device_handle  *DeviceHandle;
    int DeviceCount;
    unsigned char Address;
    int DeviceIterator;
    if (Device == NULL)
    {
        //add emitted signal(s)
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    Device->Address = 0;
    Device->DeviceHandle = NULL;

    qDebug("Get device list");
    DeviceCount = libusb_get_device_list(NULL, &DeviceList);
    if (DeviceCount <= 0)
    {
        //add emitted signal(s)
        return HT6022BX_ERROR_OTHER;
    }
    for(DeviceIterator = 0; DeviceIterator < DeviceCount; DeviceIterator++)
    {
        Address = libusb_get_device_address (DeviceList[DeviceIterator]);
        if (this->AddressList->at(Address) == 0)
        {
            qDebug("Get device descriptor");
            if (libusb_get_device_descriptor(DeviceList[DeviceIterator], &desc) == 0)
            {
                qDebug("Check VID and PID");
                if ((desc.idVendor  == this->HantekDevices->at(index).IdVendorFW)&&(desc.idProduct == this->HantekDevices->at(index).IdProduct))
                {
                    qDebug() << "Found device Hantek:" << this->HantekDevices->at(index).Name;
                    break;
                }
            }
        }
    }
    if (DeviceIterator == DeviceCount)
    {
        libusb_free_device_list(DeviceList, 1);
        //add emitted signal(s)
        return HT6022BX_ERROR_NO_DEVICE;
    }
    int errorCode;
    errorCode = libusb_open(DeviceList[DeviceIterator], &DeviceHandle);
    libusb_free_device_list(DeviceList, 1);
    if (errorCode != 0)
    {
        if (errorCode != HT6022BX_ERROR_NO_MEM && errorCode != HT6022BX_ERROR_ACCESS)
        {
            //add emitted signal(s)
            return  HT6022BX_ERROR_OTHER;
        }
        //add emitted signal(s)
        return (errorCode == HT6022BX_ERROR_NO_MEM)?HT6022BX_ERROR_NO_MEM:HT6022BX_ERROR_ACCESS;
    }
    if(libusb_kernel_driver_active(DeviceHandle, 0) == 1)
        if(libusb_detach_kernel_driver(DeviceHandle, 0) != 0)
        {
            libusb_close(DeviceHandle);
            //add emitted signal(s)
            return HT6022BX_ERROR_OTHER;
        }

    if(libusb_claim_interface(DeviceHandle, 0) != 0)
    {
        libusb_close(DeviceHandle);
        //add emitted signal(s)
        return HT6022BX_ERROR_OTHER;
    }
    this->AddressList->replace(Address,0x01);
    Device->Address      = Address;
    Device->DeviceHandle = DeviceHandle;
    //add emitted signal(s)
    return HT6022BX_SUCCESS;
}
void HT6022bx::deviceClose(HT6022BX_DeviceTypeDef *Device, const unsigned int index)
{
    qDebug() << "HT6022bx::deviceClose Hantek:" << this->HantekDevices->at(index).Name;
    if (Device != NULL)
    {
        libusb_release_interface(Device->DeviceHandle, 0);
        libusb_close(Device->DeviceHandle);
        this->AddressList->replace(Device->Address,0x00);
        Device->DeviceHandle = NULL;
        Device->Address = 0;
    }
    return;
}
HT6022BX_ErrorTypeDef HT6022bx::readData(HT6022BX_DeviceTypeDef *Device, const unsigned int index,unsigned char* CH1, unsigned char* CH2,HT6022BX_DataSizeTypeDef DataSize,unsigned int  timeout)
{
    qDebug() << "HT6022bx::readData Hantek:" << this->HantekDevices->at(index).Name;
    unsigned char *data;
    unsigned char *data_temp;
    int nread;
    int errorCode;

    data = (unsigned char*) malloc (sizeof(unsigned char)*DataSize*2);
    if (data == 0)
    {
        //add emitted signal(s)
        return HT6022BX_ERROR_NO_MEM;
    }
    if ((!IS_HT6022BX_DATASIZE (DataSize)) || (Device == NULL) ||  (CH1 == NULL) ||  (CH2 == NULL))
    {
        free(data);
        //add emitted signal(s)
        return HT6022BX_ERROR_INVALID_PARAM;
    }

    *data = HT6022BX_READ_CONTROL_DATA;
    errorCode = libusb_control_transfer(Device->DeviceHandle,
                                        HT6022BX_READ_CONTROL_REQUEST_TYPE,
                                        HT6022BX_READ_CONTROL_REQUEST,
                                        HT6022BX_READ_CONTROL_VALUE,
                                        HT6022BX_READ_CONTROL_INDEX,
                                        data,
                                        HT6022BX_READ_CONTROL_SIZE, 0);
    if (errorCode != HT6022BX_READ_CONTROL_SIZE)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE)
        {
            //add emitted signal(s)
            return HT6022BX_ERROR_OTHER;
        }
        free(data);
        //add emitted signal(s)
        return HT6022BX_ERROR_NO_DEVICE;
    }
    errorCode = libusb_bulk_transfer(   Device->DeviceHandle,
                                        HT6022BX_READ_ENDPOINT,
                                        data,
                                        DataSize*2,
                                        &nread,
                                        timeout);

    if (errorCode != HT6022BX_SUCCESS || nread != DataSize*2)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE && errorCode != HT6022BX_ERROR_TIMEOUT)
        {
            //add emitted signal(s)
            return  HT6022BX_ERROR_OTHER;
        }
        free(data);
        //add emitted signal(s)
        return (errorCode == HT6022BX_ERROR_NO_DEVICE)? HT6022BX_ERROR_NO_DEVICE: HT6022BX_ERROR_TIMEOUT;
    }
    qDebug("\nnread = %d\n",nread);
    data_temp = data;
    while (nread)
    {
        *CH1++ = *data_temp++;
        *CH2++ = *data_temp++;
        nread -= 2;
    }
    free (data);
    //add emitted signal(s)
    return HT6022BX_SUCCESS;
}
HT6022BX_ErrorTypeDef HT6022bx::readDataMulti (HT6022BX_DeviceTypeDef *Device, const unsigned int index,unsigned char* CH1, unsigned char* CH2,HT6022BX_DataSizeTypeDef DataSize,unsigned int  timeout)
{
    qDebug() << "HT6022bx::readDataMulti Hantek:" << this->HantekDevices->at(index).Name;
    unsigned char *data;
    unsigned char *data_temp;
    int nread;
    int errorCode;

    data = (unsigned char*) malloc (sizeof(unsigned char)*DataSize*2);
    if (data == 0)
    {
        //add emitted signal(s)
        return HT6022BX_ERROR_NO_MEM;
    }
    if ((!IS_HT6022BX_DATASIZE (DataSize)) || (Device == NULL) ||  (CH1 == NULL) ||  (CH2 == NULL))
    {
        free(data);
        //add emitted signal(s)
        return HT6022BX_ERROR_INVALID_PARAM;
    }

    *data = HT6022BX_READ_CONTROL_DATA;
    errorCode = libusb_control_transfer(Device->DeviceHandle,
                                        HT6022BX_READ_CONTROL_REQUEST_TYPE,
                                        HT6022BX_READ_CONTROL_REQUEST,
                                        HT6022BX_READ_CONTROL_VALUE,
                                        HT6022BX_READ_CONTROL_INDEX,
                                        data,
                                        HT6022BX_READ_CONTROL_SIZE, 0);
    if (errorCode != HT6022BX_READ_CONTROL_SIZE)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE)
        {
            //add emitted signal(s)
            qDebug("HT6022BX_ERROR_NO_DEVICE");
            return HT6022BX_ERROR_OTHER;
        }
        qDebug(" free(data): HT6022BX_ERROR_NO_DEVICE");
        free(data);
        //add emitted signal(s)
        return HT6022BX_ERROR_NO_DEVICE;
    }
    for(int i=0;i<16;i++)
    {
        qDebug("readDataMulti : %u",i);
        /**
        if(!(data == NULL))
        {
            //free(data);
            data = (unsigned char*) malloc (sizeof(unsigned char)*DataSize*2);
        }
        **/
        errorCode = libusb_bulk_transfer(   Device->DeviceHandle,
                                            HT6022BX_READ_ENDPOINT,
                                            data,
                                            DataSize*2,
                                            &nread,
                                            timeout);
        if(nread == 0){
            break;
        }
        qDebug("Post libusb_bulk_transfer: errorCode %u  nread: %u",errorCode, nread);
        if (errorCode != HT6022BX_SUCCESS || nread != DataSize*2)
        {
            qDebug("errorCode != HT6022BX_SUCCESS || nread != DataSize*2");
            if (errorCode != HT6022BX_ERROR_NO_DEVICE && errorCode != HT6022BX_ERROR_TIMEOUT)
            {
                //add emitted signal(s)
                qDebug("HT6022BX_ERROR_OTHER");
                return  HT6022BX_ERROR_OTHER;
            }
            qDebug("Previo: (errorCode == HT6022BX_ERROR_NO_DEVICE)? HT6022BX_ERROR_NO_DEVICE: HT6022BX_ERROR_TIMEOUT");
            free(data);
            //add emitted signal(s)
            return (errorCode == HT6022BX_ERROR_NO_DEVICE)? HT6022BX_ERROR_NO_DEVICE: HT6022BX_ERROR_TIMEOUT;
        }
        qDebug("\nnread = %d\n",nread);
        data_temp = data;
        while (nread)
        {
            *CH1++ = *data_temp++;
            *CH2++ = *data_temp++;
            nread -= 2;
        }
        //free(data_temp);
        //free (data);
    }
    free (data);
    //add emitted signal(s)
    return HT6022BX_SUCCESS;
}
HT6022BX_ErrorTypeDef HT6022bx::setXXX(HT6022BX_DeviceTypeDef *Device, const unsigned int index)
{
    qDebug() << "HT6022bx::setXXX Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    /*
    if ((!IS_HT6022BX_CVSIZE (CVSize)) || (Device == NULL) ||  (CalValues == NULL))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    */
    /*
    #define HT6022BX_XXX_REQUEST_TYPE           0X40//64
    #define HT6022BX_XXX_REQUEST                0XE8//232
    #define HT6022BX_XXX_VALUE                  0X00
    #define HT6022BX_XXX_INDEX                  0X00
    #define HT6022BX_XXX_LENGTH                 0X01
    #define HT6022BX_XXX_SIZE                   0X01
*/
    unsigned char data = HT6022BX_READ_CONTROL_DATA;
    errorCode = libusb_control_transfer (Device->DeviceHandle,
                                         HT6022BX_XXX_REQUEST_TYPE,
                                         HT6022BX_XXX_REQUEST,
                                         HT6022BX_XXX_VALUE,
                                         HT6022BX_XXX_INDEX,
                                         &data,
                                         HT6022BX_XXX_SIZE, 0);
    if (errorCode != HT6022BX_XXX_SIZE)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE)
        {
            return  HT6022BX_ERROR_OTHER;
        }
        return HT6022BX_ERROR_NO_DEVICE;
    }
    return HT6022BX_SUCCESS;
}

HT6022BX_ErrorTypeDef HT6022bx::setCalValues(HT6022BX_DeviceTypeDef *Device, const unsigned int index,unsigned char* CalValues,HT6022BX_CVSizeTypeDef CVSize)
{
    qDebug() << "HT6022bx::setCalValues Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    if ((!IS_HT6022BX_CVSIZE (CVSize)) || (Device == NULL) ||  (CalValues == NULL))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = libusb_control_transfer (Device->DeviceHandle,
                                         HT6022BX_SETCALLEVEL_REQUEST_TYPE,
                                         HT6022BX_SETCALLEVEL_REQUEST,
                                         HT6022BX_SETCALLEVEL_VALUE,
                                         HT6022BX_SETCALLEVEL_INDEX,
                                         CalValues,
                                         CVSize, 0);
    if (errorCode != CVSize)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE)
        {
            return  HT6022BX_ERROR_OTHER;
        }
        return HT6022BX_ERROR_NO_DEVICE;
    }
    return HT6022BX_SUCCESS;
}
HT6022BX_ErrorTypeDef HT6022bx::getCalValues(HT6022BX_DeviceTypeDef *Device, const unsigned int index,unsigned char* CalValues,HT6022BX_CVSizeTypeDef CVSize)
{
    qDebug() << "HT6022bx::getCalValues Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    if ((!IS_HT6022BX_CVSIZE (CVSize)) || (Device == NULL) ||  (CalValues == NULL))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = libusb_control_transfer (
                    Device->DeviceHandle,
                    HT6022BX_GETCALLEVEL_REQUEST_TYPE,
                    HT6022BX_GETCALLEVEL_REQUEST,
                    HT6022BX_GETCALLEVEL_VALUE,
                    HT6022BX_GETCALLEVEL_INDEX,
                    CalValues,
                    CVSize, 0);
   if (errorCode != CVSize)
   {
       if (errorCode != HT6022BX_ERROR_NO_DEVICE)
       {
           return HT6022BX_ERROR_OTHER;
        }
        return HT6022BX_ERROR_NO_DEVICE;
   }
   return HT6022BX_SUCCESS;
}
HT6022BX_ErrorTypeDef HT6022bx::setSR(HT6022BX_DeviceTypeDef *Device, const unsigned int index,HT6022BX_SRTypeDef SR)
{
    qDebug() << "HT6022bx::setSR Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    unsigned char SampleRate = SR;
    qDebug("SR = 0x%X",SR);
    if ((!IS_HT6022BX_SR (SR)) || (Device == NULL))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = libusb_control_transfer (
                    Device->DeviceHandle,
                    HT6022BX_SR_REQUEST_TYPE,
                    HT6022BX_SR_REQUEST,
                    HT6022BX_SR_VALUE,
                    HT6022BX_SR_INDEX,
                    &SampleRate,
                    HT6022BX_SR_SIZE, 0);
    if (errorCode != HT6022BX_SR_SIZE)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE)
        {
            return HT6022BX_ERROR_OTHER;
        }
        return HT6022BX_ERROR_NO_DEVICE;
    }
    return HT6022BX_SUCCESS;
}
HT6022BX_ErrorTypeDef HT6022bx::setCH1IR(HT6022BX_DeviceTypeDef *Device, const unsigned int index,HT6022BX_IRTypeDef IR)
{
    qDebug() << "HT6022bx::setCH1IR Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    unsigned char InputRange = IR;
    qDebug("IR = 0x%X",IR);
    if ((!IS_HT6022BX_IR (IR)) || (Device == NULL))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = libusb_control_transfer (
                    Device->DeviceHandle,
                    HT6022BX_IR1_REQUEST_TYPE,
                    HT6022BX_IR1_REQUEST,
                    HT6022BX_IR1_VALUE,
                    HT6022BX_IR1_INDEX,
                    &InputRange,
                    HT6022BX_IR1_SIZE, 0);
    if (errorCode != HT6022BX_IR1_SIZE)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE)
        {
            return HT6022BX_ERROR_OTHER;
        }
        return HT6022BX_ERROR_NO_DEVICE;
    }
    return HT6022BX_SUCCESS;
}
HT6022BX_ErrorTypeDef HT6022bx::setCH2IR(HT6022BX_DeviceTypeDef *Device, const unsigned int index,HT6022BX_IRTypeDef IR)
{
    qDebug() << "HT6022bx::setCH2IR Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    unsigned char InputRange = IR;
    qDebug("IR = 0x%X",IR);
    if ((!IS_HT6022BX_IR (IR)) || (Device == NULL))
    {
         return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = libusb_control_transfer (
                    Device->DeviceHandle,
                    HT6022BX_IR2_REQUEST_TYPE,
                    HT6022BX_IR2_REQUEST,
                    HT6022BX_IR2_VALUE,
                    HT6022BX_IR2_INDEX,
                    &InputRange,
                    HT6022BX_IR2_SIZE, 0);
    if (errorCode != HT6022BX_IR2_SIZE)
    {
        if (errorCode != HT6022BX_ERROR_NO_DEVICE)
        {
            return HT6022BX_ERROR_OTHER;
        }
        return HT6022BX_ERROR_NO_DEVICE;
    }
    return HT6022BX_SUCCESS;
}
