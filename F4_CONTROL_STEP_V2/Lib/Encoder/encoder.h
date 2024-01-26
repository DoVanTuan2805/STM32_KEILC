#ifndef ENCODER_H
#define ENCODER_H
#include "stm32f4xx_hal.h"

void handleInterruptEncoder(uint16_t GPIO_Pin, GPIO_TypeDef* GPIO_A_PORT, uint16_t GPIO_A_Pin, GPIO_TypeDef* GPIO_B_PORT, uint16_t GPIO_B_Pin);

#endif