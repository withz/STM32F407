#include "dac.h"

void DAC_Configuration(void)
{
	GPIO_InitTypeDef g;
	DAC_InitTypeDef dac;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	g.GPIO_Mode=GPIO_Mode_AN;
	g.GPIO_OType=GPIO_OType_PP;
	g.GPIO_PuPd=GPIO_PuPd_DOWN;
	g.GPIO_Speed=GPIO_Speed_100MHz;
	g.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOA,&g);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	dac.DAC_Trigger=DAC_Trigger_None;
	dac.DAC_WaveGeneration=DAC_WaveGeneration_None;
	dac.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
	dac.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_Init(DAC_Channel_1,&dac);
	DAC_Init(DAC_Channel_2,&dac);
	
	DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_Cmd(DAC_Channel_2,ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R,0);
	DAC_SetChannel2Data(DAC_Align_12b_R,0);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
}

void DAC_SetValue1(u32 x)
{
	DAC_SetChannel1Data(DAC_Align_12b_R,x);
}

void DAC_SetValue2(u32 x)
{
	DAC_SetChannel2Data(DAC_Align_12b_R,x);
}

