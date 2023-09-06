#include "Response_CI.h"

static UART_HandleTypeDef *handleUart;

void ResponseInit(UART_HandleTypeDef *Uart)
{
		handleUart = Uart;
}

void Response_print(const char *str, ...)
{
		char stringArray[100];
		
		va_list args;
		va_start(args, str);
		vsprintf(stringArray, str, args);
		va_end(args);
		
		HAL_UART_Transmit(handleUart, (uint8_t*)stringArray, strlen((char*)stringArray), 100);
}

