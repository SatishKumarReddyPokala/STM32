/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;

volatile int flag = 0;

/* Private functions ---------------------------------------------------------*/
	
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
	if(flag)
	{
		if(EXTI_GetITStatus(EXTI_Line0))
		{
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
	}
	else
	{
		if(EXTI_GetITStatus(EXTI_Line0))
		{
			flag =1;
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
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
  while (1)
  {
	  while(flag)
	  {
		  GPIO_ToggleBits(GPIOD,0XF000);
		  delay(1000);
		  flag = 0;
	  }
  }
}

