/*
 * msp.c
 *
 *  Created on: 2026/05/03
 *      Author: Yu
 */


#include "main.h"

/**
  * @brief  MSP 초기화
  * @retval None
  */
void HAL_MspInit(void)
{
  //Here will do low level processor specific inits.
  //1. Set up the priority grouping of the arm cortex mx processor
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  //2. Enable the required system exceptions of the arm cortex mx processor
  SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exceptions

  //3. configure the priority for the system exceptions
  HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
  HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
  HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

/**
  * @brief  Initializes the TIM Input Capture MSP.
  * @param  htim TIM Input Capture handle
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2ch1_gpio;
	//1. 주변장치로 쓰이는 버스 클럭 활성화 진행
	//GPIOA도 설정하는것과 마찬가지이므로 클럭 버스 활성화
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. 타이머 2 채널 1의 속성 부여
	//0번핀의 AF모드 -> AF모드는 1번 사용
	tim2ch1_gpio.Pin = GPIO_PIN_0;
	tim2ch1_gpio.Mode = GPIO_MODE_AF_PP;
	tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA,&tim2ch1_gpio);

	//3. NVIC 세팅
	//우선순위 설정
	//IRQ 활성화
	HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim TIM Base handle
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
  //1. enable the clock for the TIM6 peripheral
  __HAL_RCC_TIM6_CLK_ENABLE();

  //2. Enable the IRQ of TIM6
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

  //3. setup the priority for TIM6_DAC_IRQn
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);
}

/**
  * @brief  UART MSP Init.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_uart;
  //here we are going to do the low level inits. of the USART2 peripheral

  //1. enable the clock for the USART2 peripheral as well as for GPIOA peripheral
  __HAL_RCC_USART2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  //2 . Do the pin muxing configurations
  gpio_uart.Pin  = GPIO_PIN_2;
  gpio_uart.Mode = GPIO_MODE_AF_PP;
  gpio_uart.Pull = GPIO_PULLUP;
  gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
  gpio_uart.Alternate =  GPIO_AF7_USART2; //UART2_TX
  HAL_GPIO_Init(GPIOA,&gpio_uart);

  gpio_uart.Pin = GPIO_PIN_3; //UART2_RX
  HAL_GPIO_Init(GPIOA,&gpio_uart);
  //3 . Enable the IRQ and set up the priority (NVIC settings )
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  HAL_NVIC_SetPriority(USART2_IRQn,15,0);
}

