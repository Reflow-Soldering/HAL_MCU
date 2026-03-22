/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */


#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>

#define	TRUE	1U
#define FALSE	0U

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
uint8_t convert_to_capital(uint8_t data);

UART_HandleTypeDef huart2;



char *user_data = "The application is running\n\r";

uint8_t data_buffer[100];
uint8_t recvd_data;
uint32_t count = 0;
uint8_t reception_complete = FALSE;
int main(void)
{
	// First Init
	HAL_Init();

	SystemClockConfig();

	UART2_Init();

	uint16_t len_size = strlen(user_data);
	HAL_UART_Transmit(&huart2, (uint8_t *)user_data, len_size, HAL_MAX_DELAY);


	while(reception_complete != TRUE)
	{
		HAL_UART_Receive_IT(&huart2, &recvd_data, 1);
	}
	while(1);

	return 0;

}

void SystemClockConfig(void)
{


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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(recvd_data == '\r')
	{
		reception_complete = TRUE;
		data_buffer[count++] = '\r';
		HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);
	}
	else
	{
		data_buffer[count++] = recvd_data;
	}
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
