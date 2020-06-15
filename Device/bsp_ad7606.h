/*
*********************************************************************************************************
*
*	模块名称 : AD7606数据采集模块
*	文件名称 : bsp_ad7606.h
*	版    本 : V1.0
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_AD7606_H
#define _BSP_AD7606_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

void AD7606_Configuration(void);
void AD7606_Reset(void);
void AD7606_StartConvst(void);
void AD7606_ReadNowAdc(s16 * buf);


#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
