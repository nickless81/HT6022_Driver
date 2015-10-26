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
    HT6022BX_Info BL;
    BL.Name         =   "6022BL";
    BL.FwSize       =   HT6022BL_FIRMWARE_SIZE;
    BL.IdProduct    =   HT6022BL_MODEL;
    BL.IdVendor     =   HT6022_VENDOR;
    BL.IdVendorFW   =   HT6022_VENDOR_FW;
    BL.Firmware     =   HT6022BL_Firmware;
    this->HantekDevices = new QList<HT6022BX_Info>;
    this->HantekDevices->append(BE);
    this->HantekDevices->append(BL);
    this->UsbDriver = new LibUsbWrapper();
    this->UsbDriver->setEndpointOut(HT6022BX_WRITE_ENDPOINT);
    this->UsbDriver->setEndpointIn(HT6022BX_READ_ENDPOINT);
}
HT6022bx::~HT6022bx()
{
    delete this->HantekDevices;
    delete this->UsbDriver;
}
void HT6022bx::FirmwareInstall(const QString &DeviceName)
{
    qDebug() << "HT6022bx::FirmwareInstall(" << DeviceName << ")";
    int index = -1;
    for (int i = 0; i < this->HantekDevices->size(); ++i) {
        if (this->HantekDevices->at(i).Name == DeviceName)
        {
            qDebug() << "Found Device Availabe on Library: Hantek" << DeviceName;
            index = i;
            break;
        }
    }
    if(index >= 0)
    {
        this->FirmwareInstallIndex(index);
    }
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
        qDebug("IdVendor:  0x%4X",device->IdVendor);
        qDebug("IdProduct: 0x%4X",device->IdProduct);
        qDebug() << "FwSize:"       << device->FwSize;
    }
    HT6022BX_ErrorTypeDef errorCode;
    errorCode = DeviceInit();
    if(errorCode<0){
        qWarning("USB Library not initialized");
        emit deviceReady(errorCode);
        return;
    }
    int Dev_handle_int;
    Dev_handle_int = this->UsbDriver->openDevice(&device->IdVendor, &device->IdProduct);
    if (Dev_handle_int < 0)
    {
        qWarning("USB Library cannot open device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        //DeviceHandle = libusb_open_device_with_vid_pid(NULL, device->IdVendorFW, device->IdProduct);
        Dev_handle_int = this->UsbDriver->openDevice(&device->IdVendorFW, &device->IdProduct);
        if (Dev_handle_int == 0)
        {
            qDebug() << "Device Hantek"<< device->Name << "with Firmware installed connected";
            qDebug("Device Hantek IdVendor: 0x%04X IdProduct 0x%04X Connected and Ready to Used", device->IdVendorFW, device->IdProduct);
            emit deviceReady(HT6022BX_FW_SUCCESS);
            return;
        }
        emit deviceReady(HT6022BX_ERROR_NO_DEVICE);
        return;
     }
    if(this->UsbDriver->kernelDriverActive(0) == 1)
    {
        if(this->UsbDriver->detachKernelDriver(0) != 0)
        {
            qWarning("USB Library cannot detach_kernel_driver for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            //libusb_close(DeviceHandle);
            this->UsbDriver->closeDevice();
            this->DeviceExit();
            emit deviceReady(HT6022BX_ERROR_OTHER);
            return;
        }
    }
    if(this->UsbDriver->claimInterface(0) < 0)
    {
        this->deviceClose(0);
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
        if (this->UsbDriver->controlTransfer(   HT6022_FIRMWARE_REQUEST_TYPE,
                                                HT6022_FIRMWARE_REQUEST,
                                                device->Firmware,
                                                Size,
                                                Value,
                                                HT6022_FIRMWARE_INDEX
                                            ) != (int)Size)
        {
            this->UsbDriver->releaseInterface(0);
            this->UsbDriver->closeDevice();
            this->DeviceExit();
            qWarning("USB Library cannot contrl_transfer the Firmware for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            emit deviceReady(HT6022BX_ERROR_OTHER);
            return;
        }
        device->Firmware = device->Firmware + Size;
        device->FwSize--;
    }
    this->deviceClose(0);
    this->DeviceExit();
    qDebug() << "Hantek Device Connected: " << device->Name;
    emit deviceReady(HT6022BX_SUCCESS);
    delete device;
    return;
}
HT6022BX_ErrorTypeDef HT6022bx::DeviceInit()
{
    qDebug("HT6022bx::DeviceInit()");
    if (this->UsbDriver->initDevice() == 0)
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
    this->UsbDriver->exitDevice();
    return;
}
void HT6022bx::searchDevice(const QString &DeviceName)
{
    qDebug() << "HT6022bx::searchDevice("<< DeviceName << ")";
    int index = -1;
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
            index = i;
            break;
        }
    }
    if(index >= 0)
    {
        this->searchDeviceIndex(index);
    }
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
    int Dev_handle_int;
    qint16 IdProduct = this->HantekDevices->at(index).IdProduct;
    qint16 IdVendor = this->HantekDevices->at(index).IdVendor;
    Dev_handle_int = this->UsbDriver->openDevice(&IdVendor, &IdProduct);
    if (Dev_handle_int < 0)
    {
        qWarning("No Device without Firmware Connected");
        qint16 IdVendor = this->HantekDevices->at(index).IdVendorFW;
        Dev_handle_int = this->UsbDriver->openDevice(&IdVendor, &IdProduct);
        if (Dev_handle_int == 0)
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
    this->UsbDriver->closeDevice();
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
HT6022BX_ErrorTypeDef HT6022bx::deviceOpen(const unsigned int index)
{
    qDebug() << "HT6022bx::deviceOpen Hantek:" << this->HantekDevices->at(index).Name;
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
        return HT6022BX_ERROR_OTHER;
    }
    int Dev_handle_int;
    Dev_handle_int = this->UsbDriver->openDevice(&device->IdVendorFW, &device->IdProduct);
    if (Dev_handle_int < 0)
    {
        qWarning("USB Library cannot open device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendorFW,device->IdProduct);
        emit deviceReady(HT6022BX_ERROR_NO_DEVICE);
        return HT6022BX_ERROR_NO_DEVICE;
    }
    if(this->UsbDriver->kernelDriverActive(0) == 1)
    {
        if(this->UsbDriver->detachKernelDriver(0) != 0)
        {
            qWarning("USB Library cannot detach_kernel_driver for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
            //this->UsbDriver->closeDevice();
            this->deviceClose(0);
            this->DeviceExit();
            emit deviceReady(HT6022BX_ERROR_OTHER);
            return HT6022BX_ERROR_OTHER;
        }
    }
    if(this->UsbDriver->claimInterface(0) < 0)
    {
        this->deviceClose(0);
        this->DeviceExit();
        qWarning("USB Library cannot claim_interface for device IdVendor: 0x%04X, IdProduct: 0x%04X",device->IdVendor,device->IdProduct);
        emit deviceReady(HT6022BX_ERROR_OTHER);
        return HT6022BX_ERROR_OTHER;
    }
    //add emitted signal(s)
    return HT6022BX_SUCCESS;
}
void HT6022bx::deviceClose(int interface)
{
    qDebug() << "HT6022bx::deviceClose()";
    this->UsbDriver->releaseInterface(interface);
    this->UsbDriver->closeDevice();
}
HT6022BX_ErrorTypeDef HT6022bx::readData(const unsigned int index,unsigned char* CH1, unsigned char* CH2,HT6022BX_DataSizeTypeDef DataSize,unsigned int  timeout)
{
    qDebug() << "HT6022bx::readData Hantek:" << this->HantekDevices->at(index).Name;
    Q_UNUSED(timeout);
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
    if (!IS_HT6022BX_DATASIZE (DataSize) ||  (CH1 == NULL) ||  (CH2 == NULL) )
    {
        free(data);
        //add emitted signal(s)
        return HT6022BX_ERROR_INVALID_PARAM;
    }

    *data = HT6022BX_READ_CONTROL_DATA;
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_READ_CONTROL_REQUEST_TYPE,
                    HT6022BX_READ_CONTROL_REQUEST,
                    data,
                    HT6022BX_READ_CONTROL_SIZE,
                    HT6022BX_READ_CONTROL_VALUE,
                    HT6022BX_READ_CONTROL_INDEX);
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
    errorCode = this->UsbDriver->bulkReadDevice(
                    data,
                    DataSize*2,
                    &nread);
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
HT6022BX_ErrorTypeDef HT6022bx::readDataMulti (const unsigned int index,unsigned char* CH1, unsigned char* CH2,HT6022BX_DataSizeTypeDef DataSize,unsigned int  timeout)
{
    qDebug() << "HT6022bx::readDataMulti Hantek:" << this->HantekDevices->at(index).Name;
    qDebug("Work in Progress");
    Q_UNUSED(timeout);
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
    if (!IS_HT6022BX_DATASIZE (DataSize) ||  (CH1 == NULL) ||  (CH2 == NULL))
    {
        free(data);
        //add emitted signal(s)
        return HT6022BX_ERROR_INVALID_PARAM;
    }

    *data = HT6022BX_READ_CONTROL_DATA;
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_READ_CONTROL_REQUEST_TYPE,
                    HT6022BX_READ_CONTROL_REQUEST,
                    data,
                    HT6022BX_READ_CONTROL_SIZE,
                    HT6022BX_READ_CONTROL_VALUE,
                    HT6022BX_READ_CONTROL_INDEX);
    /**/
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
        errorCode = this->UsbDriver->bulkReadDevice(
                        data,
                        DataSize*2,
                        &nread);
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
HT6022BX_ErrorTypeDef HT6022bx::setXXX(const unsigned int index)
{
    qDebug() << "HT6022bx::setXXX Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    unsigned char data = HT6022BX_READ_CONTROL_DATA;
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_XXX_REQUEST_TYPE,
                    HT6022BX_XXX_REQUEST,
                    &data,
                    HT6022BX_XXX_SIZE,
                    HT6022BX_XXX_VALUE,
                    HT6022BX_XXX_INDEX);
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
HT6022BX_ErrorTypeDef HT6022bx::setCalValues(const unsigned int index,unsigned char* CalValues,HT6022BX_CVSizeTypeDef CVSize)
{
    qDebug() << "HT6022bx::setCalValues Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    if (!IS_HT6022BX_CVSIZE (CVSize) ||  (CalValues == NULL))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_SETCALLEVEL_REQUEST_TYPE,
                    HT6022BX_SETCALLEVEL_REQUEST,
                    CalValues,
                    CVSize,
                    HT6022BX_SETCALLEVEL_VALUE,
                    HT6022BX_SETCALLEVEL_INDEX);
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
HT6022BX_ErrorTypeDef HT6022bx::getCalValues(const unsigned int index,unsigned char* CalValues,HT6022BX_CVSizeTypeDef CVSize)
{
    qDebug() << "HT6022bx::getCalValues Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    if (!IS_HT6022BX_CVSIZE (CVSize) ||  (CalValues == NULL))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_GETCALLEVEL_REQUEST_TYPE,
                    HT6022BX_GETCALLEVEL_REQUEST,
                    CalValues,
                    CVSize,
                    HT6022BX_GETCALLEVEL_VALUE,
                    HT6022BX_GETCALLEVEL_INDEX);
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
HT6022BX_ErrorTypeDef HT6022bx::setSR(const unsigned int index,HT6022BX_SRTypeDef SR)
{
    qDebug() << "HT6022bx::setSR Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    unsigned char SampleRate = SR;
    qDebug("SR = 0x%X",SR);
    //if ((!IS_HT6022BX_SR (SR)) || (Device == NULL))
    if (!IS_HT6022BX_SR (SR))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_SR_REQUEST_TYPE,
                    HT6022BX_SR_REQUEST,
                    &SampleRate,
                    HT6022BX_SR_SIZE,
                    HT6022BX_SR_VALUE,
                    HT6022BX_SR_INDEX);
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
HT6022BX_ErrorTypeDef HT6022bx::setCH1IR(const unsigned int index,HT6022BX_IRTypeDef IR)
{
    qDebug() << "HT6022bx::setCH1IR Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    unsigned char InputRange = IR;
    qDebug("IR = 0x%X",IR);
    if (!IS_HT6022BX_IR (IR))
    {
        return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_IR1_REQUEST_TYPE,
                    HT6022BX_IR1_REQUEST,
                    &InputRange,
                    HT6022BX_IR1_SIZE,
                    HT6022BX_IR1_VALUE,
                    HT6022BX_IR1_INDEX);
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
HT6022BX_ErrorTypeDef HT6022bx::setCH2IR(const unsigned int index,HT6022BX_IRTypeDef IR)
{
    qDebug() << "HT6022bx::setCH2IR Hantek:" << this->HantekDevices->at(index).Name;
    int errorCode;
    unsigned char InputRange = IR;
    qDebug("IR = 0x%X",IR);
    if (!IS_HT6022BX_IR (IR))
    {
         return HT6022BX_ERROR_INVALID_PARAM;
    }
    errorCode = this->UsbDriver->controlTransfer(
                    HT6022BX_IR2_REQUEST_TYPE,
                    HT6022BX_IR2_REQUEST,
                    &InputRange,
                    HT6022BX_IR2_SIZE,
                    HT6022BX_IR2_VALUE,
                    HT6022BX_IR2_INDEX);
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
