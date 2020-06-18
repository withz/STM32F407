#ifndef USART2_H
#define USART2_H

#include "stm32f4xx.h"

void USART2_Configuration(void);
void USART2_DMASendWords(u8 *c, int len);

extern void (*USART2_Recv)(unsigned char);

#endif
