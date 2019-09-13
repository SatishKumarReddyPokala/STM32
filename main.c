/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"


/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

volatile int flag = 0;
	
void EXTI_CONFIG(void)
{
	//Enable clocks for GPIO and Sysconfig
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitTypeDef my_GPIO;
	EXTI_InitTypeDef my_EXTI;
	NVIC_InitTypeDef my_NVIC;
	
	my_GPIO.GPIO_Mode = GPIO_Mode_IN;
	my_GPIO.GPIO_OType = GPIO_OType_PP;
	my_GPIO.GPIO_Pin = GPIO_Pin_0;
	my_GPIO.GPIO_PuPd = GPIO_PuPd_DOWN;
	my_GPIO.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &my_GPIO);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	my_EXTI.EXTI_Line = EXTI_Line0;
	my_EXTI.EXTI_LineCmd = ENABLE;
	my_EXTI.EXTI_Mode = EXTI_Mode_Interrupt;
	my_EXTI.EXTI_Trigger = EXTI_Trigger_Rising;
	
	EXTI_Init(&my_EXTI);
	
	my_NVIC.NVIC_IRQChannel = EXTI0_IRQn;
	my_NVIC.NVIC_IRQChannelCmd = ENABLE;
	my_NVIC.NVIC_IRQChannelPreemptionPriority = 0x00;
	my_NVIC.NVIC_IRQChannelSubPriority = 0x00;
	
	NVIC_Init(&my_NVIC);
}

void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0)){
//		Delay(100);
		flag =1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	
	EXTI_CONFIG();
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef my_gpiod;
	
	my_gpiod.GPIO_Mode = GPIO_Mode_OUT;
	my_gpiod.GPIO_OType = GPIO_OType_PP;
	my_gpiod.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	my_gpiod.GPIO_PuPd = GPIO_PuPd_NOPULL;
	my_gpiod.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOD, &my_gpiod);
	
  /* Infinite loop */
  while (1){
		while(flag){
		GPIO_ToggleBits(GPIOD,0XF000);
			flag = 0;
		}
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

	
  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
