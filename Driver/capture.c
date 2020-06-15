#include "capture.h"

u32 capture_period[8];
u32 capture_duty[8];

void Capture_Configuration(u8 count)
{	 
	u8 bits[8];
	const u16 tim_prescaler = 840 - 1;

	GPIO_InitTypeDef 					GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  				TIM_ICInitStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;
	
	bits[0]=count&0x01;
	bits[1]=count&0x02;
	bits[2]=count&0x04;
	bits[3]=count&0x08;
	bits[4]=count&0x10;
	bits[5]=count&0x20;
	bits[6]=count&0x40;
	bits[7]=count&0x80;

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	
	TIM_TimeBaseStructure.TIM_Period = 40000-1;
	TIM_TimeBaseStructure.TIM_Prescaler =  tim_prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;	
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	
	
	if(bits[0]!=0){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOE,GPIO_Pin_9);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  (tim_prescaler+1)*2-1;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
		TIM_PWMIConfig(TIM1, &TIM_ICInitStructure); 
		
		TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);      
		TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		TIM_Cmd(TIM1, ENABLE);
	}
	if(bits[1]!=0){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  tim_prescaler;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
		TIM_PWMIConfig(TIM2, &TIM_ICInitStructure); 
		
		TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);      
		TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		TIM_Cmd(TIM2, ENABLE);
	}
	if(bits[2]!=0){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  tim_prescaler;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
		TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
		
		TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
		TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM_Cmd(TIM3, ENABLE);
	}
	if(bits[3]!=0){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  tim_prescaler;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);
		
		TIM_SelectInputTrigger(TIM4, TIM_TS_TI1FP1);      
		TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		TIM_Cmd(TIM4, ENABLE);
	}
	if(bits[4]!=0){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  tim_prescaler;
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_PWMIConfig(TIM5, &TIM_ICInitStructure);
		
		TIM_SelectInputTrigger(TIM5, TIM_TS_TI1FP1);      
		TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM5, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
		TIM_Cmd(TIM5, ENABLE);
	}
	if(bits[5]!=0){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  (tim_prescaler+1)*2-1;
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_PWMIConfig(TIM8, &TIM_ICInitStructure);
		
		TIM_SelectInputTrigger(TIM8, TIM_TS_TI1FP1);      
		TIM_SelectSlaveMode(TIM8, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
		TIM_Cmd(TIM8, ENABLE);
	}
	if(bits[6]!=0){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  tim_prescaler;
		TIM_TimeBaseStructure.TIM_Prescaler =  (tim_prescaler+1)*2-1;
		TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_PWMIConfig(TIM9, &TIM_ICInitStructure);
		
		TIM_SelectInputTrigger(TIM9, TIM_TS_TI1FP1);      
		TIM_SelectSlaveMode(TIM9, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM9, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM9, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
		TIM_Cmd(TIM9, ENABLE);
	}
	if(bits[7]!=0){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12);
		
		TIM_TimeBaseStructure.TIM_Prescaler =  tim_prescaler;
		TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_PWMIConfig(TIM12, &TIM_ICInitStructure);
		
		TIM_SelectInputTrigger(TIM12, TIM_TS_TI1FP1);      
		TIM_SelectSlaveMode(TIM12, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM12, TIM_MasterSlaveMode_Enable);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		TIM_ITConfig(TIM12, TIM_IT_CC1, ENABLE); 
		TIM_ClearITPendingBit(TIM12, TIM_IT_CC1);
		TIM_Cmd(TIM12, ENABLE);
	}
}

void TIM1_CC_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		capture_period[0] = TIM_GetCapture1(TIM1); 
		cap = TIM_GetCapture2(TIM1);
		if(cap>90 && cap < 210){
			capture_duty[0] = TIM_GetCapture2(TIM1);
		}
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1); 
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC2|TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{		
		capture_period[1] = TIM_GetCapture1(TIM2); 
		cap = TIM_GetCapture2(TIM2);
		if(cap>90 && cap < 210){
			capture_duty[1] = TIM_GetCapture2(TIM2);
		}
		
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1); 
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);
}


void TIM3_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{		
		capture_period[2] = TIM_GetCapture1(TIM3); 
		cap = TIM_GetCapture2(TIM3);
		if(cap>90 && cap < 210){
			capture_duty[2] = TIM_GetCapture2(TIM3);
		}
		
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update);
}

void TIM4_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{		
		capture_period[3] = TIM_GetCapture1(TIM4); 
		cap = TIM_GetCapture2(TIM4);
		if(cap>90 && cap < 210){
			capture_duty[3] = TIM_GetCapture2(TIM4);
		}
		
	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update);
}

void TIM5_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
	{		
		capture_period[4] = TIM_GetCapture1(TIM5); 
		cap = TIM_GetCapture2(TIM5);
		if(cap>90 && cap < 210){
			capture_duty[4] = TIM_GetCapture2(TIM5);
		}
		
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); 
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update);
}


void TIM8_CC_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)
	{		
		capture_period[5] = TIM_GetCapture1(TIM8); 
		cap = TIM_GetCapture2(TIM8);
		if(cap>90 && cap < 210){
			capture_duty[5] = TIM_GetCapture2(TIM8);
		}
		
	}
	TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM8, TIM_IT_CC1|TIM_IT_Update);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET)
	{		
		capture_period[6] = TIM_GetCapture1(TIM9); 
		cap = TIM_GetCapture2(TIM9);
		if(cap>90 && cap < 210){
			capture_duty[6] = TIM_GetCapture2(TIM9);
		}
		
	}
	TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM9, TIM_IT_CC1|TIM_IT_Update);
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
	u32 cap = 0;
	if (TIM_GetITStatus(TIM12, TIM_IT_CC1) != RESET)
	{		
		capture_period[7] = TIM_GetCapture1(TIM12); 
		cap = TIM_GetCapture2(TIM12);
		if(cap>90 && cap < 210){
			capture_duty[7] = TIM_GetCapture2(TIM12);
		}
		
	}
	TIM_ClearITPendingBit(TIM12, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM12, TIM_IT_CC1|TIM_IT_Update);
}
