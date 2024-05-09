#include "uart.h"

static UART_HandleTypeDef *huart;

void UART_init(UART_HandleTypeDef *uart)
{
  huart = uart;
}
void UART_putChar(unsigned char _data)
{
	uint8_t dat[1]= {_data};
	HAL_UART_Transmit_DMA(huart,dat,1);
}
void Uart_Printf(const char *str, ...)
{
		char *stringArray = malloc(100 * sizeof(char));
		//char stringArray[100];
		va_list args;
		va_start(args, str);
		vsprintf(stringArray, str, args);
		va_end(args);
		
		HAL_UART_Transmit(huart, (uint8_t*)stringArray, strlen((char*)stringArray), 1000);
		free(stringArray);
}

//--------------------------------------------END------------------------------------//


