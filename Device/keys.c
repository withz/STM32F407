#include "keys.h"

void Keys_Configuration(void)
{
	GPIO_InitTypeDef gpio;
	
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOE,&gpio);
	
}

u8 Keys_ReadKey(void)
{
	u8 key=0;
	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)){
		key|=(1<<0);
	}
	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)){
		key|=(1<<1);
	}
	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)){
		key|=(1<<2);
	}
	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)){
		key|=(1<<3);
	}
	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)){
		key|=(1<<4);
	}
	return key;
}
