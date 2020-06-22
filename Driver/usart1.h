#ifndef USART1_H
#define USART1_H

#include "stm32f4xx.h"


void USART1_Configuration(void);
void USART1_SendChar(char b);
void USART1_SendWords(char* c, u32 len);
void USART1_GetData(char* s);
void USART1_GetWord(char* s);

extern void (*USART1_Recv)(u8);

#endif
