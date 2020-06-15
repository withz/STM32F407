#ifndef _KEYS_H_
#define _KEYS_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"


void Keys_Configuration(void);
u8 Keys_ReadKey(void);

#endif
