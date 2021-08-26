#include "stm32l4xx_hal.h"
#include "boardef.h"
#include <string.h>

static TIM_HandleTypeDef Delay_htim;

void Delay_Init(void){

	/* EANBLE RCC FOR DELAY TIMER */

	if(strcmp(DELAY_TIM_STRING, "TIM6") == 0){  //// ustawić tak aby można było użyć każdego timera
		__HAL_RCC_TIM6_CLK_ENABLE();
	}

	HAL_Delay(1);


	/* INIT DELAY TIMER */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	Delay_htim.Instance = DELAY_TIM;
	Delay_htim.Init.Prescaler = DELAY_TIM_APB_FREQ-1;
	Delay_htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	Delay_htim.Init.Period = 0xffff-1;
	Delay_htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&Delay_htim) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&Delay_htim, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/* START DELAY TIMER */

	HAL_TIM_Base_Start(&Delay_htim);

}

void Delay_Us(uint16_t us){

	__HAL_TIM_SET_COUNTER(&Delay_htim, 0);
	while(__HAL_TIM_GET_COUNTER(&Delay_htim) < us);

}
