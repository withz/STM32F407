/*
*********************************************************************************************************
*
*	ģ������ : TM7705 ����ģ��(2ͨ����PGA��16λADC)
*	�ļ����� : bsp_tm7705.h
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
