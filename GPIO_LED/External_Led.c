#include "main.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/**
  * @brief  Main program
  */
int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  /* Enable the GPIOC peripheral */ 
  GPIO_InitTypeDef my_gpio_struct;						//GPIO structure
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);				//Enabling clock for GPIOC
  
  my_gpio_struct.GPIO_Pin = GPIO_Pin_2 ;	            //Enable LED pin 02
  my_gpio_struct.GPIO_Mode = GPIO_Mode_OUT;				  	//Mode of the pin IN/OUT/AF/AN
  my_gpio_struct.GPIO_OType = GPIO_OType_PP;					//Push-Pull or Open Drain
  my_gpio_struct.GPIO_PuPd = GPIO_PuPd_UP; 					  //Pull-Up,Pull-Down,None
  
  GPIO_Init(GPIOC, &my_gpio_struct);						//Initialize the structure
    	 
  /* Infinite loop */
  while (1)
  {
		GPIO_ToggleBits(GPIOC, 0xf000);					//Toggle GPIOC PIN number 2
		Delay(100);
  }
}


//Delay function

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

