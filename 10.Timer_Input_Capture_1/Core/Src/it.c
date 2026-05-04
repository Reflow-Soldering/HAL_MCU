/*
 * it.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */

#include <main.h>

extern TIM_HandleTypeDef htimer2;
extern TIM_HandleTypeDef htimer6;

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

/**
  * @brief 해당 함수는 TIM2의 인터럽트 핸들음
  */
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);
}

void TIM6_DAC_IRQHandler(void)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  HAL_TIM_IRQHandler(&htimer6);
}
