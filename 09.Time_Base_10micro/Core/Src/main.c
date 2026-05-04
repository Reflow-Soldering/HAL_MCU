/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */


#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>

void SystemClockConfig(void);

void Error_handler(void);

void Timer10_Init(void);

void GPIO_Init(void);

TIM_HandleTypeDef htimer10;



int main(void)
{
	// First Init
	HAL_Init();

	SystemClockConfig();

	//LED chrlghk
	GPIO_Init();

	//여기에는 Basic Timer가 없으므로 General 10 타이머 사용
	//Timer10은 APB2에 달려있음
	Timer10_Init();

	//타이머 시작하기->인터럽트방식임
	HAL_TIM_Base_Start_IT(&htimer10);


	while(1);

	return 0;

}

void SystemClockConfig(void)
{

}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef Led_Gpio;
	Led_Gpio.Mode = GPIO_MODE_OUTPUT_PP;
	Led_Gpio.Pin = GPIO_PIN_5;
	Led_Gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &Led_Gpio);
}

void Timer10_Init(void)
{

	//10usec임
	//타이머 몇번 쓸거임?
	htimer10.Instance = TIM10;

	//초기화 할 타이머 프리스케일 입력
	htimer10.Init.Prescaler = 0;

	//초기화 할 피리어드(주기)
	htimer10.Init.Period = 159;

	//초기화 진행
	if(HAL_TIM_Base_Init(&htimer10) != HAL_OK)
	{
		Error_handler();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}


void Error_handler(void)
{
	while(1);
}

uint8_t convert_to_capital(uint8_t data)
{
	if(data >= 'a' && data<= 'z')
	{
		data = data - ('a' - 'A');
	}
	else
	{
		data = data;
	}
	return data;
}
