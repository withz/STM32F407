#include "usb_app.h"

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev; 

void OTG_FS_IRQHandler(void)
{   		    
	if (USB_OTG_IsHostMode(&USB_OTG_Core_dev)) //确定是不是USB主机模式?
	{  
		USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);//USB主机中断
	}
	else
	{
		USBD_OTG_ISR_Handler(&USB_OTG_Core_dev);//USB丛机中断
	}											 
}  


