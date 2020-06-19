#ifndef USB_APP_H
#define USB_APP_H

#include "string.h"
#include "stm32f4xx.h"

#include "usb_conf.h"
#include "usb_hcd_int.h"
#include "usb_dcd_int.h"

#include "usbh_usr.h"

#include "usbd_usr.h"
#include "usbd_desc.h"

extern USBH_HOST  USB_Host; 
extern USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;



#endif