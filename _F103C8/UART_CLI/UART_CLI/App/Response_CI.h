#ifndef RESPONSE_CI_H
#define RESPONSE_CI_H
#include "stdio.h"
#include "stdarg.h"
#include "string.h" 
#include "stm32f1xx_hal.h"

void ResponseInit(UART_HandleTypeDef *Uart);
void Response_print(const char *str, ...);
#endif
