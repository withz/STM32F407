#include "bstring.h"

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
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





