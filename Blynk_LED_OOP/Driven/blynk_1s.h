#ifndef BLYNK1_H
#define BYLNK1_H

#include "driven.h"
typedef enum
{
    BLYNK1S,
    BLYNK2S
}ModeBlynk_t;

typedef struct 
{
    GPIO_t gpio;
    ModeBlynk_t mode;
}BlynkMode_t;
void Blynk_ctor(BlynkMode_t *blynkMode, 
                ModeBlynk_t modeBlynk,
                GPIO_TypeDef *GpioPort, 
				uint16_t gpioPin, 
				GPIOMode_TypeDef GpioMode, 
				GPIOSpeed_TypeDef GpioSpeed);
#endif
