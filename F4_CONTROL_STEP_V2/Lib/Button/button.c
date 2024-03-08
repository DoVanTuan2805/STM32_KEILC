#include "button.h"

#define TIMEOUT 3000

// ----------------- FUNCTION ---------------
__weak void bt_press_callback(volatile Button_t *button)
{
}
__weak void bt_release_callback(volatile Button_t *button)
{
}
__weak void bt_press_timout_callback(volatile Button_t *button)
{
}
void bt_handle(volatile Button_t *button)
{ // GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin
	volatile uint8_t state = HAL_GPIO_ReadPin(button->PORT, button->PIN);
	if (state != button->btn_filter)
	{ /// loc nhieu nut nhan
		button->btn_filter = state;
		button->t_debounce = HAL_GetTick();
	}
	if (HAL_GetTick() - button->t_debounce >= 15)
	{ /// het khoang thoi gian nhieu
		button->btn_current = state;
		if (button->btn_current != button->btn_last)
		{ // dang nhan
			if (button->btn_current == 0)
			{
				button->time_start_press = HAL_GetTick();
				button->is_press = 1;
				bt_press_callback(button);
			}
			else
			{
				button->is_press = 0;
				bt_release_callback(button);
			}
			button->btn_last = button->btn_current;
		}
	}
	if (button->is_press == 1 && HAL_GetTick() - button->time_start_press >= TIMEOUT)
	{
		button->is_press = 0;
		bt_press_timout_callback(button);
	}
}
void Button_Init(volatile Button_t *button, GPIO_TypeDef *PORT, uint16_t PIN)
{
	button->PORT = PORT;
	button->PIN = PIN;
	button->btn_filter = 1;
	button->btn_last = 1;
	button->btn_current = 0;
	button->t_debounce = 0;
	button->time_start_press = 0;
	button->is_press = 0;
}
