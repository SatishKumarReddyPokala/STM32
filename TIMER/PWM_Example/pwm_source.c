#include "main.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private functions ---------------------------------------------------------*/

void delay(int milli)
{
	int i,j;
	for(i=0;i<milli;i++)
	{
		j=1;
		while(j>=0)
		{
			j--;
		}
	}
}

void timer_fun(int Duty_cycle)
{
	
	GPIO_InitTypeDef my_GPIOInit;
	TIM_TimeBaseInitTypeDef my_TimeBaseInit;
	TIM_OCInitTypeDef my_OCInit;

	//Enable clock
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	//GPIO Configurations
	
	my_GPIOInit.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	my_GPIOInit.GPIO_Mode = GPIO_Mode_AF;
	my_GPIOInit.GPIO_PuPd = GPIO_PuPd_UP;
	my_GPIOInit.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOD, &my_GPIOInit);
	
	//Alternate Function
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	
	//Timer Base Initialisation
	
	my_TimeBaseInit.TIM_Period = 8399;
	my_TimeBaseInit.TIM_Prescaler = 0;
	my_TimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	my_TimeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
	
	//Common TIM settings
	my_OCInit.TIM_OCMode = TIM_OCMode_PWM2;
	my_OCInit.TIM_OutputState = TIM_OutputState_Enable;
	my_OCInit.TIM_OCPolarity = TIM_OCPolarity_Low;
	my_OCInit.TIM_Pulse = Duty_cycle;
	
	TIM_TimeBaseInit(TIM4, &my_TimeBaseInit);
	TIM_OC1Init(TIM4, &my_OCInit);
	TIM_OC2Init(TIM4, &my_OCInit);
	TIM_OC3Init(TIM4, &my_OCInit);
	TIM_OC4Init(TIM4, &my_OCInit);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_Cmd(TIM4, ENABLE);

}

/**
  * @brief  Main program
  */
int main(void)
{
	int period=0;
	
	/* SysTick end of count event each 10ms */
  	RCC_GetClocksFreq(&RCC_Clocks);
  	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
		
  	/* Infinite loop */
	while (1)
	{
		while(8399 >= period)
		{
			timer_fun(period);
			period++;
			delay(2000);
		}
		while(period >=0)
		{
			timer_fun(period);
			period--;
			delay(2000);
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
