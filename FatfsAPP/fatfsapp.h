#ifndef FATFSAPP_H
#define FATFSAPP_H

#include "stm32f4xx.h"
#include "ff.h"	
#include "diskio.h"

DWORD get_fattime(void);
int RAM_disk_status(void);
int MMC_disk_status(void);
int USB_disk_status(void);
int RAM_disk_initialize(void);
int MMC_disk_initialize(void);
int USB_disk_initialize(void);
int RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count);
int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count);
int USB_disk_write(const BYTE *buff, LBA_t sector, UINT count);
int RAM_disk_read(BYTE *buff, LBA_t sector, UINT count);
int MMC_disk_read(BYTE *buff, LBA_t sector, UINT count);
int USB_disk_read(BYTE *buff, LBA_t sector, UINT count);

#endif
