#include "usb_app.h"

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


