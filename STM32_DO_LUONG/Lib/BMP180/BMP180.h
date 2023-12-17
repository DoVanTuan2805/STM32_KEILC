#ifndef _BMP180_H_
#define _BMP180_H_

#include "stm32f1xx_hal.h"



void BMP180_Start (void);

float BMP180_GetTemp (void);

float BMP180_GetPress (int oss);

float BMP180_GetAlt (int oss);

#endif
