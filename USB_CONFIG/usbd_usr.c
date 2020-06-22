/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "usbd_usr.h"
#include <stdio.h>

/** @addtogroup USBD_USER
  * @{
  */

/** @addtogroup USBD_MSC_DEMO_USER_CALLBACKS
  * @{
  */

/** @defgroup USBD_USR 
  * @brief    This file includes the user application layer
  * @{
  */

/** @defgroup USBD_USR_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_USR_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_USR_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_USR_Private_Variables
  * @{
  */
/* Points to the DEVICE_PROP structure of current device */
/* The purpose of this register is to speed up the execution */

USBD_Usr_cb_TypeDef USBD_USR_cb = {
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,

  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};

/**
  * @}
  */

/** @defgroup USBD_USR_Private_Constants
  * @{
  */

#define USER_INFORMATION1  "> Single Lun configuration.\n"
#define USER_INFORMATION2  "> microSD is used.\n"
/**
  * @}
  */



/** @defgroup USBD_USR_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_USR_Private_Functions
  * @{
  */

/**
* @brief  Displays the message on LCD on device lib initialization
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{


}

/**
* @brief  Displays the message on LCD on device reset event
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset(uint8_t speed)
{
    switch (speed)
	{
		case USB_OTG_SPEED_HIGH:
			//printf("USB Device Library v1.1.0  [HS]\r\n");
			break; 
		case USB_OTG_SPEED_FULL: 
			//printf("USB Device Library v1.1.0  [FS]\r\n");
			break;
		default:
			//printf("USB Device Library v1.1.0  [??]\r\n"); 
			break;
	}
}


/**
* @brief  Displays the message on LCD on device config event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConfigured(void)
{
    //usbx.bDeviceState|=0X80;
}

/**
* @brief  Displays the message on LCD on device suspend event 
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
    //usbx.bDeviceState=0;
}


/**
* @brief  Displays the message on LCD on device resume event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{

}


/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConnected(void)
{
    
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceDisconnected(void)
{
    //usbx.bDeviceState=0;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
