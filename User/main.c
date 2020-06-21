#include "main.h"


int main(void)
{
//    u32 i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
    FSMC_SRAM_Init();

	MG_RCC(MF);
	MG_Output(MF, M9);
	MG_Output(MF, M10);
	MG_Set(MF, M9, 1);
	MG_Set(MF, M10, 1);

    USBD_Configuration();
    
    
	while(1)
	{
		delay_ms(200);
	}
}


