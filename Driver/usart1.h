#ifndef __USART1_H__
#define __USART1_H__

#include "stm32f4xx.h"
#include "sys.h"

void USART1_Configuration(void);
void USART1_SendChar(char b);
void USART1_SendWords(char* c, u32 len);
void USART1_GetData(char* s);
void USART1_GetWord(char* s);

extern void (*USART1_Recv)(u8);

#endif
