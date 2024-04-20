#ifndef DRIVEN_H
#define DRIVEN_H

#include "stm32f10x_gpio.h"
#include "stdint.h"
struct VtableModeLed;

typedef struct
{
    struct VtableModeLed *vptr;
	GPIOSpeed_TypeDef GpioSpeed;
	GPIO_TypeDef *GpioPort;
	uint16_t GpioPin;
	GPIOMode_TypeDef GpioMode;
}GPIO_t;
struct VtableModeLed
{
    void (*blynk1s) (GPIO_t  const *pGpio);
    void (*blynk2s) (GPIO_t  const *pGpio);
};

static inline void Blynk_1s(GPIO_t  const *const pGpio)
{
    (*pGpio->vptr->blynk1s) (pGpio);
}
static inline void Blynk_2s(GPIO_t  const *const pGpio)
{
    (*pGpio->vptr->blynk2s) (pGpio);
}

void Gpio_config(GPIO_t *pGpio, 
				GPIO_TypeDef *GpioPort, 
				uint16_t gpioPin, 
				GPIOMode_TypeDef GpioMode, 
				GPIOSpeed_TypeDef GpioSpeed);
void Gpio_SetLow(GPIO_t  const *pGpio);
void Gpio_SetHigh(GPIO_t  const *pGpio);
void EnableModeBlynk_1s(GPIO_t const *pGpio[], int numArray);
void EnableModeBlynk_2s(GPIO_t const *pGpio[], int numArray);
#endif

