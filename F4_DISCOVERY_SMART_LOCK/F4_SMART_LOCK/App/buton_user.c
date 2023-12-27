#include "buton_user.h"
#include "lcd_user.h"
#include "main.h"
Button_t bt1, bt2, bt3, detectFinger;
uint8_t fingerId, confidence; 
uint16_t templateCount;

extern uint8_t stateCheckInOut;			// from file main.c
extern bool stateLogin;							// from file main.c
extern uint64_t timeWaitLogin;			// from file main.c
extern uint64_t timeWaitCheckInOut;	// from file main.c

void initButton(void)
{
		Button_Init(&bt1, BT1_GPIO_Port, BT1_Pin);
		Button_Init(&bt2, BT2_GPIO_Port, BT2_Pin);
		Button_Init(&bt3, BT3_GPIO_Port, BT3_Pin);
		Button_Init(&detectFinger, FINGER_DETECT_GPIO_Port, FINGER_DETECT_Pin);
}
void handleButton(void)
{
		bt_handle(&bt1);
		bt_handle(&bt2);
		bt_handle(&bt3);
		bt_handle(&detectFinger);
		if(HAL_GetTick() - timeWaitLogin > 5000)
		{
			stateLogin = false;
		}
		if(HAL_GetTick() - timeWaitCheckInOut > 5000)
		{
			stateCheckInOut = 0;
			//stateLogin = false;
		}
}
void bt_press_callback(Button_t *button) {
		if(button == &bt1)
		{
			HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
			timeWaitLogin = HAL_GetTick();
			stateLogin = true;
		}
		else if (button == &bt2)
		{
				HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
				if(stateLogin == true)
				{
					timeWaitLogin = HAL_GetTick();
					timeWaitCheckInOut = HAL_GetTick();
					stateCheckInOut = 1;			// CHECK IN
				}
		}
		else if (button == &bt3)
		{
				HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
				if(stateLogin == true)
				{
					timeWaitLogin = HAL_GetTick();
					timeWaitCheckInOut = HAL_GetTick();
					stateCheckInOut = 2;		// CHECK IN
				}
		}
		else if (button == &detectFinger)
		{
			uint8_t check = fingerHandle();
			//templateCount = get_template_number();
			if(check == FINGERPRINT_OK)
			{
					fingerId = getFingerID();
					confidence = getConfidence();
					//delete_fingerprint(fingerId);
			}
		}
}