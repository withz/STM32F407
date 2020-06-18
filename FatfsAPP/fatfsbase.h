#ifndef FATFSBASE_H
#define FATFSBASE_H

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
int RAM_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff);
int MMC_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff);
int USB_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff);

#endif


