/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */
//#include "stm32f4xx_hal.h"	//가급적 main.h에 몰아두자
#include "main.h"
extern UART_HandleTypeDef huart2;

void SysTick_Handler(void)
{

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
