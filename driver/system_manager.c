#include "mcal_rcc.h"
#include "mcal_systick.h"
#include "system_manager.h"

void System_Init(){
	MCAL_RCC_InitSystemClock();
	MCAL_SysTick_Init(168000000);
}
