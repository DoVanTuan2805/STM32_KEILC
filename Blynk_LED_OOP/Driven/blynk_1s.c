#include "blynk_1s.h"

static void ModeBlynk_1s(GPIO_t const *const pGpio);
static void ModeBlynk_2s(GPIO_t  const *const pGpio);
void Blynk_ctor(BlynkMode_t *blynkMode, 
                ModeBlynk_t modeBlynk,
                GPIO_TypeDef *GpioPort, 
				uint16_t gpioPin, 
				GPIOMode_TypeDef GpioMode, 
				GPIOSpeed_TypeDef GpioSpeed)
{
    static struct VtableModeLed BlynkVtable =
    {
        &ModeBlynk_1s,
        &ModeBlynk_2s
    };
    
    Gpio_config(&blynkMode->gpio, GpioPort, gpioPin, GpioMode, GpioSpeed);
	blynkMode->gpio.vptr = &BlynkVtable;
    blynkMode->mode = modeBlynk;
}
static void delay(int n)
{
	while(n--);
}
static void ModeBlynk_1s(GPIO_t  const *const pGpio)
{
	Gpio_SetLow(pGpio);
	delay(1000000);
	Gpio_SetHigh(pGpio);
	delay(1000000);
}
static void ModeBlynk_2s(GPIO_t  const *const pGpio)
{
	Gpio_SetLow(pGpio);
	delay(2000000);
	Gpio_SetHigh(pGpio);
	delay(2000000);
}

