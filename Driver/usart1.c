#include "usart1.h"

void USART1_Recv_Callback(u8 data);
void (*USART1_Recv)(u8) = USART1_Recv_Callback;

void USART1_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_9;  
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	USART_DeInit(USART1);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART1,&usart);
	
	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
}

void USART1_SendChar(char b)
{
	unsigned short wait = 0;
    while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET)
	{
		wait++;
		if(wait>65530)
			return;
	}
	USART_SendData(USART1,b);
}

void USART1_SendWords(char* c, u32 len)
{
	unsigned short wait = 0;
	while(len--)
	{
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET)
		{
			wait++;
			if(wait>65530)
				return;
		}
		USART_SendData(USART1,*c);
		c++;
	}
}

void USART1_Recv_Callback(u8 data){}

void USART1_IRQHandler(void)
{
	u8 rec=0;
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!=RESET){
		rec=USART_ReceiveData(USART1);
		(*USART1_Recv)(rec);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
