#include "usart2.h"

#define USART2_DMASend_Buffer_Len		256

void USART2_Recv_Callback(u8 data);
void (*USART2_Recv)(u8) = USART2_Recv_Callback;
u8 USART2_DMASend_Buffer[USART2_DMASend_Buffer_Len];

void USART2_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;
	DMA_InitTypeDef   dma;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz; 
	GPIO_Init(GPIOD,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_6;  
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz; 
	GPIO_Init(GPIOD,&gpio);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);

	USART_DeInit(USART2);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART2,&usart);
	
	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	DMA_Cmd(DMA1_Stream6,DISABLE);
	DMA_DeInit(DMA1_Stream6);
	dma.DMA_Channel=DMA_Channel_4;
	dma.DMA_Memory0BaseAddr=(u32)USART2_DMASend_Buffer;
	dma.DMA_PeripheralBaseAddr=(u32)(&(USART2->DR));
	dma.DMA_BufferSize=USART2_DMASend_Buffer_Len;
	dma.DMA_DIR=DMA_DIR_MemoryToPeripheral;
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
	
	nvic.NVIC_IRQChannel=DMA1_Stream6_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=1;
	nvic.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&nvic);
	
	DMA_Init(DMA1_Stream6,&dma);
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	DMA_Cmd(DMA1_Stream6,DISABLE);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
}

void USART2_DMASendWords(u8 *c, int len)
{
	u32 i;
	while(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET);
	for(i=0;i<len;i++){
		USART2_DMASend_Buffer[i]=c[i];
	}
	DMA_SetCurrDataCounter(DMA1_Stream6,len);
	DMA_Cmd(DMA1_Stream6,ENABLE);
}

void USART2_Recv_Callback(u8 data){}

void USART2_IRQHandler(void)
{
	u8 rec=0;
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!=RESET){
		rec=USART_ReceiveData(USART2);
		(*USART2_Recv)(rec);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}

void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6)!=RESET){
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
		DMA_Cmd(DMA1_Stream6,DISABLE);
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
	}
}
