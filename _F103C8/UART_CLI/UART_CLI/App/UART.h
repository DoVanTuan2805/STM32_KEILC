#ifndef UART_H
#define UART_H

#include "stdint.h"
#include "string.h"
#include "getTemp_CI.h"

void ReceiveData(uint8_t dataRx);
void UartHandle(void);

#endif