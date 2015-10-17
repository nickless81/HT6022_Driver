#ifndef HT6022BX_GLOBAL_H
#define HT6022BX_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#include <libusb-1.0/libusb.h>
#include "HT6022fw.h"
#include "HT6022BLfw.h"
#if defined(HT6022BX_LIBRARY)
#  define HT6022BXSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HT6022BXSHARED_EXPORT Q_DECL_IMPORT
#endif
#define HT6022_VENDOR               0X04B4
#define HT6022_VENDOR_FW            0X04B5
#define HT6022BE_MODEL              0X6022
#define HT6022BL_MODEL              0X602A
#define HT6022BL_AddressListSize    256
#define HT6022BX_NUMBER_DEVICES     2
#define HT6022BX_READ_ENDPOINT      0X86
#define HT6022BX_CONTROL_SIZE       6
/**************************************************/
#define HT6022BX_IR1_REQUEST_TYPE           0X40
#define HT6022BX_IR1_REQUEST                0XE0
#define HT6022BX_IR1_VALUE                  0X00
#define HT6022BX_IR1_INDEX                  0X00
#define HT6022BX_IR1_SIZE                   0X01

#define HT6022BX_IR2_REQUEST_TYPE           0X40
#define HT6022BX_IR2_REQUEST                0XE1
#define HT6022BX_IR2_VALUE                  0X00
#define HT6022BX_IR2_INDEX                  0X00
#define HT6022BX_IR2_SIZE                   0X01

#define HT6022BX_SR_REQUEST_TYPE            0X40
#define HT6022BX_SR_REQUEST                 0XE2
#define HT6022BX_SR_VALUE                   0X00
#define HT6022BX_SR_INDEX                   0X00
#define HT6022BX_SR_SIZE                    0X01

#define HT6022BX_SETCALLEVEL_REQUEST_TYPE   0XC0
#define HT6022BX_SETCALLEVEL_REQUEST        0XA2
#define HT6022BX_SETCALLEVEL_VALUE          0X08
#define HT6022BX_SETCALLEVEL_INDEX          0X00

#define HT6022BX_GETCALLEVEL_REQUEST_TYPE   0X40
#define HT6022BX_GETCALLEVEL_REQUEST        0XA2
#define HT6022BX_GETCALLEVEL_VALUE          0X08
#define HT6022BX_GETCALLEVEL_INDEX          0X00

#define HT6022BX_READ_CONTROL_REQUEST_TYPE  0X40
#define HT6022BX_READ_CONTROL_REQUEST       0XE3
#define HT6022BX_READ_CONTROL_VALUE         0X00
#define HT6022BX_READ_CONTROL_INDEX         0X00
#define HT6022BX_READ_CONTROL_SIZE          0X01
#define HT6022BX_READ_CONTROL_DATA          0X01
#define HT6022BX_READ_BULK_PIPE             0X86//ENDPOINT
/**************************************************/
/*
int libusb_control_transfer(
        libusb_device_handle *devh,
        uint8_t bmRequestType,
        uint8_t bRequest,
        uint16_t wValue,
        uint16_t wIndex,
        unsigned char *data,
        uint16_t wLength,
        unsigned int timeout
        )
*/
typedef struct
{
    unsigned char   *bmRequestType;
    unsigned char   *bRequest;
    uint16_t        *wValue;
    uint16_t        *wIndex;
    uint16_t        *wLength;
    unsigned char   *data;
}HT6022BX_ControlTransfer;
typedef struct
{
    QString         Name;
    int             FwSize;
    int             IdProduct;
    int             IdVendor;
    int             IdVendorFW;
    unsigned char*  Firmware;
    libusb_device_handle *DeviceHandle;
    unsigned char Address;
}HT6022BX_UsbDevice;
typedef struct
{
    unsigned int        index;
    QString             Name;
    int                 FwSize;
    int                 IdProduct;
    int                 IdVendor;
    int                 IdVendorFW;
    unsigned char*      Firmware;
}HT6022BX_Info;

/**
  * @brief Error Code
  */
typedef enum
{
    HT6022_SUCCESS              =   0,//Firmware Installed Properly
    HT6022_FW_SUCCESS           =   1,//Device Connected with Firmware already Installed
    HT6022_DEVICE_CONNECTED     =   2,
    HT6022_ERROR_INVALID_PARAM  =  -2,
    HT6022_ERROR_ACCESS         =  -3,
    HT6022_ERROR_NO_DEVICE      =  -4,
    HT6022_ERROR_TIMEOUT        =  -7,
    HT6022_ERROR_NO_MEM         = -11,
    HT6022_ERROR_NO_DEVICELIST  = -12,
    HT6022_ERROR_OTHER          = -99
}HT6022BX_ErrorTypeDef;

#define  IS_HT6022_ERROR(ERROR) (((ERROR) == HT6022_SUCCESS) || \
                 ((ERROR) == HT6022_ERROR_INVALID_PARAM) || \
                 ((ERROR) == HT6022_ERROR_ACCESS )   || \
                 ((ERROR) == HT6022_ERROR_NO_DEVICE) || \
                 ((ERROR) == HT6022_ERROR_TIMEOUT)   || \
                 ((ERROR) == HT6022_ERROR_NO_MEM)    || \
                 ((ERROR) == HT6022_ERROR_OTHER))
#endif // HT6022BX_GLOBAL_H
