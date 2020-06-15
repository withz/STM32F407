#include "bstring.h"

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

char *itoa(int value, char *string, int radix)
{
	int i, d;
	int flag = 0;
	char *ptr = string;

	if (radix != 10){
		*ptr = 0;
		return string;
	}
	if (!value){
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	if (value < 0){
		*ptr++ = '-';
		value *= -1;
	}

	for (i = 10000; i > 0; i /= 10){
		d = value / i;
		if (d || flag){
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	*ptr = 0;

	return string;
}
 

int atoi(const char *str)
{
	int s=0;
	unsigned char falg=0;
	
	while(*str==' '){
		str++;
	}
	if(*str=='-'||*str=='+'){
		if(*str=='-'){
			falg=1;
		}
		str++;
	}
 
	while(*str>='0'&&*str<='9'){
		s=s*10+*str-'0';
		str++;
		if(s<0){
			s=2147483647;
			break;
		}
	}
	return s*(falg?-1:1);
}





