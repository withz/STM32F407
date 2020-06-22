#include "usb_app.h"

#include "string.h"

#include "usbd_desc.h"
#include "usbd_cdc_if.h"

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev; 

void OTG_FS_IRQHandler(void)
{   		    
	if (USB_OTG_IsHostMode(&USB_OTG_Core_dev)) //ȷ���ǲ���USB����ģʽ?
	{  
		USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);//USB�����ж�
	}
	else
	{
		USBD_OTG_ISR_Handler(&USB_OTG_Core_dev);//USB�Ի��ж�
	}											 
}  



void USBD_Configuration(void)
{
    USBD_Init(&USB_OTG_Core_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USBD_USR_cb);
}
