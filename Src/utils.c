#include "config.h"

unsigned int g_system_tick = 0;

unsigned int retTickDiff(unsigned int tick)
{
	return get_system_tick()-tick;
}

void SysTick_Handler(void)
{
	g_system_tick++;
}

unsigned int get_system_tick(void)
{
	return g_system_tick;
}

void reset(void)
{
	
	debug_out1(":reset...\r\n", 10);
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);
	IWDG_ReloadCounter();
	IWDG_Enable();
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	while(1) {
	}	
}



