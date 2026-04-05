/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */
//#include "stm32f4xx_hal.h"	//가급적 main.h에 몰아두자
#include "main.h"


void SysTick_Handler(void)
{

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


