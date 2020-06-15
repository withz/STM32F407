#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "mi2c.h"
#include "arm_math.h"

#define PCA9685_MODE1 0x00
#define PCA9685_MODE2 0x01

#define PCA9685_PWM_BASE  		 0X06
#define PCA9685_PWM_BASE_ON_L  0X06
#define PCA9685_PWM_BASE_ON_H  0X07
#define PCA9685_PWM_BASE_OFF_L 0X08
#define PCA9685_PWM_BASE_OFF_H 0X09
#define PCA9685_PWM_STEP 4

#define PCA9685_PRESCALE				0XFE

void PWM_Driver_Configuration(void);
void PWM_Driver_SetFreq(u16 freq);
void PWM_Driver_SetHigh(u8 n, u32 on, u32 off);
void PWM_Driver_SetPwm(u8 n, u16 pwm);
u8 PWM_Driver_Read(u8 reg);
void PWM_Driver_Write(u8 reg, u8 cmd);
void PWM_Driver_WriteAll(u16* pwm, u8 n);

#endif


