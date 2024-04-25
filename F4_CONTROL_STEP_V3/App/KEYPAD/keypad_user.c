#include "keypad_user.h"

#include "tim.h"




char key[16] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'*', '#', 'A', 'B', 'C', 'D'};
	
uint8_t u8_KeyStatesArr[KEYS] = {0};

void keypad_handler()
{
	KEYPAD_Scan(0);
}

void keypad_init()
{
	KEYPAD_Init(0, u8_KeyStatesArr);
	HAL_TIM_Base_Start_IT(&htim11); // scan key via timer interrupt
}

char getKey()
{
	if (u8_KeyStatesArr[KEY_0] == KEY_PRESSED)
	{
		return key[0];
	}
	else if (u8_KeyStatesArr[KEY_1] == KEY_PRESSED)
	{
		return key[1];
	}
	else if (u8_KeyStatesArr[KEY_2] == KEY_PRESSED)
	{
		return key[2];
	}
	else if (u8_KeyStatesArr[KEY_3] == KEY_PRESSED)
	{
		return key[3];
	}
	else if (u8_KeyStatesArr[KEY_4] == KEY_PRESSED)
	{
		return key[4];
	}
	else if (u8_KeyStatesArr[KEY_5] == KEY_PRESSED)
	{
		return key[5];
	}
	else if (u8_KeyStatesArr[KEY_6] == KEY_PRESSED)
	{
		return key[6];
	}
	else if (u8_KeyStatesArr[KEY_7] == KEY_PRESSED)
	{
		return key[7];
	}
	else if (u8_KeyStatesArr[KEY_8] == KEY_PRESSED)
	{
		return key[8];
	}
	else if (u8_KeyStatesArr[KEY_9] == KEY_PRESSED)
	{
		return key[9];
	}
	else if (u8_KeyStatesArr[KEY_A] == KEY_PRESSED)
	{
		return key[10];
	}
	else if (u8_KeyStatesArr[KEY_H] == KEY_PRESSED)
	{
		return key[11];
	}
	else if (u8_KeyStatesArr[KEY_F1] == KEY_PRESSED)
	{
		return key[12];
	}
	else if (u8_KeyStatesArr[KEY_F2] == KEY_PRESSED)
	{
		return key[13];
	}
	else if (u8_KeyStatesArr[KEY_F3] == KEY_PRESSED)
	{
		return key[14];
	}
	else if (u8_KeyStatesArr[KEY_F4] == KEY_PRESSED)
	{
		return key[15];
	}
	else
	{
		return 'x';
	}
}
