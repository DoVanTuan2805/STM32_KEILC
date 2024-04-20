#include "driven.h"
#include "assert.h"
static void Blynk_1s_(GPIO_t  const *const pGpio);
static void Blynk_2s_(GPIO_t  const *const pGpio);
void Gpio_config(GPIO_t *pGpio, 
				GPIO_TypeDef *GpioPort, 
				uint16_t gpioPin, 
				GPIOMode_TypeDef GpioMode, 
				GPIOSpeed_TypeDef GpioSpeed)
{
    struct VtableModeLed Vtable = {
        &Blynk_1s_,
        &Blynk_2s_
    };
    pGpio->vptr= &Vtable;
    pGpio->GpioPin = gpioPin;
    pGpio->GpioSpeed = GpioSpeed;
    pGpio->GpioMode = GpioMode;
    pGpio->GpioPort = GpioPort;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
    GPIO_InitTypeDef GpioInitStruct = {};
    GpioInitStruct.GPIO_Pin = pGpio->GpioPin;
    GpioInitStruct.GPIO_Speed = pGpio->GpioSpeed;
    GpioInitStruct.GPIO_Mode = pGpio->GpioMode;
	GPIO_Init(GpioPort, &GpioInitStruct);
}
void Gpio_SetLow(GPIO_t  const *pGpio)
{
	GPIO_ResetBits(pGpio->GpioPort, pGpio->GpioPin);
}
void Gpio_SetHigh(GPIO_t  const *pGpio)
{
    GPIO_SetBits(pGpio->GpioPort, pGpio->GpioPin);
}
void EnableModeBlynk_1s(GPIO_t const *pGpio[], int numArray)
{
	for(int i = 0; i < numArray; i++)
	{
		Blynk_1s(pGpio[i]);
	}
}
void EnableModeBlynk_2s(GPIO_t const *pGpio[], int numArray)
{
	for(int i = 0; i < numArray; i++)
	{
		Blynk_2s(pGpio[i]);
	}
}
static void Blynk_1s_(GPIO_t  const *const pGpio)
{
    assert(0);
}
static void Blynk_2s_(GPIO_t  const *const pGpio)
{
    assert(0);
}
