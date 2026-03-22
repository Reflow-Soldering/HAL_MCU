/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */


#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define	TRUE	1U
#define FALSE	0U

void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;



int main(void)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	char msg[100];


	HAL_Init();

	UART2_Init();


	memset(&osc_init,0,sizeof(osc_init));
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;


	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}
	//Clock Type = 사용하고자 하는 클럭들을 선택
	//SYSCLKSource = 공급되는 클럭이 어디에서 나오는 것인지 설정
	//AHBCLKDivider = 몇 배로 나누는지 -> 원하는 클럭에 따라
	//APB1/APB2CLKDivider 모두 같음
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_handler();
	}

	//내부 클럭 생성 중단
	__HAL_RCC_HSI_DISABLE();

	//SYSTICK 재설정
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	UART2_Init();

	//클럭 표시 UART2로
	memset(&msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(&msg,0,sizeof(msg));
	sprintf(msg,"HCLK : %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(&msg,0,sizeof(msg));
	sprintf(msg,"APB1CLK : %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(&msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	while(1);

	return 0;

}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if( HAL_UART_Init(&huart2) != HAL_OK)
	{
		//문제가 발생하면 들어옴
		Error_handler();
	}

}

void Error_handler(void)
{
	while(1);
}
