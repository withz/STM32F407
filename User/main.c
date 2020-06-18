#include "main.h"

#include "fatfsapp.h"
#include "sram.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
    FSMC_SRAM_Init();
    
    storage_init();
    storage_open("test.txt");
    
    
	while(1)
	{
		delay_ms(200);
        storage_write();
	}
}


