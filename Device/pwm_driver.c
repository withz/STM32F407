#include "pwm_driver.h"

#define PWM_Driver_ADDR  0x80

Mi2c pwm_driver={
	.highFirst=1,
	.time=0,
	.scl_gpio=MC,
	.sda_gpio=MC,
	.scl_pin=M5,
	.sda_pin=M4 
};

void PWM_Driver_Configuration(void)
{
	MI2C_Init(&pwm_driver);
	PWM_Driver_SetFreq(200);
}

void PWM_Driver_Write(u8 reg, u8 cmd)
{
	MI2C_Write_Reg(&pwm_driver,PWM_Driver_ADDR>>1,reg,cmd);
}

u8 PWM_Driver_Read(u8 reg)
{
	return MI2C_Read_Reg(&pwm_driver,PWM_Driver_ADDR>>1,reg);
}

void PWM_Driver_Reset(void)
{
	u8 re=0;
	MI2C_Start(&pwm_driver);
	
	MI2C_Write(&pwm_driver,0);
	re+=MI2C_GetAck(&pwm_driver);
	
	MI2C_Write(&pwm_driver,0x06);
	re+=MI2C_GetAck(&pwm_driver);
	
	MI2C_End(&pwm_driver);
}

void PWM_Driver_SetFreq(u16 freq)
{
	uint32_t prescale;
	u8 oldmode=0;
	u8 newmode;
	double prescaleval;
	//freq *= 0.92;  // Correct for overshoot in the frequency setting 
	prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;
	prescale = floor(prescaleval + 0.5);

	PWM_Driver_Write(PCA9685_MODE1, 0);
	PWM_Driver_Reset();
	oldmode = PWM_Driver_Read(PCA9685_MODE1);
	newmode = (oldmode&0x7F) | 0x1F; // sleep
	PWM_Driver_Write(PCA9685_MODE1, newmode); // go to sleep
	PWM_Driver_Write(PCA9685_PRESCALE, prescale); // set the prescaler
	PWM_Driver_Write(PCA9685_MODE1, oldmode);
	delayms(2);
	PWM_Driver_Write(PCA9685_MODE1, 0xa1);
}

void PWM_Driver_SetHigh(u8 n, u32 on, u32 off)
{
	PWM_Driver_Write(PCA9685_PWM_BASE_ON_L+PCA9685_PWM_STEP*n,on);
	PWM_Driver_Write(PCA9685_PWM_BASE_ON_H+PCA9685_PWM_STEP*n,on>>8);
	PWM_Driver_Write(PCA9685_PWM_BASE_OFF_L+PCA9685_PWM_STEP*n,off);
	PWM_Driver_Write(PCA9685_PWM_BASE_OFF_H+PCA9685_PWM_STEP*n,off>>8);
}
//pwm = 0 -> 4095
void PWM_Driver_SetPwm(u8 n, u16 pwm)
{
	PWM_Driver_SetHigh(n, 0, pwm);
}

void PWM_Driver_WriteAll(u16* pwm, u8 n)
{
	u8 i;
	u8 write_buf[64];
	
	for(i=0;i<n;i++){
		write_buf[4*i+0]=0;
		write_buf[4*i+1]=0;
		write_buf[4*i+2]=pwm[i];
		write_buf[4*i+3]=pwm[i]>>8;
	}
	
	MI2C_Write_Len(&pwm_driver,PWM_Driver_ADDR>>1,PCA9685_PWM_BASE,n*4,write_buf);
}
