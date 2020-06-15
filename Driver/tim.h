#ifndef TIM_H
#define TIM_H


#include "stm32f4xx.h"

extern const unsigned char IntTime;

void TIM2_Configuration(void);

void TIM3_Configuration(void);
void OneSecTask(void);
void TIM2_Start(void);
void TIM3_Start(void);

#endif 
