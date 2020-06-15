#ifndef __USART3_H__
#define __USART3_H__

#include "stm32f4xx.h"
#include "sys.h"

void USART3_Configuration(void);
void USART3_SendChar(char b);
void USART3_SendWords(char* c, int length);
void USART3_DMA_RecvStart(void);

extern void (*USART3_Recv)(unsigned char);
extern void (*USART3_RecvFinish)(unsigned int, unsigned char*);

#endif
