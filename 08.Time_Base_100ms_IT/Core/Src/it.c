/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */

#include "main.h"
extern TIM_HandleTypeDef htimer10;

void SysTick_Handler(void)
{

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer10);
}
