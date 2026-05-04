/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */

#include "main.h"

void HAL_MspInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */

	// Low Level 초기화 진행
	// 1. 프로세서 우선순위 그룹화
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	// 2. 익셉션 인에이블
	SCB->SHCSR |= (0x7 << 16); //Usabe Fault, Memory Fault, Bus Fault System Exception 활성화

	// 3. 시스템 익셉션 우선순위 규정
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	//Systick은 HAL_Init에 있음
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	//1. 타이머 10 클럭 활성화
	__HAL_RCC_TIM10_CLK_ENABLE();

	//2. 타이머 10의 인터럽트를 활성화
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	//3. 인터럽트 우선순위 설정
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 15, 0);
}
