#include "usart3.h"

void USART3_Recv_Callback(u8 data);
void USART3_Recv_CallbackFrame(u32 len, u8 *data);

void (*USART3_Recv)(u8) = USART3_Recv_Callback;
void (*USART3_RecvFinish)(u32 len, u8*) = USART3_Recv_CallbackFrame;

#define USART3_RECV_DMA_BUFFER_SIZE 	512

u8 USART3_DMARecv_Buffer[USART3_RECV_DMA_BUFFER_SIZE];
u8 USART3_DMARecv_ShadowBuffer[USART3_RECV_DMA_BUFFER_SIZE];

void USART3_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;
	DMA_InitTypeDef   dma;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_8;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz; 
	GPIO_Init(GPIOD,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_9;  
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz; 
	GPIO_Init(GPIOD,&gpio);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);

	USART_DeInit(USART3);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART3,&usart);

	nvic.NVIC_IRQChannel = USART3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 3;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	DMA_Cmd(DMA1_Stream1,DISABLE);
	DMA_DeInit(DMA1_Stream1);
	dma.DMA_Channel=DMA_Channel_4;
	dma.DMA_Memory0BaseAddr=(u32)USART3_DMARecv_Buffer;
	dma.DMA_PeripheralBaseAddr=(u32)(&(USART3->DR));
	dma.DMA_BufferSize=USART3_RECV_DMA_BUFFER_SIZE;
	dma.DMA_DIR=DMA_DIR_PeripheralToMemory;
	dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dma.DMA_Mode=DMA_Mode_Normal;
	dma.DMA_Priority=DMA_Priority_Low;
	dma.DMA_FIFOMode=DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
	dma.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA1_Stream1,&dma);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Stream1,DISABLE);

	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART3,ENABLE);
}

void USART3_SendChar(char b)
{
  unsigned short wait = 0;
	while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET)
	{
		wait++;
		if(wait>65530)
			return;
	}
	USART_SendData(USART3,b);
}

void USART3_SendWords(char* c, int length)
{
	unsigned short wait = 0;
	while(length--)
	{
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET)
		{
			wait++;
			if(wait>65530)
				return;
		}
		USART_SendData(USART3,*c);
		c++;
	}
}

void USART3_Recv_Callback(u8 data){}

void USART3_Recv_CallbackFrame(u32 len, u8 *data){}

void USART3_DMA_RecvStart(void)
{
	DMA_Cmd(DMA1_Stream1,ENABLE);
}

void USART3_IRQHandler(void)
{
	u8 rec=0;
	u32 i;
	s32 len=0;
	if(USART_GetFlagStatus(USART3,USART_FLAG_IDLE)!=RESET){
		DMA_Cmd(DMA1_Stream1,DISABLE);
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);
		len = USART3_RECV_DMA_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Stream1);
		if(len<0){
			len=0;
		}
		DMA_SetCurrDataCounter(DMA1_Stream1, USART3_RECV_DMA_BUFFER_SIZE);
		for(i=0;i<len;i++){
			USART3_DMARecv_ShadowBuffer[i] = USART3_DMARecv_Buffer[i];
		}
		DMA_Cmd(DMA1_Stream1,ENABLE);
		
		(*USART3_RecvFinish)(len, USART3_DMARecv_ShadowBuffer);
		for(i=0;i<len;i++){
			USART3_DMARecv_ShadowBuffer[i] = 0;
		}
		
		rec=USART_ReceiveData(USART3);
		rec=rec;
		USART_ClearITPendingBit(USART3,USART_IT_IDLE);
	}
}
