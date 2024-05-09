#ifndef UART_H_
#define UART_H_

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h" 
#include "stdlib.h"

void UART_init(UART_HandleTypeDef *uart);
void UART_putChar(unsigned char _data);
void Uart_Printf(const char *str, ...);
#endif /* UART_H_ */

