/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */

#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */

	// Low Level 초기화 진행
	// 1. 프로세서 우선순위 그룹화
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	// 2. 익셉션 인에이블

	// 3. 시스템 익셉션 우선순위 규정

	//
}
