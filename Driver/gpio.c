#include "gpio.h"

GPIO_TypeDef *MG_PORT_F(MG_PORT gpio);
uint16_t MG_PIN_F(MG_PIN pin);

void MG_Output(MG_PORT port, MG_PIN pin)
{
    GPIO_InitTypeDef gp;
    GPIO_TypeDef *gpio = MG_PORT_F(port);
    uint16_t gpin = MG_PIN_F(pin);
	
	gp.GPIO_Mode=GPIO_Mode_OUT;
	gp.GPIO_OType=GPIO_OType_PP;
	gp.GPIO_Speed=GPIO_Speed_100MHz;
	gp.GPIO_Pin=gpin;
	GPIO_Init(gpio,&gp);
}

void MG_Input(MG_PORT port, MG_PIN pin, s8 pull_s)
{
    GPIOPuPd_TypeDef pull;
    GPIO_InitTypeDef gp;
    GPIO_TypeDef *gpio = MG_PORT_F(port);
    uint16_t gpin = MG_PIN_F(pin);
    
    switch(pull_s){
        case 1: pull = GPIO_PuPd_UP;
            break;
        case 0: pull = GPIO_PuPd_DOWN;
            break;
        default: pull = GPIO_PuPd_NOPULL;
            break;
    }
	
	gp.GPIO_Mode=GPIO_Mode_IN;
    gp.GPIO_PuPd=pull;
	gp.GPIO_Speed=GPIO_Speed_100MHz;
	gp.GPIO_Pin=gpin;
	GPIO_Init(gpio,&gp);
}


void MG_Set(MG_PORT port, MG_PIN pin,u8 mode_s)
{
    GPIO_TypeDef *gpio = MG_PORT_F(port);
    uint16_t gpin = MG_PIN_F(pin);
	
	if(mode_s==0){
		GPIO_ResetBits(gpio, gpin);
	}else{
		GPIO_SetBits(gpio, gpin);
	}
}

void MG_Toggle(MG_PORT port, MG_PIN pin)
{
    GPIO_TypeDef *gpio = MG_PORT_F(port);
    uint16_t gpin = MG_PIN_F(pin);
	
	MG_Set(port, pin, !GPIO_ReadOutputDataBit(gpio, gpin));
}

u8 MG_Get(MG_PORT port, MG_PIN pin)
{
	GPIO_TypeDef *gpio = MG_PORT_F(port);
    uint16_t gpin = MG_PIN_F(pin);

	return GPIO_ReadInputDataBit(gpio, gpin);
}

GPIO_TypeDef *MG_PORT_F(MG_PORT gpio)
{
    switch(gpio){
        case MA: return GPIOA;
        case MB: return GPIOB;
        case MC: return GPIOC;
        case MD: return GPIOD;
        case ME: return GPIOE;
        case MF: return GPIOF;
        case MG: return GPIOG;
        case MH: return GPIOH;
        default: return GPIOG;
    }
}

uint16_t MG_PIN_F(MG_PIN pin)
{
    switch(pin){
        case M0: return GPIO_Pin_0;
        case M1: return GPIO_Pin_1;
        case M2: return GPIO_Pin_2;
        case M3: return GPIO_Pin_3;
        case M4: return GPIO_Pin_4;
        case M5: return GPIO_Pin_5;
        case M6: return GPIO_Pin_6;
        case M7: return GPIO_Pin_7;
        case M8: return GPIO_Pin_8;
        case M9: return GPIO_Pin_9;
        case M10: return GPIO_Pin_10;
        case M11: return GPIO_Pin_11;
        case M12: return GPIO_Pin_12;
        case M13: return GPIO_Pin_13;
        case M14: return GPIO_Pin_14;
        case M15: return GPIO_Pin_15;       
        default: return GPIO_Pin_0;
    }
}

void MG_RCC(MG_PORT port)
{
	switch(port)
	{
		case MA:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
			break;
		case MB:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
			break;
		case MC:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
			break;
		case MD:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
			break;
		case ME:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
			break;
		case MF:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
			break;
		case MG:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
			break;
        case MH:RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
			break;
	}
}

