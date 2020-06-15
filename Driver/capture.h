#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include "stm32f4xx.h"
#include "sys.h"

void Capture_Configuration(u8 count);

extern u32 capture_period[];
extern u32 capture_duty[];
#endif


