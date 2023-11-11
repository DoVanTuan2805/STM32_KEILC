#include "pin.h"
uint32_t pinNum;
void pinMode(Pin_t pin, pinMode_t modeGpio)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		if(modeGpio == OUTPUT)
		{
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		}
		else if(modeGpio == INPUT)
		{
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
		}
		
		if(pin <= PA15)
		{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				pinNum = 1 << pin;
				GPIO_InitStruct.Pin = (uint16_t)pinNum;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		}else if(pin > PA15 && pin <= PB15)
		{
				__HAL_RCC_GPIOB_CLK_ENABLE();
				pinNum = 1 << (pin - 16);
				GPIO_InitStruct.Pin = (uint16_t)pinNum;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		}else if(pin > PB15 && pin <= PC15)
		{
				__HAL_RCC_GPIOC_CLK_ENABLE();
				pinNum = 1 << (pin - 32);
				GPIO_InitStruct.Pin = (uint16_t)pinNum;
				HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		}
		
		
}

void digitalWrite(Pin_t pin, uint8_t state)
{
		if(pin <= PA15)
		{
				pinNum = 1 << pin;
				HAL_GPIO_WritePin(GPIOA, pinNum, state);
		}else if(pin > PA15 && pin <= PB15)
		{
				pinNum = 1 << (pin - 16);
				HAL_GPIO_WritePin(GPIOB, pinNum, state);
		}else if(pin > PB15 && pin <= PC15)
		{
				pinNum = 1 << (pin - 32);
				HAL_GPIO_WritePin(GPIOC, pinNum, state);
		}
}

uint8_t digitalRead(Pin_t pin)
{
		uint8_t state;
		if(pin <= PA15)
		{
				pinNum = 1 << pin;
				state = (HAL_GPIO_ReadPin(GPIOA, pinNum));
		}else if(pin > PA15 && pin <= PB15)
		{
				pinNum = 1 << (pin - 16);
				state = (HAL_GPIO_ReadPin(GPIOB, pinNum));
		}else if(pin > PB15 && pin <= PC15)
		{
				pinNum = 1 << (pin - 32);
				state = (HAL_GPIO_ReadPin(GPIOC, pinNum));
		}
		return state;
}

