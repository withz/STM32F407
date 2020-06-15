#include "ad.h"

unsigned short ADC_ConvertedValue[128];

void ADC_Configuration(void)
{
    GPIO_InitTypeDef         gpio;
    ADC_InitTypeDef          adc;
    DMA_InitTypeDef          dma;
    ADC_CommonInitTypeDef com;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA| RCC_AHB1Periph_GPIOB, ENABLE); 
  
    gpio.GPIO_Mode = GPIO_Mode_AN;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);
    gpio.GPIO_Pin = GPIO_Pin_0;//-----
	GPIO_Init(GPIOA,&gpio);
    gpio.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA,&gpio);	
//	gpio.GPIO_Pin = GPIO_Pin_4;
//	GPIO_Init(GPIOA,&gpio);
//	gpio.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB,&gpio);
	gpio.GPIO_Pin = GPIO_Pin_1;//-----
	GPIO_Init(GPIOB,&gpio);
	
    DMA_DeInit(DMA2_Stream4);
	dma.DMA_Channel=DMA_Channel_0;
	dma.DMA_DIR=DMA_DIR_PeripheralToMemory;
	dma.DMA_FIFOMode=DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
	dma.DMA_Mode=DMA_Mode_Circular;
	dma.DMA_Priority=DMA_Priority_High;
	
	dma.DMA_BufferSize=96;
	dma.DMA_Memory0BaseAddr=(u32)&ADC_ConvertedValue;
	dma.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	dma.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dma.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	
	dma.DMA_PeripheralBaseAddr=(u32)(&ADC1->DR);
	dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	dma.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream4,&dma);
	DMA_Cmd(DMA2_Stream4,ENABLE);
	
	ADC_DeInit();
	com.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	com.ADC_Mode=ADC_Mode_Independent;
	com.ADC_Prescaler=ADC_Prescaler_Div2;
	com.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&com);
	
	adc.ADC_NbrOfConversion=6;
	adc.ADC_ContinuousConvMode=ENABLE;
	adc.ADC_DataAlign=ADC_DataAlign_Right;
	adc.ADC_ExternalTrigConv=ADC_ExternalTrigConvEdge_None;
	adc.ADC_Resolution=ADC_Resolution_12b;
	adc.ADC_ScanConvMode=ENABLE;
    ADC_Init(ADC1, &adc);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_3Cycles);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_28Cycles);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SampleTime_28Cycles);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
}

int ADC_Filter(unsigned char x)
{
    int sum = 0;
    int adc_i = 0;
    
	if(x<1||x>8)
		return 0;
	
    for(adc_i=0+x-1;adc_i<128;)
    { 
        sum += ADC_ConvertedValue[adc_i];
		adc_i+=8;
    }
    sum >>=4;
    
    return sum;
}
