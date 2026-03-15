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
	SCB->SHCSR |= (0x7 << 16); //Usabe Fault, Memory Fault, Bus Fault System Exception 활성화

	// 3. 시스템 익셉션 우선순위 규정
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	//Systick은 HAL_Init에 있음
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	//저수준 초기화 진행. USART2 장치

	//2-1. UART를 위한 GPIO 변수 생성
	GPIO_InitTypeDef gpio_uart;

	//1. USART2 이용 위한 클럭 활성화 UART 장치와 GPIOA 활성화 필수임
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//2. 핀 MUX 하여 용도 설정
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;			//3번핀은 TX로 설정하기 위함 나머지 모두 같음 AF7마저도
	HAL_GPIO_Init(GPIOA, &gpio_uart);	//그리고 설정완료


	//3. IRQ 활설화 및 우선순위 설정(NVIC 설정)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

}
