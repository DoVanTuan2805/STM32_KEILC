#include "stm32f10x.h"
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

#define PORTA	((GPIO_typeDef*) 0x40010800)

void GpioConfigOutput(void);
void EXIT_B12_Config(void);
void Delay(int time);
void EXTI15_10_IRQHandler(void);
static uint8_t state;

int main(void)
{
	GpioConfigOutput();
	EXIT_B12_Config();
	while(1)
	{
		if(state == 1)
		{
			PORTA->ODR &= ~(1U << 6);
			PORTA->ODR &= ~(1U << 7);
		}
		else 
		{
			PORTA->ODR |= (1U << 6);
			PORTA->ODR |= (1U << 7);
		}
		
	}
}
void GpioConfigOutput(void)
{
	RCC->APB2ENR |= (1U << 2);		/*	ENABLE CLOCK PORT A */
	
	PORTA->CRL &= (0x00 << 24);		/*	RESET PIN 6 PORT A	*/
	PORTA->CRL |= (0x03 << 24);		/*	0x03 = 0011 -> OUTPUT 50MHZ	*/
	
	PORTA->ODR |= (1U << 6);
	
	PORTA->CRL &= (0x00 << 28);		/*	RESET PIN 7 PORT A	*/
	PORTA->CRL |= (0x03 << 28);		/*	0x03 = 0011 -> OUTPUT 50MHZ	*/
	
	PORTA->ODR |= (1U << 7);
}
void EXIT_B12_Config(void)
{
	RCC->APB2ENR |= (1 << 0);  /* Enable AFIO CLOCK */
	
	AFIO->EXTICR[3] &= ~(0x01U << 0);		/* CLEAR EXTERNAL CONFIG REGISTER PB12 */
	
	AFIO->EXTICR[3] |= (0x01 << 0);  		/* SET EXTERNAL CONFIG REGISTER PB12 */
	
	/*
			0 : Interrupt request from Line x is masked
			1 : Interrupt request from Line x is not masked
	*/
	EXTI->IMR |= (1U << 12);					/* ENABLE MASK REGISTER	*/
	
	/* Enable RISING Edge Trigger for PB12 */
	EXTI->FTSR |= ( 1U << 12);  
	
	/* Disable FALLING Edge Trigger for PB12 */
	EXTI->RTSR &= ~( 1U << 12); 
	
	/* ENABLE PENDING REGISTER for PB12 */
	EXTI->PR |= (1U << 12);
	
	/* SET INTERRUPT ENABLE REGISTER for PB12 */
	NVIC->ISER[1] |= (1U << 8); 
	
}
void EXTI15_10_IRQHandler(void)
{
	uint32_t PR_REGISTER = 0;
	uint32_t IMR_REGISTER = 0;
	
	PR_REGISTER = EXTI->PR;
	IMR_REGISTER = EXTI->IMR;
	
	EXTI->PR = PR_REGISTER;

	/* if(((PR_REGISTER & 0x1000) != 0) && (((IMR_REGISTER & 0x1000)) != 0))	*/
	if((PR_REGISTER |= (1U << 12) != 0) && (IMR_REGISTER |= (1U << 12) != 0))
	{
			if(state == 1)
			{
				state = 0;
			}
			else 
			{
				state = 1;
			}
	}
}
void Delay(int time)
{
	while(time--);
}
