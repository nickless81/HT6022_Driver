/**
  ******************************************************************************
  * @file    HT6022BLfw.c
  * @author  Jose Pablo Castro Valverde
  * @email   josepablo.castro@gmail.com
  * @version V1.0
  * @date    10-10-2015
  * @brief   This file provides the HT6022BL firmware         
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HT6022BLFW_H
#define __HT6022BLFW_H

#ifdef __cplusplus
 extern "C" {
#endif



/** @addtogroup HT6022_Driver
  * @{
  */
/* Private define ------------------------------------------------------------*/ 
#define HT6022BL_FIRMWARE_SIZE          470
#define HT6022BL_FIRMWARE_VENDOR_ID     0X04B4
#define HT6022BL_FIRMWARE_PRODUCT_ID    0X602A
#define HT6022BL_FIRMWARE_REQUEST_TYPE  0X40
#define HT6022BL_FIRMWARE_REQUEST       0XA0
#define HT6022BL_FIRMWARE_INDEX         0X00

/*Exported variables---------------------------------------------------------*/
extern unsigned char HT6022BL_Firmware[];

/**
  * @}
  */ 

#ifdef __cplusplus
    }
#endif

#endif

/****************************END OF FILE*********************************/
