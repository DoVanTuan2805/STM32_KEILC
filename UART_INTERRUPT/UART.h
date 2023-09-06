#include "stm32f10x.h"
#include "string.h"

typedef enum
{
		UART_1 = 1,
		UART_2 = 2,
		UART_3 = 3
}UART_t;

typedef enum
{
		POLLING,
		INTERRUPT
}MODE_t;

void ConfigUART(UART_t UART, MODE_t MODE);

void ConfigNVIC_UART(UART_t UART);

void UART_Init(UART_t UART, MODE_t MODE);

void UARTSendChar(UART_t UART ,uint8_t c);

void UARTSendString(UART_t UART, uint8_t *c);

void UARTGet(UART_t UART ,uint8_t *data, uint16_t length);

extern void USART1_IRQHandler(void);

extern void USART2_IRQHandler(void);

extern void USART3_IRQHandler(void);

