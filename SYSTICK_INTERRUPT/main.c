#include "stm32f10x.h"

typedef enum 
{
	PORTA,
	PORTB,
	PORTC
} GPIO_PORT;

typedef enum 
{
	I_PullDown,
	I_PullUp,
	O_2Mhz,
	O_10Mhz,
	O_50Mhz
} GPIO_MODE;

typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;
		
}GPIO_typeDef;

typedef struct
{
	union
	{
			uint32_t STK_CTRL;
			struct
			{
					uint32_t ENABLE:1;
					uint32_t TICKINT:1;
					uint32_t CLKSOUCRE:1;
					uint32_t unused1:13;
					uint32_t COUNTFLAG:1;
					uint32_t unused2:15;
			}Register;
			
	}SysTickControl;
	
	union
	{
			uint32_t STK_LOAD;
			struct
			{
					uint32_t RELOAD:24;
					uint32_t unused:8;
			}Register;
			
	}SysTickReloadValue;
	
	union
	{
			uint32_t STK_VAL;
			struct
			{
					uint32_t CURRENT:24;
					uint32_t unused:8;
			}Register;
			
	}SysTickCurrentValue;
	
	union
	{
			uint32_t STK_CALIB;
			struct
			{
					uint32_t TENMS:24;
					uint32_t unused:6;
					uint32_t SKEW:1;
					uint32_t NOREF:1;
			}Register;
			
	}SysTickCalibrationValue;
	
}SysTick_t;

#define SYSTICK ((SysTick_t *) 0xE000E010)

#define PORT_A ((GPIO_typeDef*) 0x40010800)
#define PORT_B ((GPIO_typeDef*) 0x40010C00)
#define PORT_C ((GPIO_typeDef*) 0x40011000)
#define PORT_D ((GPIO_typeDef*) 0x40011400)

void initGpio(GPIO_PORT PORT, uint16_t pin, GPIO_MODE mode);
void WriteGpio(GPIO_PORT PORT, uint16_t pin, uint8_t state);
void delay(int time);
void SystickDelay(uint32_t timeMillis);
void ConfigSysTickInterrupt(void);
void SysTick_Handler(void);

int main(void)
{
	ConfigSysTickInterrupt();
	initGpio(PORTA, 7, O_50Mhz);
	while(1)
	{
		//PORT_A->ODR ^= (1U << 7);
		
		//WriteGpio(PORTA, 7, 1);
		//SystickDelay(100);
		//WriteGpio(PORTA, 7, 0);
		//delay(1000000);
	}
}
void WriteGpio(GPIO_PORT PORT, uint16_t pin, uint8_t state)
{
	uint16_t tempPin;
	tempPin = pin;
	
	if(tempPin > 7)
	{
		tempPin = tempPin - 8;
	}
	if(PORT == PORTA)
	{
		state ? (PORT_A->ODR |= (1U << tempPin)) : (PORT_A->ODR &= ~(1U << tempPin));
	}
	else if(PORT == PORTB)
	{
		state ? (PORT_B->ODR |= (1U << tempPin)) : (PORT_B->ODR &= ~(1U << tempPin));
	}
	else if(PORT == PORTC)
	{
		state ? (PORT_C->ODR |= (1U << tempPin)) : (PORT_C->ODR &= ~(1U << tempPin));
	}
}

void initGpio(GPIO_PORT PORT, uint16_t pin, GPIO_MODE mode)
{
	uint16_t tempPin;
	tempPin = pin;
	if(tempPin > 7)
	{
		tempPin = tempPin - 8;
	}
	if(PORT == PORTA)
	{
		RCC->APB2ENR |= (1U << 2);
		if(pin <= 7)
		{
			PORT_A->CRL &= (0x00U << tempPin);				/* RESET PORT */
			switch (mode)
			{
				case O_2Mhz:
					PORT_A->CRL |= (0x02U << (tempPin * 4));			/* MODE7 : 10 -> OUTPUT MAX 2MHZ */
					break;
				case O_10Mhz:
					PORT_A->CRL |= (0x01U << (tempPin * 4));			/* MODE7 : 01 -> OUTPUT MAX 10MHZ */
					break;
				case O_50Mhz:
					PORT_A->CRL |= (0x03U << (tempPin * 4));			/* MODE7 : 11 -> OUTPUT MAX 50MHZ */
					break;
				case I_PullDown:
					PORT_A->CRL |= (0x08U << (tempPin * 4));
					GPIOB->ODR &= ~(1 << tempPin);
					break;
				case I_PullUp:
					PORT_A->CRL |= (0x08U << (tempPin * 4));
					GPIOB->ODR |= (1 << tempPin);
					break;
			}
		}
		else 
		{
			PORT_A->CRH &= (0x00U << tempPin);	
			PORT_A->CRL |= (1U << (tempPin * 4));
		}
		
	}
	else if (PORT == PORTB)
	{
		RCC->APB2ENR |= (1U << 3);
		PORT_B->CRL &= 0x0FFFFFFFU;	
	}
	else if (PORT == PORTC)
	{
		RCC->APB2ENR |= (1U << 4);
		PORT_B->CRL &= 0x0FFFFFFFU;	
	}
}

void delay(int time)
{
	while (time-- > 0);
}


/*	T = 1 / F = 1 / 72M */
/*	STK_LOAD = T * (72U * 1000U - 1U)  =  0.001ms */
void SystickDelay(uint32_t timeMillis)
{
	while( timeMillis != 0)
	{
			/*	config time reload */
			SYSTICK->SysTickReloadValue.STK_LOAD = 72U * 1000U - 1U;				
			/*	ENABLE CLOCK AHB */
			SYSTICK->SysTickControl.Register.CLKSOUCRE = 1U;
			/*	DISABLE SYSTICK INTERRUPT */
			SYSTICK->SysTickControl.Register.TICKINT = 0U;
			/*	ENABLE COUNTER */
			SYSTICK->SysTickControl.Register.ENABLE = 1U;
			/*	SET CURRENT VALUE */
			SYSTICK->SysTickCurrentValue.STK_VAL = 0U;
			while(SYSTICK->SysTickControl.Register.COUNTFLAG != 1)
			{}
			timeMillis--;
	}
}
void ConfigSysTickInterrupt(void)
{
		/*	config time reload */
		SYSTICK->SysTickReloadValue.STK_LOAD = 72U * 1000U - 1U;				
		/*	ENABLE CLOCK AHB */
		SYSTICK->SysTickControl.Register.CLKSOUCRE = 1U;
		/*	ENABLE SYSTICK INTERRUPT */
		SYSTICK->SysTickControl.Register.TICKINT = 1U;
		/*	ENABLE COUNTER */
		SYSTICK->SysTickControl.Register.ENABLE = 1U;
		/*	SET CURRENT VALUE */
		SYSTICK->SysTickCurrentValue.STK_VAL = 0U;
}
void SysTick_Handler(void)
{
	static uint32_t countTime;

	if(countTime % 1000 == 0)
	{
			PORT_A->ODR ^= (1U << 7);
			countTime = 0;
	}		
	countTime++;
}
