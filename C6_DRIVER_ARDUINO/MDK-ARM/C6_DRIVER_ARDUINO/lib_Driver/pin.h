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

void pinMode(const char* pin, pinMode_t modeGpio);

void digitalWrite(const char* pin, uint8_t state);

#endif