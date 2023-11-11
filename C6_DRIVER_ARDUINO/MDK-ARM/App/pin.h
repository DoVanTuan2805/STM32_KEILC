#ifndef pin_h
#define pin_h
#include "stdint.h"
#include "string.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

typedef enum {
	OUTPUT,
	INPUT,
	INPUT_PULLUP,
	INPUT_PULLDOWN,
}pinMode_t;

typedef enum {
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15
}Pin_t;



void pinMode(Pin_t pin, pinMode_t modeGpio);
void digitalWrite(Pin_t pin, uint8_t state);
uint8_t digitalRead(Pin_t pin);
#endif