#ifndef RNG_H
#define RNG_H

#include "stm32f4xx.h"


u8 RNG_Init(void);
u32 RNG_Get_RandomNum(void);
int RNG_Get_RandomRange(int min,int max);
#endif
