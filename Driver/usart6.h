#ifndef __USART6_H__
#define __USART6_H__

#include "stm32f4xx.h"
#include "sys.h"

void USART6_Configuration(void);
void USART6_SendChar(char b);
void USART6_SendWords(char* c, int length);

extern void (*USART6_Recv)(unsigned char*, unsigned short);

#endif
