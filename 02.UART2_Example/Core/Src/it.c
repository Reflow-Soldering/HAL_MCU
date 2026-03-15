/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */


void SysTick_Handler(void)
{

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
