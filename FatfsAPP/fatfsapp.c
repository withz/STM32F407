#include "fatfsapp.h"

DWORD get_fattime(void)
{
    return 0;
}

int RAM_disk_status(void)
{
    return 0;
}

int MMC_disk_status(void)
{
    return 0;
}

int USB_disk_status(void)
{
    return 0;
}

int RAM_disk_initialize(void)
{
    return 0;
}

int MMC_disk_initialize(void)
{
    return 0;
}

int USB_disk_initialize(void)
{
    return 0;
}

int RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    return 0;  
}

int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    return 0; 
}

int USB_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    return 0; 
}

int RAM_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
    return 0; 
}

int MMC_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
    return 0; 
}

int USB_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
    return 0; 
}

