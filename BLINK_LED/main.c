#include "stm32f10x.h"

typedef enum 
{
	PORTA,
	PORTB,
	PORTC,
	PORTD,
	PORTE
} GPIO_PORT;

typedef enum 
{
	PullDown,
	PullUp
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

#define PORT_A ((GPIO_typeDef*) 0x40010800)
#define PORT_B ((GPIO_typeDef*) 0x40010C00)
#define PORT_C ((GPIO_typeDef*) 0x40011000)
#define PORT_D ((GPIO_typeDef*) 0x40011400)

static uint32_t u32Count;

void delay(int ret);
void GpioConfigOutput(void);
void GpioConfigInput(void);
void GpioSetInput(GPIO_PORT port, uint16_t pin, GPIO_MODE mode);

int main(void)
{
	GpioConfigOutput();
	GpioConfigInput();
	while(1)
	{
		GPIOC->ODR ^= 0x2000U;
		delay(1000000);
		/*
		if((PORT_B->IDR & (1U << 12)) == 0)
		{
			delay(800000);
			if((PORT_B->IDR & (1U << 12)) == 0){
				u32Count++;
				if(u32Count % 2U == 0){
					PORT_A->ODR |= (1 << 7);
				}
				else 
				{
					PORT_A->ODR &= ~(1U << 7);
				}
			}
		}
		*/
		
		
		/*
		GPIOC->ODR |= 0x2000U;
		delay(1);
		GPIOC->ODR &= (~0x2000U);
		delay(1);
		*/
	}

}
void delay(int ret)
{
	while (ret-- > 0);
}
void GpioConfigOutput(void)
{
	RCC->APB2ENR |= (1U << 2);		/*  ENABLE CLOCK PORT A */
	
	PORT_A->CRL &= 0x0FFFFFFFU;		/* RESET PORT A PIN 7*/
	PORT_A->CRL |= (0x03U << 28);		/* SET PIN 7 OUTPUT , MODE7 : 11 -> OUTPUT MAX 50MHZ*/
	PORT_A->ODR |= (1 << 7);				/* SET PIN 7 */
	
	RCC->APB2ENR |= 0x10U;				/* 0b1000 ENABLE CLOCK PORT C*/
	
	PORT_C->CRH &= 0xFF0FFFFFU;
	PORT_C->CRH |= 0x00300000U;
	PORT_C->ODR |=0x2000;
}
void GpioConfigInput(void)
{
	RCC->APB2ENR |= (1U << 3);		/*  ENABLE CLOCK PORT B */
	/* Reset the PORT B PIN 12 */
	PORT_B->CRH &= (0x00 << 16);
	/* Input Mode and 10: Input with pull-up / pull-down  */
	PORT_B->CRH |= (0x08 << 16);
	/* Pull up */
	PORT_B->ODR |= (1U << 12);
}
void GpioSetInput(GPIO_PORT port, uint16_t pin, GPIO_MODE mode)
{
	switch(port)
	{
		case PORTA:
			RCC->APB2ENR |= (1U << 2);
			break;
		case PORTB:
			RCC->APB2ENR |= (1U << 3);
			break;
		case PORTC:
			RCC->APB2ENR |= (1U << 4);
			break;
		case PORTD:
			RCC->APB2ENR |= (1U << 5);
			break;
		case PORTE:
			RCC->APB2ENR |= (1U << 6);
			break;
	}
	switch(mode)
	{
		case PullDown:
			break;
		case PullUp:
			break;
	}
	
}


