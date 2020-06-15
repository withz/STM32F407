#include "main.h"


int main(void)
{
    SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
    
	while(1)
	{
		
	}
}


