/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "string.h"

static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

int length =0;	//input string lenth
char str[100];	//input string store

//Initialization function
void usart3_fun()
{
	//structure initialise
	USART_InitTypeDef	my_usart_init;
	GPIO_InitTypeDef	my_gpio_init;
	
	
	//Enable clock for GPIO and USART
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//Alternate Function GPIO initializations
	
	my_gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	//10 --> Tx and 11 --> Rx
	my_gpio_init.GPIO_Mode	=	GPIO_Mode_AF;
	my_gpio_init.GPIO_OType = GPIO_OType_PP;
	my_gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	my_gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	
	//Alternate Function
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	//USART Base Initialisation

	my_usart_init.USART_BaudRate = 9600;
	my_usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	my_usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	my_usart_init.USART_Parity = USART_Parity_No;
	my_usart_init.USART_StopBits = USART_StopBits_1;
	my_usart_init.USART_WordLength = USART_WordLength_8b;
	
	//Init functions
	
	GPIO_Init(GPIOB, &my_gpio_init);
	USART_Init(USART3, &my_usart_init);
	
	//ENABLE Peripheral
	
	USART_Cmd(USART3, ENABLE);
	
}


/*function to sending a character	*/

void USART3_send_char(char ch)
{
	//checking for flag
	while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
	//send character
	USART_SendData(USART3, (int )ch);
}

/*	function to sending a string	*/

void USART3_string(char *str)
{
	while(*str)
	{
		USART3_send_char(*str++);
	}
	memset(str,'\0',length);
	length = 0;
}

//get from terminal

char USART_GetChar()
{
    while (!USART_GetFlagStatus(USART3, USART_FLAG_RXNE));
    return USART_ReceiveData(USART3);
}

/*

	Function for handling Input sequence end.
	This will print the information received on terminal.

*/
void input_end(void)
{
	USART3_send_char('\n');
	USART3_string(str);
	USART3_send_char('\n');
	memset(str,var,count);		//resetting memory with NULL characters
	count = 0;			//Point start of the buffere for fresh data
	USART3_string("success\n\r");
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	char ch;
	int count =0;
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	uint16_t var = '\0';
	
	usart3_fun();
		
  /* Infinite loop */
  while (1)
  {
		ch = USART_GetChar();	//waiting for a character
//		USART3_send_char(ch);	//For debug user can print input character
		str[count] = ch;			//storing the character.
		count++;							//storing data length
	  
	  //If transmission is completed.
		if (ch== 0x0D)				//checking for the enter ascii then printing complete string in next line and going to next line for fresh entry
		{
			input_end();	//Input End token received
		}
  }
}
