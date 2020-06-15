#include "flash.h"

#define Flash_Buffer_Size	4096
u32 Flash_Buffer[Flash_Buffer_Size]={0};
const u32 Flash_BaseAddress=0X08000000;
const u32 Flash_Address=0x080E0000;

u32 Flash_GetSector(u32 Address);
u32 Flash_GetOffset(u32 Address);
u32 Flash_GetSectorStart(u32 sec);

void Flash_WriteWords(u16 p,u32* d,u16 len)
{
	STMFLASH_WriteLenByte(p*4+Flash_Address,d,len);
}

void Flash_WriteData(u16 p,u32 data)
{
	STMFLASH_WriteLenByte(p*4+Flash_Address,&data,1);
}

u32 Flash_ReadData(u16 p)
{
	return STMFLASH_Read(p*4+Flash_Address);
}



u32 STMFLASH_Read(u32 ReadAddr)
{
	return *(__IO u32*) ReadAddr;
}

void STMFLASH_WriteLenByte(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite)
{
	u16 i;
	u32 startSec = 0;
	u32 endSec = 0;
	u32 offset = 0;
	u32 startAddr = 0;
	
	if(WriteAddr<Flash_BaseAddress||WriteAddr>STM32_FLASH_SIZE*1024){
		return;
	}
	startSec=Flash_GetSector(WriteAddr);
	endSec=Flash_GetSector(NumToWrite*4+WriteAddr);
	if(startSec!=endSec){
		return ;
	}
	startAddr=Flash_GetSectorStart(startSec);
	offset=WriteAddr-startAddr;
	if(offset>Flash_Buffer_Size){
		return ;
	}
	
	
	for(i=0;i<Flash_Buffer_Size;i++){
		Flash_Buffer[i]=STMFLASH_Read(startAddr+i*4);
	}
	if(NumToWrite+offset>Flash_Buffer_Size){
		NumToWrite=Flash_Buffer_Size-offset;
	}
	for(i=0;i<NumToWrite;i++){
		Flash_Buffer[offset+i]=*pBuffer;
		pBuffer++;
	}
	
	FLASH_Unlock();
	for(i=startSec;i<=endSec;i+=8){
		FLASH_EraseSector(startSec,VoltageRange_3);
	}
	for(i=0;i<Flash_Buffer_Size;i++){
		FLASH_ProgramWord(WriteAddr+i*4, Flash_Buffer[i]);
	}
	FLASH_Lock();
}

u32 Flash_GetSector(u32 Address)
{
	u32 sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)){
		sector = FLASH_Sector_0;  
	}else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)){
		sector = FLASH_Sector_1;  
	}else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2)){
		sector = FLASH_Sector_2;  
	}else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3)){
		sector = FLASH_Sector_3;  
	}else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4)){
		sector = FLASH_Sector_4;  
	}else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5)){
		sector = FLASH_Sector_5;  
	}else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6)){
		sector = FLASH_Sector_6;  
	}else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7)){
		sector = FLASH_Sector_7;  
	}else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8)){
		sector = FLASH_Sector_8;  
	}else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9)){
		sector = FLASH_Sector_9;  
	}else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10)){
		sector = FLASH_Sector_10;  
	}else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11)){
		sector = FLASH_Sector_11;  
	}else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12)){
		sector = FLASH_Sector_12;  
	}else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13)){
		sector = FLASH_Sector_13;  
	}else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14)){
		sector = FLASH_Sector_14;  
	}else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15)){
		sector = FLASH_Sector_15;  
	}else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16)){
		sector = FLASH_Sector_16;  
	}else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17)){
		sector = FLASH_Sector_17;  
	}else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18)){
		sector = FLASH_Sector_18;  
	}else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19)){
		sector = FLASH_Sector_19;  
	}else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20)){
		sector = FLASH_Sector_20;  
	}else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21)){
		sector = FLASH_Sector_21;  
	}else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22)){
		sector = FLASH_Sector_22;  
	}else{
		sector = FLASH_Sector_23;
	}
	
	return sector;
}

u32 Flash_GetSectorStart(u32 sec)
{
	switch(sec){
		case FLASH_Sector_0: return ADDR_FLASH_SECTOR_0;
		case FLASH_Sector_1: return ADDR_FLASH_SECTOR_1;
		case FLASH_Sector_2: return ADDR_FLASH_SECTOR_2;
		case FLASH_Sector_3: return ADDR_FLASH_SECTOR_3;
		case FLASH_Sector_4: return ADDR_FLASH_SECTOR_4;
		case FLASH_Sector_5: return ADDR_FLASH_SECTOR_5;
		case FLASH_Sector_6: return ADDR_FLASH_SECTOR_6;
		case FLASH_Sector_7: return ADDR_FLASH_SECTOR_7;
		case FLASH_Sector_8: return ADDR_FLASH_SECTOR_8;
		case FLASH_Sector_9: return ADDR_FLASH_SECTOR_9;
		case FLASH_Sector_10: return ADDR_FLASH_SECTOR_10;
		case FLASH_Sector_11: return ADDR_FLASH_SECTOR_11;
		case FLASH_Sector_12: return ADDR_FLASH_SECTOR_12;
		case FLASH_Sector_13: return ADDR_FLASH_SECTOR_13;
		case FLASH_Sector_14: return ADDR_FLASH_SECTOR_14;
		case FLASH_Sector_15: return ADDR_FLASH_SECTOR_15;
		case FLASH_Sector_16: return ADDR_FLASH_SECTOR_16;
		case FLASH_Sector_17: return ADDR_FLASH_SECTOR_17;
		case FLASH_Sector_18: return ADDR_FLASH_SECTOR_18;
		case FLASH_Sector_19: return ADDR_FLASH_SECTOR_19;
		case FLASH_Sector_20: return ADDR_FLASH_SECTOR_20;
		case FLASH_Sector_21: return ADDR_FLASH_SECTOR_21;
		case FLASH_Sector_22: return ADDR_FLASH_SECTOR_22;
		case FLASH_Sector_23: return ADDR_FLASH_SECTOR_23;
	}
	return ADDR_FLASH_SECTOR_23;
}

