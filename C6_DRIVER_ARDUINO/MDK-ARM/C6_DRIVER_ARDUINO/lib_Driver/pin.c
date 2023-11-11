#include "pin.h"

void pinMode(const char* pin, pinMode_t modeGpio)
{
		if(pin[0] == 'P')
		{
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			if(pin[1] == 'A')
			{}
			if(pin[1] == 'B')
			{}
			if(pin[1] == 'C')
			{
					/* GPIO Ports Clock Enable */
					__HAL_RCC_GPIOC_CLK_ENABLE();
					switch (modeGpio)
					{
						case OUTPUT:
							if(strstr(pin, "13") == 0)
							{
									HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
								
									GPIO_InitStruct.Pin = GPIO_PIN_13;
									GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
									GPIO_InitStruct.Pull = GPIO_NOPULL;
									GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
									HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
							}
							break;
						case INPUT:
							break;
						case INPUT_PULLUP:
							break;
						case INPUT_PULLDOWN:
							break;
					}
			}
		}
}

void digitalWrite(const char* pin, uint8_t state)
{
		if(pin[0] == 'P')
		{
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			if(pin[1] == 'A')
			{}
			if(pin[1] == 'B')
			{}
			if(pin[1] == 'C')
			{
				if(strstr(pin, "13") == 0)
				{
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, state);
				}
			}
		}
}


