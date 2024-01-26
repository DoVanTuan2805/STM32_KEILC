#include "Button_user.h"
#include "gpio.h"
#include "ILI9431_user.h"
Button_t bt_down, bt_up;

extern dataUser_t dataUserInput;
extern bool changeMode;
extern Screen_t screenCurr;
bool changeButton;
void initButton(void)
{
		Button_Init(&bt_down, BT_DOWN_GPIO_Port, BT_DOWN_Pin);
		Button_Init(&bt_up, BT_UP_GPIO_Port, BT_UP_Pin);
}
void buttonHandle(void)
{
		bt_handle(&bt_down);
		bt_handle(&bt_up);

}
void bt_press_callback(Button_t *button) {
	if(button == &bt_up)
	{
			if(screenCurr == ENCODER_ROTATION_SCREEN)
			{
					changeButton = true;
					dataUserInput.RotationInTFT += ROUND / dataUserInput.IndexRotation;
			}
	}
	else if(button == &bt_down)
	{
			if(screenCurr == ENCODER_ROTATION_SCREEN)
			{
					changeButton = true;
					dataUserInput.RotationInTFT -= ROUND / dataUserInput.IndexRotation;
			}
	}
}