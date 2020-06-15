/*
*********************************************************************************************************
*
*	模块名称 : TM7705 驱动模块(2通道带PGA的16位ADC)
*	文件名称 : bsp_tm7705.h
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TM7705_H
#define _BSP_TM7705_H

#include "stm32f4xx.h"
#include "sys.h"
#include "spi.h"
#include "delay.h"

void bsp_InitTM7705(void);
int16_t TM7705_ReadAdc(uint8_t _ch);
void TM7705_ReadAll(u32 *buf);
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
