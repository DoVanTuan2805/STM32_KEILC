#include "UART.h"

void ConfigUART(UART_t UART, MODE_t MODE)
{
		USART_TypeDef *uartReal;
		switch (UART)
		{
				case UART_1:
						uartReal = USART1;
						break;
				case UART_2:
						uartReal = USART2;
						break;
				case UART_3:
						uartReal = USART3;
						break;
		}
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
		uartReal->BRR = 0xEA6;
		
		/*	ENABLE TRANSMIT	*/
		uartReal->CR1 |= (1U << 3);
		
		/*	ENABLE RECEIVE	*/
		uartReal->CR1 |= (1U << 2);
		
		if(MODE == INTERRUPT)
		{
				/*	ENABLE INTERUPT RECEIVE	*/
				uartReal->CR1 |= (1U << 5);
				ConfigNVIC_UART(UART);
		}
		/*	ENABLE USART	*/
		uartReal->CR1 |= (1U << 13);
}
/*--------------------------------------------------------------------*/
void ConfigNVIC_UART(UART_t UART)
{
		volatile uint8_t posion;
		if(UART == UART_1)
		{
				posion = 5;
		}
		else if(UART == UART_2)
		{
				posion = 6;
		}
		else
		{
				posion = 7;
		}
		NVIC->ISER[1] |= (1U << posion);
		NVIC->ICPR[1] |= (1U << posion);
}
/*--------------------------------------------------------------------*/
void UART_Init(UART_t UART, MODE_t MODE)
{
		switch (UART)
		{
				case UART_1:
						ConfigUART(UART, MODE);
						break;
				case UART_2:
						break;
				case UART_3:
						break;
		}
}
/*--------------------------------------------------------------------*/
void UARTSendChar(UART_t UART ,uint8_t c)
{
		USART_TypeDef *uartReal;
		switch (UART)
		{
				case UART_1:
						uartReal = USART1;
						break;
				case UART_2:
						uartReal = USART2;
						break;
				case UART_3:
						uartReal = USART3;
						break;
		}
		uartReal->DR = c;
		while (!(uartReal->SR & (1<<6)));
}
/*--------------------------------------------------------------------*/
void UARTSendString(UART_t UART, uint8_t *c)
{
		uint8_t u8Count = 0;
		while(c[u8Count] != '\0')
		{
				UARTSendChar( UART ,c[u8Count]);
				u8Count++;
		}
}
/*--------------------------------------------------------------------*/
void UARTGet(UART_t UART ,uint8_t *data, uint16_t length)
{
		uint16_t u16Count;
		USART_TypeDef *uartReal;
		switch (UART)
		{
				case UART_1:
						uartReal = USART1;
						break;
				case UART_2:
						uartReal = USART2;
						break;
				case UART_3:
						uartReal = USART3;
						break;
		}
		for (u16Count = 0 ; u16Count < length; u16Count++)
		{
				while (!(uartReal->SR & (1<<5)));
				*data = (uint8_t) uartReal->DR;
				data++;
		}
}