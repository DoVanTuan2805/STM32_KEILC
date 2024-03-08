#include "encoder.h"
#include "gpio.h"
static uint64_t timeReadEncoder;
__weak void decrese_callback(uint16_t GPIO_Pin)
{
}
__weak void increse_callback(uint16_t GPIO_Pin)
{
}
void handleInterruptEncoder(uint16_t GPIO_Pin, GPIO_TypeDef *GPIO_A_PORT, uint16_t GPIO_A_Pin, GPIO_TypeDef *GPIO_B_PORT, uint16_t GPIO_B_Pin)
{
	if (HAL_GetTick() - timeReadEncoder > 50)
	{
		if (GPIO_Pin == GPIO_A_Pin)
		{
			if (HAL_GPIO_ReadPin(GPIO_B_PORT, GPIO_B_Pin) == 1)
			{
				decrese_callback(GPIO_Pin);
			}
			else if (HAL_GPIO_ReadPin(GPIO_B_PORT, GPIO_B_Pin) == 0)
			{
				increse_callback(GPIO_Pin);
			}
		}
	}
	timeReadEncoder = HAL_GetTick();
}
