#include "stm32f10x.h"

void ConfigUART1(void);
void UARTSendChar(uint16_t c);
void UARTSendString(uint16_t *c);
uint16_t UARTGetString(void);
void delay(int ret);

static uint16_t chat;

int main(void)
{
		ConfigUART1();
		while(1)
		{
				//UARTSendChar(chat);
				chat = UARTGetString();
				UARTSendChar(chat);
				//delay(1000000);
		}
}
void ConfigUART1(void)
{
		/*	ENABLE CLOCK GPOIO A	*/
		RCC->APB2ENR |= (1U << 2);	
	
		/*	PA9 : MODE : OUTPUT MODE 50Mhz / ALTERNATE FUNCTION OUTPUT PUSH PULL	*/
		GPIOA->CRH &= 0xFFFFFF0F;
		GPIOA->CRH |= (0x0E << 4);
		
		
		/*	PA10 : MODE : INPUT	/ INPUT WITH PULL-UP / PULL-DOWN*/
		GPIOA->CRH &= 0xFFFFF0FF;
		GPIOA->CRH |= (0x08 << 8);
		
		/*	ENABLE CLOCK ALTERNATE FUNCTION	*/
		RCC->APB2ENR |= (1U << 0);
	
		/*	ENABLE CLOCK UART 2	*/
		RCC->APB2ENR |= (1U << 14);
	
		/*	
				CONFIG BAUDRATE : 19200 
				USART1 USE CLOCK APB2 -> MAX 72Mhz
				
				Tx/Rx Baud = (72Mhz) / 16 * (USART  DIV)
				
				USART  DIV = (72Mhz) / 16 * Tx/Rx Baud
				
				USART  DIV = (72Mhz) / 16 * 9600			-> 234,375

				DIV_Mantissa = 234				= EA
				
				DIV_Fraction = 0,375 * 16	= 6
				
		*/
		USART1->BRR = 0xEA6;
		
		/*	ENABLE TRANSMIT	*/
		USART1->CR1 |= (1U << 3);
		
		/*	ENABLE RECEIVE	*/
		USART1->CR1 |= (1U << 2);
		
		/*	ENABLE USART	*/
		USART1->CR1 |= (1U << 13);
}
void UARTSendChar(uint16_t c)
{
		USART1->DR = c;
		while (!(USART1->SR & (1<<6)));
}
void UARTSendString(uint16_t *c)
{
		uint8_t u8Count = 0;
		while(c[u8Count] != '\0')
		{
				UARTSendChar(c[u8Count]);
				u8Count++;
		}
}
uint16_t UARTGetString(void)
{
		uint16_t data;
		while (! (USART1->SR & (1<<5)) );
		data = USART1->DR;
		return data;
}
void delay(int ret)
{
	while (ret-- > 0);
}