//STM32F407


//Memory mapping registers

//clock configurations
#define RCC_BASE_ADDR 	0x40023800
#define	RCC_CR 					(RCC_BASE_ADDR+0x00)						//16-Bit HSE_ON
#define	RCC_CFGR 				(RCC_BASE_ADDR+0x08)						//01 for HSE as system clock(Bits 0 and 1)
#define RCC_AHB1ENB 		(RCC_BASE_ADDR+0x30)						//GPIOD Enable 3-Bit

//GPIOD configurations

#define GPIOD_BASE_ADDR 			0x40020C00
#define	GPIOD_MODE			(GPIOD_BASE_ADDR+0x00)					//01 for general purpose
#define	GPIO_PUPDR			(GPIOD_BASE_ADDR+0x0C)					//01 pull-up
#define	GPIOD_ODR				(GPIOD_BASE_ADDR+0x14)					//each bit for each pin first 16 pins are used

#define REG(address) (*(unsigned int *)address)					//function poiner to the address
	


void delay(int val)
{
	//Some delay function
	int i,j;
	for(i=0;i<val;i++)
	{
		j=0;
		while(j<=2000)
		{
			//do nothing
			j++;
		}
	}
}

//15 -> blue
//14 -> red
//13 -> orange
//12 -> green

int main()
{
	REG(RCC_CR) |= (1<<16);
	REG(RCC_CFGR) |= (1<<0);
	REG(RCC_AHB1ENB) |= (1<<3);
	//12,13,14,15 pins are connected to on-board LED's
	REG(GPIOD_MODE) |= (1<<24) | (1<<26) | (1<<28) | (1<<30);				//Enbale PULL UP for the pins
	REG(GPIO_PUPDR)	|= (1<<24) | (1<<26) | (1<<28) | (1<<30);				//General Pupose Output mode for the pins
	while(1)
	{
		//ON LED
		REG(GPIOD_ODR) |= (1<<12);					//Setting pin high
		delay(500);
		REG(GPIOD_ODR) |= (1<<13);
		delay(500);
		REG(GPIOD_ODR) |= (1<<14);
		delay(500);
		REG(GPIOD_ODR) |= (1<<15);
		delay(500);
		
		//OFF LED
		REG(GPIOD_ODR) &= ~(1<<12);					//Clearing pin
		delay(500);
		REG(GPIOD_ODR) &= ~(1<<13);
		delay(500);
		REG(GPIOD_ODR) &= ~(1<<14);
		delay(500);
		REG(GPIOD_ODR) &= ~(1<<15);
		delay(500);
	}
}

