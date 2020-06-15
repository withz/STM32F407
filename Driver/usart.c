#include "usart.h"

#define RX_BUFFER_SIZE1 32
#define RX_BUFFER_SIZE2 64
#define RX_BUFFER_SIZE3 32

char rx_buffer1[RX_BUFFER_SIZE1];
unsigned char rx_buffer1_pointer = 0;

char rx_buffer2[RX_BUFFER_SIZE2];
unsigned char rx_buffer2_pointer = 0;

char rx_buffer3[RX_BUFFER_SIZE3];		
unsigned char rx_buffer3_pointer = 0;


//-------------------------------USART1------------------------------------

void USART1_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_9;  
    gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

    USART_DeInit(USART1);
    usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No ;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART1,&usart);
    USART_Cmd(USART1,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

	DMA_DeInit(DMA2_Stream2);
	dma.DMA_Channel=DMA_Channel_4;
	dma.DMA_DIR=DMA_DIR_PeripheralToMemory;
	dma.DMA_FIFOMode=DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
	dma.DMA_Mode=DMA_Mode_Circular;
	dma.DMA_Priority=DMA_Priority_VeryHigh;
	
	dma.DMA_BufferSize=RX_BUFFER_SIZE1;
	dma.DMA_Memory0BaseAddr=(uint32_t)rx_buffer1;
	dma.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dma.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	
	dma.DMA_PeripheralBaseAddr=(uint32_t)&(USART1->DR);
	dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	dma.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2,&dma);
	DMA_Cmd(DMA2_Stream2,ENABLE);

    DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, DISABLE);
 
    nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    DMA_Cmd(DMA2_Stream2, ENABLE);
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

void USART1_SendWords(char* c)
{
	unsigned short wait = 0;
	while(*c)
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

void USART1_GetData(char* s)
{
	unsigned char i;
	for(i=0;i<RX_BUFFER_SIZE1;i++)
	{
		*s=rx_buffer1[i];
		s++;
	}
}

void USART1_GetWord(char* s)
{
	unsigned char i=0;
	while(rx_buffer1[rx_buffer1_pointer]!='\0')
	{
		if(rx_buffer1[rx_buffer1_pointer]!='.')
		{
			*s=rx_buffer1[rx_buffer1_pointer];
			*s=0;
			s++;i++;
			rx_buffer1_pointer++;
			if(i>RX_BUFFER_SIZE1-1)
				break;
		}
		else
		{
			*s=0;
			rx_buffer1_pointer++;
			if(rx_buffer1_pointer>RX_BUFFER_SIZE1-1)
			{
				rx_buffer1_pointer = 0;
			}
			break;
		}
		
		if(rx_buffer1_pointer>RX_BUFFER_SIZE1-1)
		{
			rx_buffer1_pointer = 0;
		}
	}
}


//--------------------USART2--------------------------------

void USART2_Configuration(void)		
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_2;  
    gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

    USART_DeInit(USART2);
    usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No ;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART2,&usart);
    USART_Cmd(USART2,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

	DMA_DeInit(DMA1_Stream5);
	dma.DMA_Channel=DMA_Channel_4;
	dma.DMA_DIR=DMA_DIR_PeripheralToMemory;
	dma.DMA_FIFOMode=DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
	dma.DMA_Mode=DMA_Mode_Circular;
	dma.DMA_Priority=DMA_Priority_VeryHigh;
	
	dma.DMA_BufferSize=RX_BUFFER_SIZE2;
	dma.DMA_Memory0BaseAddr=(uint32_t)rx_buffer2;   
	dma.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dma.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	
	dma.DMA_PeripheralBaseAddr= (uint32_t)&(USART2->DR);
	dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	dma.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5,&dma);
	DMA_Cmd(DMA1_Stream5,ENABLE);

    DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, DISABLE);
 
    nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    DMA_Cmd(DMA1_Stream5, ENABLE);
}

void USART2_SendChar(char b)
{
	unsigned short wait = 0;
	while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET)
	{
		wait++;
		if(wait>65530)
			return;
	}
	USART_SendData(USART2,b);
}

void USART2_SendWords(char* c)
{
	unsigned short wait = 0;
	while(*c)
	{
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET)
		{
			wait++;
			if(wait>65530)
				return;
		}
		USART_SendData(USART2,*c);
		c++;
	}
}

void USART2_GetData(char* s)
{
	unsigned char i;
	for(i=0;i<RX_BUFFER_SIZE2;i++)
	{
		*s=rx_buffer2[i];
		s++;
	}
}

void USART2_GetWord(char* s)
{
	unsigned char i=0;
	
	while(rx_buffer2[rx_buffer2_pointer]!='\0')
	{
		if(rx_buffer2[rx_buffer2_pointer]!='.')
		{
			*s=rx_buffer2[rx_buffer2_pointer];
			rx_buffer2[rx_buffer2_pointer]=0;
			s++;i++;
			rx_buffer2_pointer++;
			if(i>RX_BUFFER_SIZE2-1)
				break;
		}
		else
		{
			rx_buffer2[rx_buffer2_pointer]=0;
			rx_buffer2_pointer++;
			if(rx_buffer2_pointer>RX_BUFFER_SIZE2-1)
			{
				rx_buffer2_pointer = 0;
			}
			break;
		}
		
		if(rx_buffer2_pointer>RX_BUFFER_SIZE2-1)
		{
			rx_buffer2_pointer = 0;
		}
	}
}

//---------------------------------------USART3-------------------------------------------

void USART3_Configuration(void)	
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_10;  
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&gpio);

    USART_DeInit(USART3);
    usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No ;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART3,&usart);
    USART_Cmd(USART3,ENABLE);
    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);

	DMA_DeInit(DMA1_Stream1);
	dma.DMA_Channel=DMA_Channel_4;
	dma.DMA_DIR=DMA_DIR_PeripheralToMemory;
	dma.DMA_FIFOMode=DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
	dma.DMA_Mode=DMA_Mode_Circular;
	dma.DMA_Priority=DMA_Priority_VeryHigh;
	
	dma.DMA_BufferSize=RX_BUFFER_SIZE3;
	dma.DMA_Memory0BaseAddr= (uint32_t)rx_buffer3;   
	dma.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dma.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	
	dma.DMA_PeripheralBaseAddr=(uint32_t)&(USART3->DR);
	dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	dma.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream1,&dma);
	DMA_Cmd(DMA1_Stream1,ENABLE);

    DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, DISABLE);
 
    nvic.NVIC_IRQChannel = DMA1_Stream1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    DMA_Cmd(DMA1_Stream1, ENABLE);
}

void USART3_SendChar(char b)		//发送字符
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

void USART3_SendWords(char* c)		//发送字符串
{
	unsigned short wait = 0;
	while(*c)
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



void USART3_GetData(char* s)		//获取接收数据缓冲区字符
{
	unsigned char i;
	for(i=0;i<RX_BUFFER_SIZE3;i++)
	{
		*s=rx_buffer3[i];
		s++;
	}
}

void USART3_GetWord(char* s)		//获取接收数据缓冲区内容
{
	unsigned char i=0;
	while(rx_buffer3[rx_buffer3_pointer]!='\0')
	{
		if(rx_buffer3[rx_buffer3_pointer]!='.')
		{
			*s=rx_buffer3[rx_buffer3_pointer];
			rx_buffer3[rx_buffer3_pointer]=0;
			s++;i++;
			rx_buffer3_pointer++;
			if(i>RX_BUFFER_SIZE3-1)
				break;
		}
		else
		{
			rx_buffer3[rx_buffer3_pointer]=0;
			rx_buffer3_pointer++;
			if(rx_buffer3_pointer>RX_BUFFER_SIZE3-1)
			{
				rx_buffer3_pointer = 0;
			}
			break;
		}
		
		if(rx_buffer3_pointer>RX_BUFFER_SIZE3-1)
		{
			rx_buffer3_pointer = 0;
		}
	}
}



