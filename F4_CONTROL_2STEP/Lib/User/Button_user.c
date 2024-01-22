#include "Button_user.h"
#include "gpio.h"
#include "ILI9431_user.h"
#include "tim.h"
Button_t bt1, bt2;

extern bool changeMode;
static Screen_t screen;

extern Screen_t screen_s;
void initButton(void)
{
		Button_Init(&bt1, BT1_GPIO_Port, BT1_Pin);
		Button_Init(&bt2, BT2_GPIO_Port, BT2_Pin);
}
void buttonHandle(void)
{
		bt_handle(&bt1);
		bt_handle(&bt2);

}
void bt_press_callback(Button_t *button) {
	if(button == &bt2)
	{
			if (screen == MANUAL_SCREEN)
			{
					changeMode = true;
					screen = AUTO_SCREEN;
					__HAL_TIM_SetCounter(&htim2, 1);
			}
			else if (screen == AUTO_SCREEN)
			{
					changeMode = true;
					screen = MANUAL_SCREEN;
					__HAL_TIM_SetCounter(&htim2, 1);
			}
			screen_s = screen;
	}
}