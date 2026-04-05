/*
 * main.c
 *
 *  Created on: Mar 10, 2026
 *      Author: zmffl
 */


#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define	TRUE	1U
#define FALSE	0U

#define	SYS_CLOCK_FREQ_100MHZ	100
#define	SYS_CLOCK_FREQ_84MHZ	84
#define	SYS_CLOCK_FREQ_50MHZ	50


void UART2_Init(void);
void Error_handler(void);
void SystemClock_Config(uint8_t Clock_Freq);
void SystemClock_Config_HSE(uint8_t Clock_Freq);

UART_HandleTypeDef huart2;



int main(void)
{


	/*
	 * 과제
	 * 1. PLL을 이용한 HCLK 어플리케이션 제작
	 * 2. PLL 소스를 HSI와 HSE를 각각 이용하여 시도해봄
	 * 3. 50Mhz, 80MHz, 120MHz 세 가지를 하지만 411은 100MHz가 최대이므로 100MHz로 한다.
	 *
	 */
	HAL_Init();

	SystemClock_Config_HSE(SYS_CLOCK_FREQ_100MHZ);

	UART2_Init();

	char msg[100];

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
	sprintf(msg,"APB2CLK : %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	while(1);

	return 0;

}
void SystemClock_Config(uint8_t Clock_Freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	osc_init.OscillatorType			=	RCC_OSCILLATORTYPE_HSI;	//내부클럭임
	osc_init.HSIState				=	RCC_HSI_ON;				//내부클럭 온
	osc_init.HSICalibrationValue	=	RCC_HSICALIBRATION_DEFAULT;	//16이 기본값 16써도 됨
	//PLL관련 중요 설정
	// PLLR, Q, N, M, P 이렇게 있다.
	// 시스템 클럭에 들어가는 클럭은 PLLM, PLLN, PLLP 세가지이다.
	// PLL Q 는 DSIO, USB등에만 쓰이는 형태임 기본값은 4
	// PLL R 은 I2S, SAI, SYSTEM and SPDIF-Rx Clock등에 쓰임 기본값은 2
	// PLL HSE를 이용한 50MHZ PLLM 8, PLLN 100 PLLP 2
	// PLL HSI를 이용한 50MHZ PLLM 16, PLLN 100 PLLP 2 -> HSI 먼저한다.

	osc_init.PLL.PLLState			=	RCC_PLL_ON;
	osc_init.PLL.PLLSource			=	RCC_PLLSOURCE_HSI;
	//시스템 주파수마다 레이턴시 다름 주의할 것
	uint32_t Flatency	= 0;
	switch(Clock_Freq)
	{
		case SYS_CLOCK_FREQ_50MHZ :
			//50MHz
			osc_init.PLL.PLLM		=	16;
			osc_init.PLL.PLLN		=	100;
			osc_init.PLL.PLLP		=	2;
			clk_init.ClockType				=	RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			=	RCC_SYSCLK_DIV2;
			clk_init.APB1CLKDivider			= 	RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider			=	RCC_HCLK_DIV2;
			Flatency						=	FLASH_ACR_LATENCY_2WS;
			break;

		case SYS_CLOCK_FREQ_84MHZ :
			osc_init.PLL.PLLM		=	16;
			osc_init.PLL.PLLN		=	168;
			osc_init.PLL.PLLP		=	2;
			clk_init.ClockType				=	RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			=	RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= 	RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider			=	RCC_HCLK_DIV2;
			Flatency						=	FLASH_ACR_LATENCY_3WS;
			break;

		case SYS_CLOCK_FREQ_100MHZ :
			osc_init.PLL.PLLM		=	16;
			osc_init.PLL.PLLN		=	200;
			osc_init.PLL.PLLP		=	2;
			clk_init.ClockType				=	RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			=	RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= 	RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider			=	RCC_HCLK_DIV2;
			Flatency						=	FLASH_ACR_LATENCY_3WS;
			break;

		default :
			return;
//			break;
	}


	//구조체 주의
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	//구조체 주의
	if(HAL_RCC_ClockConfig(&clk_init, Flatency) != HAL_OK)
	{
		Error_handler();
	}


	//systick 설정
	//1ms로 설정하는 것
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	//systick 소스를 어떻게 할 것인지?
	//1로 나누거나 8로 나누는거 뿐이 없음
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void SystemClock_Config_HSE(uint8_t Clock_Freq)
{

	// 여기에는 크리스탈이 달려있지 않으므로 코딩만 하고 패스해야함
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	osc_init.OscillatorType			=	RCC_OSCILLATORTYPE_HSE;	//외부클럭임
	osc_init.HSEState				=	RCC_HSE_BYPASS;				//외부클럭 온

	//PLL관련 중요 설정
	// PLLR, Q, N, M, P 이렇게 있다.
	// 시스템 클럭에 들어가는 클럭은 PLLM, PLLN, PLLP 세가지이다.
	// PLL Q 는 DSIO, USB등에만 쓰이는 형태임 기본값은 4
	// PLL R 은 I2S, SAI, SYSTEM and SPDIF-Rx Clock등에 쓰임 기본값은 2
	// PLL HSE를 이용한 50MHZ PLLM 8, PLLN 100 PLLP 2
	// PLL HSI를 이용한 50MHZ PLLM 16, PLLN 100 PLLP 2 -> HSI 먼저한다.
	// 발생하는 문제점 1 -> 클럭 주파수가 높을수록 Power Scale이 달라져야한다.
	osc_init.PLL.PLLState			=	RCC_PLL_ON;
	osc_init.PLL.PLLSource			=	RCC_PLLSOURCE_HSE;

	//시스템 주파수마다 레이턴시 다름 주의할 것
	uint32_t Flatency	= 0;
	switch(Clock_Freq)
	{
	//PLLM 만 바꾸면 끝!
		case SYS_CLOCK_FREQ_50MHZ :
			//50MHz
			__HAL_RCC_PWR_CLK_ENABLE();
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
			osc_init.PLL.PLLM		=	8;
			osc_init.PLL.PLLN		=	100;
			osc_init.PLL.PLLP		=	2;
			clk_init.ClockType				=	RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			=	RCC_SYSCLK_DIV2;
			clk_init.APB1CLKDivider			= 	RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider			=	RCC_HCLK_DIV2;
			Flatency						=	FLASH_ACR_LATENCY_2WS;
			break;

		case SYS_CLOCK_FREQ_84MHZ :
			//411re는 84MHz까지는 Power Scale이 2다
			__HAL_RCC_PWR_CLK_ENABLE();
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
			osc_init.PLL.PLLM		=	8;
			osc_init.PLL.PLLN		=	168;
			osc_init.PLL.PLLP		=	2;
			clk_init.ClockType				=	RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			=	RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= 	RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider			=	RCC_HCLK_DIV2;
			Flatency						=	FLASH_ACR_LATENCY_3WS;
			break;

		case SYS_CLOCK_FREQ_100MHZ :
			//100MHz는 반드시 Power Scale을 높여주라 -> 1로 최대치니까
			__HAL_RCC_PWR_CLK_ENABLE();
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

			//446re에는 overdrive까지 enable해야하는 표가 있다. 411re는 없으므로 무관하다.
			osc_init.PLL.PLLM		=	8;
			osc_init.PLL.PLLN		=	200;
			osc_init.PLL.PLLP		=	2;
			clk_init.ClockType				=	RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			=	RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= 	RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider			=	RCC_HCLK_DIV2;
			Flatency						=	FLASH_ACR_LATENCY_3WS;
			break;

		default :
			return;
//			break;
	}


	//구조체 주의
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	//구조체 주의
	if(HAL_RCC_ClockConfig(&clk_init, Flatency) != HAL_OK)
	{
		Error_handler();
	}


	//systick 설정
	//1ms로 설정하는 것
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	//systick 소스를 어떻게 할 것인지?
	//1로 나누거나 8로 나누는거 뿐이 없음
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

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
