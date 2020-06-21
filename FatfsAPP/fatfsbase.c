#include "fatfsbase.h"




SD_CardInfo card_info;
SD_CardStatus card_status;

DWORD get_fattime(void)
{
    return 0;
}

int RAM_disk_initialize(void)
{
    return 0;
}

int MMC_disk_initialize(void)
{
	SD_Error Status = SD_OK;
	Status = SD_Init();
	if(Status != SD_OK){
		return -1;
	}
	Status = SD_GetCardInfo(&card_info);
	if(Status != SD_OK){
		return -1;
	}
	Status = SD_GetCardStatus(&card_status);
	if(Status != SD_OK){
		return -1;
	}
    return 0;
}

int USB_disk_initialize(void)
{
    return 0;
}

int RAM_disk_status(void)
{
    return 0;
}

int MMC_disk_status(void)
{
	SDCardState Status = SD_CARD_READY;
	Status = SD_GetState();
	if(Status == SD_CARD_ERROR){
		return -1;
	}
    return 0;
}

int USB_disk_status(void)
{
    return 0;
}



int RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    u32 i;
    for(i=0;i<count;i++){
        FSMC_SRAM_WriteBuffer((u8*)buff, (sector+i)*512, 512);
    }
    return 0;  
}

int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    SD_Error Status = SD_OK;
    Status = SD_WriteMultiBlocks(
        (u8*)buff, 
        sector * card_info.CardBlockSize, 
        card_info.CardBlockSize, 
        count
    );
    if(Status == SD_OK){
        Status = SD_WaitWriteOperation();
        while(SD_GetStatus() != SD_TRANSFER_OK);
    }
    if(Status != SD_OK){
        return -1;
    }
    return 0; 
}

int USB_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    return 0; 
}

int RAM_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
    u32 i;
    for(i=0;i<count;i++){
        FSMC_SRAM_ReadBuffer(buff, (sector+i)*512, 512);
    }
    return 0; 
}

int MMC_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
    SD_Error Status = SD_OK;
    Status = SD_ReadMultiBlocks(
        buff, 
        sector * card_info.CardBlockSize, 
        card_info.CardBlockSize, 
        count
    );
    if(Status == SD_OK){
        Status = SD_WaitReadOperation();
        while(SD_GetStatus() != SD_TRANSFER_OK);
    }
    if(Status != SD_OK){
        return -1;
    }
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
    switch(cmd){
        case CTRL_SYNC:
            break;
        case GET_SECTOR_COUNT: *(u32*)buff = card_info.CardCapacity/card_info.CardBlockSize;
            break;
        case GET_SECTOR_SIZE: *(u16*)buff = 512;
            break;
        case GET_BLOCK_SIZE: *(u32*)buff = card_info.CardBlockSize;
            break;
        case CTRL_TRIM: //*(u32*)buff = 4096;
            break;
        default:
            break;
    }
    return back; 
}

int USB_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT back = RES_OK;
    return back; 
}

