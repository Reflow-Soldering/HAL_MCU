/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */


#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void)


int main(void)
{
	// First Init
	HAL_Init();

	SystemClockConfig();

	return 0;

}

void SystemClockConfig(void)
{


}
