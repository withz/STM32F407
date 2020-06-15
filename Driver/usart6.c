#include "usart6.h"

void USART6_Recv_Callback(u8* data, u16 len);
void (*USART6_Recv)(u8*,u16) = USART6_Recv_Callback;
u8 USART6_Recv_Buf[256];
u16 USART6_Recv_Pointer=0;

void USART6_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz; 
	GPIO_Init(GPIOC,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_14;  
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz; 
	GPIO_Init(GPIOG,&gpio);
	
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);

	USART_DeInit(USART6);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART6,&usart);
	

	nvic.NVIC_IRQChannel = USART6_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART6,ENABLE);
}

void USART6_SendChar(char b)
{
  unsigned short wait = 0;
	while( USART_GetFlagStatus(USART6,USART_FLAG_TXE) == RESET)
	{
		wait++;
		if(wait>65530)
			return;
	}
	USART_SendData(USART6,b);
}

void USART6_SendWords(char* c, int length)
{
	unsigned short wait = 0;
	while(length--)
	{
		while( USART_GetFlagStatus(USART6,USART_FLAG_TXE) == RESET)
		{
			wait++;
			if(wait>65530)
				return;
		}
		USART_SendData(USART6,*c);
		c++;
	}
}

void USART6_Recv_Callback(u8* data, u16 len){}

void USART6_IRQHandler(void)
{
	u8 rec=0;
	u16 len=0;
 	if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET){
		rec=USART_ReceiveData(USART6);
		USART6_Recv_Buf[USART6_Recv_Pointer++]=rec;
		USART6_Recv_Pointer=USART6_Recv_Pointer%128;
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET){
		rec=USART_ReceiveData(USART6);
		len=USART6_Recv_Pointer;
		USART6_Recv_Pointer=0;
		(*USART6_Recv)(USART6_Recv_Buf, len);
		USART_ClearITPendingBit(USART6,USART_IT_IDLE);
	}
	
}
