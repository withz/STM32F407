#include "bsp_ad7606.h"

#define RESET_0()		GPIO_ResetBits(GPIOG,GPIO_Pin_8);
#define RESET_1()		GPIO_SetBits(GPIOG,GPIO_Pin_8);

#define AD1BZ_R()		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3);
#define AD2BZ_R()		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4);
#define AD1CS_0()		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
#define AD1CS_1()		GPIO_SetBits(GPIOF,GPIO_Pin_1);
#define AD2CS_0()		GPIO_ResetBits(GPIOF,GPIO_Pin_2);
#define AD2CS_1()		GPIO_SetBits(GPIOF,GPIO_Pin_2);
#define ADSCK_0()		GPIO_ResetBits(GPIOF,GPIO_Pin_5);
#define ADSCK_1()		GPIO_SetBits(GPIOF,GPIO_Pin_5);
#define CONVST_0()	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
#define CONVST_1()	GPIO_SetBits(GPIOF,GPIO_Pin_0);

void AD7606_Configuration(void)
{
	GPIO_InitTypeDef gpio;

	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5;
	GPIO_Init(GPIOF,&gpio);
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_Init(GPIOG,&gpio);
	
	gpio.GPIO_Mode=GPIO_Mode_IN;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
  gpio.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOF,&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 \
	 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOG,&gpio);

	GPIO_SetBits(GPIOG,GPIO_Pin_15);
	GPIO_ResetBits(GPIOG, GPIO_Pin_13);
	
	GPIO_ResetBits(GPIOG, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
//	GPIO_SetBits(GPIOG, GPIO_Pin_10 | GPIO_Pin_11);

	AD7606_Reset();
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_Reset
*	����˵��: Ӳ����λAD7606����λ֮��ָ�����������״̬��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	RESET_0();	/* �˳���λ״̬ */

	RESET_1();	/* ���븴λ״̬ */
	RESET_1();	/* �������ӳ١� RESET��λ�ߵ�ƽ��������С50ns�� */
	RESET_1();
	RESET_1();

	RESET_0();	/* �˳���λ״̬ */
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_StartConvst
*	����˵��: ����1��ADCת��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_StartConvst(void)
{
	/* page 7��  CONVST �ߵ�ƽ�����Ⱥ͵͵�ƽ��������� 25ns */
	/* CONVSTƽʱΪ�� */
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_0();
	CONVST_1();
}

u8 AD7606_RESULT(void)
{
	u8 result=0;
	ADSCK_0();
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0)<<0);
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1)<<1);
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)<<2);
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3)<<3);
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4)<<4);
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5)<<5);
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6)<<6);
	result |= (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)<<7);
	ADSCK_1();
	return result;
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_ReadNowAdc
*	����˵��: ��ȡ8·�������������洢��ȫ�ֱ��� g_tAD7606
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_ReadNowAdc(s16 * buf)
{
	u8 i;
	u8 temp;
	u16 data[20];
	
	AD7606_StartConvst();
	delayus(10);
	
	AD1CS_0();
	for(i=0;i<8;i++){
		temp = AD7606_RESULT();
		data[i] = AD7606_RESULT();
		data[i] = data[i]<<8;
		data[i] = data[i] + temp;
	}
	AD1CS_1();
	
	AD2CS_0();
	for(i=8;i<16;i++){
		temp = AD7606_RESULT();
		data[i] = AD7606_RESULT();
		data[i] = data[i]<<8;
		data[i] = data[i] + temp;
	}
	AD2CS_1();
	
	for(i=0;i<16;i++){
		buf[i]=data[i];
	}
}


