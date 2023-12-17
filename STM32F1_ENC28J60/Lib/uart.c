#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static UART_HandleTypeDef *huart;

void UART_init(UART_HandleTypeDef *uart)
{
  huart = uart;
}
void UART_putChar(unsigned char _data)
{
	uint8_t dat[1]= {_data};
  HAL_UART_Transmit(huart,dat,1,10);
}
void UART_putString(char *string)
{
  HAL_UART_Transmit(huart,(uint8_t *)string,strlen((const char *)string),1000);
}


//--------------------------------------------END------------------------------------//


