#include "fatfsbase.h"
#include "stdlib.h"
#include "sram.h"


#include "usart1.h"

__IO uint32_t *ram_disk = (__IO uint32_t *) (Bank1_SRAM4_ADDR);
typedef struct{ uint8_t pRead[512];} *kankan_t;
kankan_t kankan;

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
    u32 start=sector*512;
    u32 i;
    
    kankan = (kankan_t)ram_disk;
    kankan->pRead[0] = kankan->pRead[0];
    
    for(i=start;i<count;i++){
        ram_disk[i]=buff[i];
    }
    USART1_SendWords((char*)buff, count);
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
    u32 start=sector*512;
    u32 i;
    
    for(i=start;i<count;i++){
        buff[i]=ram_disk[i];
    }
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

int RAM_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT back = RES_OK;
    switch(cmd){
        case CTRL_SYNC:
            break;
        case GET_SECTOR_COUNT: *(u32*)buff = 2048;
            break;
        case GET_SECTOR_SIZE: *(u16*)buff = 512;
            break;
        case GET_BLOCK_SIZE: *(u32*)buff = 512;
            break;
        case CTRL_TRIM: //*(u32*)buff = 4096;
            break;
        default:
            break;
    }
    return back; 
}

int MMC_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT back = RES_OK;
    return back; 
}

int USB_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT back = RES_OK;
    return back; 
}

