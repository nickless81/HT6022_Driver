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
#define HT6022BX_WRITE_ENDPOINT     0X02
#define HT6022BX_CONTROL_SIZE       6
/**************************************************/
/**/
#define HT6022BX_XXX_REQUEST_TYPE           0X40//64
#define HT6022BX_XXX_REQUEST                0XE8//232
#define HT6022BX_XXX_VALUE                  0X00
#define HT6022BX_XXX_INDEX                  0X00
#define HT6022BX_XXX_LENGTH                 0X01
#define HT6022BX_XXX_DATA                   0x01
#define HT6022BX_XXX_SIZE                   0X01
/**/
/***************************************************/
#define HT6022BX_IR1_REQUEST_TYPE           0X40//64
#define HT6022BX_IR1_REQUEST                0XE0//224
#define HT6022BX_IR1_VALUE                  0X00
#define HT6022BX_IR1_INDEX                  0X00
#define HT6022BX_IR1_SIZE                   0X01

#define HT6022BX_IR2_REQUEST_TYPE           0X40//64
#define HT6022BX_IR2_REQUEST                0XE1//225
#define HT6022BX_IR2_VALUE                  0X00
#define HT6022BX_IR2_INDEX                  0X00
#define HT6022BX_IR2_SIZE                   0X01

#define HT6022BX_SR_REQUEST_TYPE            0X40//64
#define HT6022BX_SR_REQUEST                 0XE2//226
#define HT6022BX_SR_VALUE                   0X00
#define HT6022BX_SR_INDEX                   0X00
#define HT6022BX_SR_SIZE                    0X01

#define HT6022BX_SETCALLEVEL_REQUEST_TYPE   0XC0//192
#define HT6022BX_SETCALLEVEL_REQUEST        0XA2//162
#define HT6022BX_SETCALLEVEL_VALUE          0X08
#define HT6022BX_SETCALLEVEL_INDEX          0X00

#define HT6022BX_GETCALLEVEL_REQUEST_TYPE   0X40//64
#define HT6022BX_GETCALLEVEL_REQUEST        0XA2//162
#define HT6022BX_GETCALLEVEL_VALUE          0X08
#define HT6022BX_GETCALLEVEL_INDEX          0X00

#define HT6022BX_READ_CONTROL_REQUEST_TYPE  0X40//64
#define HT6022BX_READ_CONTROL_REQUEST       0XE3//227
#define HT6022BX_READ_CONTROL_VALUE         0X00
#define HT6022BX_READ_CONTROL_INDEX         0X00
#define HT6022BX_READ_CONTROL_SIZE          0X01
#define HT6022BX_READ_CONTROL_DATA          0X01
#define HT6022BX_READ_BULK_PIPE             0X86//ENDPOINT
/*
 bRequest:  Decimal     Hexadecimal
            232
            162
            224
            225
            226
            227
 */

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
    libusb_device_handle    *DeviceHandle;
    unsigned char           Address;
}HT6022BX_DeviceTypeDef;
typedef struct
{
    QString         Name;
    int             FwSize;
    qint16          IdProduct;
    qint16          IdVendor;
    qint16          IdVendorFW;
    unsigned char*  Firmware;
    //libusb_device_handle *DeviceHandle;
    //unsigned char Address;
}HT6022BX_UsbDevice;
typedef struct
{
    unsigned int        index;
    QString             Name;
    int                 FwSize;
    qint16              IdProduct;
    qint16              IdVendor;
    qint16              IdVendorFW;
    unsigned char*      Firmware;
}HT6022BX_Info;

/**
  * @brief Error Code
  */
typedef enum
{
    HT6022BX_SUCCESS              =   0,//Firmware Installed Properly
    HT6022BX_FW_SUCCESS           =   1,//Device Connected with Firmware already Installed
    HT6022BX_DEVICE_CONNECTED     =   2,
    HT6022BX_ERROR_INVALID_PARAM  =  -2,
    HT6022BX_ERROR_ACCESS         =  -3,
    HT6022BX_ERROR_NO_DEVICE      =  -4,
    HT6022BX_ERROR_TIMEOUT        =  -7,
    HT6022BX_ERROR_NO_MEM         = -11,
    HT6022BX_ERROR_NO_DEVICELIST  = -12,
    HT6022BX_ERROR_OTHER          = -99
}HT6022BX_ErrorTypeDef;

#define  IS_HT6022BX_ERROR(ERROR) (((ERROR) == HT6022BX_SUCCESS) || \
                 ((ERROR) == HT6022BX_ERROR_INVALID_PARAM) || \
                 ((ERROR) == HT6022BX_ERROR_ACCESS )   || \
                 ((ERROR) == HT6022BX_ERROR_NO_DEVICE) || \
                 ((ERROR) == HT6022BX_ERROR_TIMEOUT)   || \
                 ((ERROR) == HT6022BX_ERROR_NO_MEM)    || \
                 ((ERROR) == HT6022BX_ERROR_OTHER))
/**
  * @brief  Size of data bufffer
  */
typedef enum
{
  HT6022BX_1KB   = 0x00000400,//0x04 /*!< 1024 Bytes */
  HT6022BX_2KB   = 0x00000800,//0x08 /*!< 2048 Bytes */
  HT6022BX_4KB   = 0x00001000,//0x04 /*!< 4096 Bytes */
  HT6022BX_8KB   = 0x00002000,//0x04 /*!< 8192 Bytes */
  HT6022BX_16KB  = 0x00004000,//0x04 /*!< 16384 Bytes */
  HT6022BX_32KB  = 0x00008000,//0x04 /*!< 32768 Bytes */
  HT6022BX_64KB  = 0x00010000,//0x04 /*!< 65536 Bytes */
  HT6022BX_128KB = 0x00020000,//0x04 /*!< 131072 Bytes */
  HT6022BX_256KB = 0x00040000,//0x04 /*!< 262144 Bytes */
  HT6022BX_512KB = 0x00080000,//0x04 /*!< 524288 Bytes */
  HT6022BX_1MB   = 0x00100000 //0x04 /*!< 1048576 Bytes */
}HT6022BX_DataSizeTypeDef;
#define  IS_HT6022BX_DATASIZE(SIZE) (((SIZE) == HT6022BX_1KB)  ||\
                                   ((SIZE) == HT6022BX_2KB)  ||\
                                   ((SIZE) == HT6022BX_4KB)  ||\
                                   ((SIZE) == HT6022BX_8KB)  ||\
                                   ((SIZE) == HT6022BX_16KB) ||\
                                   ((SIZE) == HT6022BX_32KB) ||\
                                   ((SIZE) == HT6022BX_64KB) ||\
                                   ((SIZE) == HT6022BX_128KB)||\
                                   ((SIZE) == HT6022BX_256KB)||\
                                   ((SIZE) == HT6022BX_512KB)||\
                                   ((SIZE) == HT6022BX_1MB))

/**
  * @brief Size of calibration values buffer
  */
typedef enum
{
  HT6022BX_32B   = 0x00000010, /*!< 32 Bytes */
  HT6022BX_64B   = 0x00000020, /*!< 64 Bytes */
  HT6022BX_128B  = 0x00000080 /*!< 128 Bytes */
}HT6022BX_CVSizeTypeDef;
#define  IS_HT6022BX_CVSIZE(SIZE)   (((SIZE) == HT6022BX_32B)  ||\
                                    ((SIZE) == HT6022BX_64B)  ||\
                                    ((SIZE) == HT6022BX_128B))

/**
  * @brief  Sample rate
  */
typedef enum
{
    HT6022BX_48MSa  = 0x30, /*!< 24MSa per channel */
    HT6022BX_16MSa  = 0x10, /*!< 16MSa per channel */
    HT6022BX_8MSa   = 0x08, /*!< 8MSa per channel */
    HT6022BX_4MSa   = 0x04, /*!< 4MSa per channel */
    HT6022BX_1MSa   = 0x01, /*!< 1MSa per channel */
    HT6022BX_500KSa = 0x32, /*!< 500KSa per channel */
    HT6022BX_200KSa = 0x14, /*!< 200KSa per channel */
    HT6022BX_100KSa = 0x0A  /*!< 100KSa per channel */
}HT6022BX_SRTypeDef;
#define  IS_HT6022BX_SR(SR) (((SR) == HT6022BX_48MSa)  ||\
                            ((SR) == HT6022BX_16MSa)  ||\
                            ((SR) == HT6022BX_8MSa)   ||\
                            ((SR) == HT6022BX_4MSa)   ||\
                            ((SR) == HT6022BX_1MSa)   ||\
                            ((SR) == HT6022BX_500KSa) ||\
                            ((SR) == HT6022BX_200KSa) ||\
                            ((SR) == HT6022BX_100KSa))

/**
  * @brief Input range
  */
typedef enum
{
    HT6022BX_10V   = 0x01, /*!< -5V    to 5V    */
    HT6022BX_5V    = 0x02, /*!< -2.5V  to 2.5V  */
    HT6022BX_2V    = 0x05, /*!< -1V    to 1V    */
    HT6022BX_1V    = 0x0A  /*!< -500mv to 500mv */
    //5V/DIV
    //2V/DIV
    //1V/DIV
    //500mV/DIV
    //200mV/DIV
    //100mV/DIV
    //50mV/DIV
    //20mV/DIV
}HT6022BX_IRTypeDef;
#define  IS_HT6022BX_IR(IR) (((IR) == HT6022BX_10V) ||\
                            ((IR) == HT6022BX_5V)  ||\
                            ((IR) == HT6022BX_2V)  ||\
                            ((IR) == HT6022BX_1V))

#endif // HT6022BX_GLOBAL_H
