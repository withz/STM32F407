#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h"

void delay_init(void);
void delay_ms(u32 n);
void delay_us(u32 n);
void delayms(u32 n);
void delayus(u32 n);


#endif

