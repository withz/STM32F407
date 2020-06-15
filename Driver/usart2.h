#ifndef __USART2_H__
#define __USART2_H__

#include "stm32f4xx.h"
#include "sys.h"

void USART2_Configuration(void);
void USART2_DMASendWords(u8 *c, int len);

extern void (*USART2_Recv)(unsigned char);

#endif
