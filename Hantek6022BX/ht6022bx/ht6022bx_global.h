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
#define HT6022_FW_VENDOR    0X04B4
#define HT6022BE_MODEL      0X6022
#define HT6022BL_MODEL      0X602A
typedef struct
{
    QString         Name;
    int             FwSize;
    int             IdProduct;
    int             IdVendor;
    unsigned char*  Firmware;
}HT6022BX_Info;

/**
  * @brief Error Code
  */
typedef enum
{
     HT6022_SUCCESS  = 0,
     HT6022_ERROR_INVALID_PARAM = -2,
     HT6022_ERROR_ACCESS        = -3,
     HT6022_ERROR_NO_DEVICE     = -4,
     HT6022_ERROR_TIMEOUT       = -7,
     HT6022_ERROR_NO_MEM        = -11,
     HT6022_ERROR_NO_DEVICELIST = -12,
     HT6022_ERROR_OTHER         = -99
}HT6022_ErrorTypeDef;

#define  IS_HT6022_ERROR(ERROR) (((ERROR) == HT6022_SUCCESS) || \
                 ((ERROR) == HT6022_ERROR_INVALID_PARAM) || \
                 ((ERROR) == HT6022_ERROR_ACCESS )   || \
                 ((ERROR) == HT6022_ERROR_NO_DEVICE) || \
                 ((ERROR) == HT6022_ERROR_TIMEOUT)   || \
                 ((ERROR) == HT6022_ERROR_NO_MEM)    || \
                 ((ERROR) == HT6022_ERROR_OTHER))
#endif // HT6022BX_GLOBAL_H
