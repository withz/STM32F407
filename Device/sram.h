#ifndef __SRAM_H
#define __SRAM_H															    


#include "stm32f4xx.h"

											  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n);


#endif

