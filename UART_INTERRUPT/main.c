#include "stm32f10x.h"
#include "UART.h"
#include "stdio.h"
#include "string.h"
void USART1_IRQHandler(void);

void ConfigUART1(void);

void UARTGetString(uint8_t *data, uint16_t length);
void delay(int ret);

static uint8_t dataRev[10];

int main(void)
{
		ConfigUART(UART_1, INTERRUPT);
		while(1)
		{
				//UARTSendChar(UART_1, 'c');
		}
}

void USART1_IRQHandler(void)
{
		//UARTSendString(dataRev);
		//memset(dataRev, 0, sizeof(dataRev));
		static uint16_t i;
		if((USART1->SR & (1 << 5)))
		{
				dataRev[i] = (uint8_t) USART1->DR;
				i++;
				if( i == sizeof(dataRev))
				{
					UARTSendString(UART_1, dataRev);
					//memset(dataRev, 0 , sizeof(dataRev));
					i = 0;
				}
				
		}
		USART1->SR &= ~(1U << 5);
		NVIC->ICPR[1] |= (1U << 5);
}
void delay(int ret)
{
	while (ret-- > 0);
}
