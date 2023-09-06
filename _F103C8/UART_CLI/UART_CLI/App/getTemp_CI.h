#ifndef GETTEMP_CI_H
#define GETTEMP_CI_H
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "UART.h"
#include "Response_CI.h"
#include "Sensor.h"
extern UART_HandleTypeDef huart1;
void getTemp(char **argv, uint8_t argvNum);
#endif