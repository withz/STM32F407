/*
*********************************************************************************************************
*
*	ģ������ : AD7606���ݲɼ�ģ��
*	�ļ����� : bsp_ad7606.h
*	��    �� : V1.0
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
