/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes the usb host library user callbacks
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
#include <string.h>
#include "usbh_usr.h"
#include "usb_app.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_MSC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR 
* @brief    This file includes the usb host stack user callbacks
* @{
*/

/** @defgroup USBH_USR_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Defines
* @{
*/
#define IMAGE_BUFFER_SIZE    512
/**
* @}
*/


/** @defgroup USBH_USR_Private_Macros
* @{
*/
extern USB_OTG_CORE_HANDLE USB_OTG_Core;
/**
* @}
*/


/** @defgroup USBH_USR_Private_Variables
* @{
*/


/* Points to the DEVICE_PROP structure of current device */
/* The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USBH_USR_cb = {
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages ---------------*/

/**
* @}
*/


/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/

/**
* @}
*/


/** @defgroup USBH_USR_Private_Functions
* @{
*/


/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
    USB_OTG_GINTMSK_TypeDef intmsk;
	intmsk.d32=USB_OTG_READ_REG32(&USB_OTG_Core_dev.regs.GREGS->GINTMSK);
	intmsk.b.usbsuspend=0;	//关闭挂起中断 
	USB_OTG_WRITE_REG32(&USB_OTG_Core_dev.regs.GREGS->GINTMSK,intmsk.d32); 

}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
  
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError(void)
{

}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Status
*/
void USBH_USR_DeviceDisconnected(void)
{


}

/**
* @brief  USBH_USR_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
		//printf("高速(HS)USB设备!\r\n");
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
		//printf("全速(FS)USB设备!\r\n"); 
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
		//printf("低速(LS)USB设备!\r\n");  
	}
	else
	{
		//printf("设备错误!\r\n");  
	}
}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
    USBH_DevDesc_TypeDef *hs;
	hs=DeviceDesc; 
}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
    
}


/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef * itfDesc,
                                          USBH_EpDesc_TypeDef * epDesc)
{
    USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
		//printf("可移动存储器设备!\r\n"); 
	}else if((*id).bInterfaceClass==0x03)
	{
		//printf("HID 设备!\r\n"); 
	}
}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  Manufacturer String 
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{

}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{

}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNum_String 
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{

}



/**
* @brief  EnumerationDone 
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{

}


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{

}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{

}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Status
*/
void USBH_USR_OverCurrentDetected(void)
{

}


/**
* @brief  USBH_USR_MSC_Application 
*         Demo application for mass storage
* @param  None
* @retval Status
*/
int USBH_USR_MSC_Application(void)
{

}



void delay(__IO uint32_t nCount)
{

}

/**
* @brief  Toggle_Leds
*         Toggle leds to shows user input state
* @param  None
* @retval None
*/
static void Toggle_Leds(void)
{

}

/**
* @brief  USBH_USR_DeInit
*         Deinit User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{

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
