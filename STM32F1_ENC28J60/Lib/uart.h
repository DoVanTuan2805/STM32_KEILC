#ifndef UART_H_
#define UART_H_

#include "stm32f1xx_hal.h"
void UART_init(UART_HandleTypeDef *uart);
void UART_putChar(unsigned char _data);
void UART_putString(char *string);

#endif /* UART_H_ */

