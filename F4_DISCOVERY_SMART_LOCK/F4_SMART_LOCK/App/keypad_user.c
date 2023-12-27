#include "keypad_user.h"
#include "usart.h"
#include "string.h"
#define SIZE_PASSWORD 4

static volatile uint8_t index;
volatile uint8_t press;
volatile uint8_t statePressPrev;
volatile uint8_t indexPassword = 0;
char buffPassword[SIZE_PASSWORD];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim3.Instance)
	{
		KEYPAD_Scan(0);
		
		if(index < KEYS)
		{		
				if(u8_KeyStatesArr[index] == KEY_PRESSED)
				{
					press = 1;
					getPassword();
				}
				index++;
				
		}
		else 
		{
				
				index = 0;
				press = 0;
		}
		
	}
}
void initKeyPad(void)
{
		statePressPrev = press;
		KEYPAD_Init(0, u8_KeyStatesArr);
		HAL_TIM_Base_Start_IT(&htim3);		// scan key via timer interrupt
}
char keyPadHandle()
{
		if(u8_KeyStatesArr[KEY_0] == KEY_PRESSED)
		{
				return key[0];
		}
		else if(u8_KeyStatesArr[KEY_1] == KEY_PRESSED)
		{
				return key[1];
		}
		else if(u8_KeyStatesArr[KEY_2] == KEY_PRESSED)
		{
				return key[2];
		}
		else if(u8_KeyStatesArr[KEY_3] == KEY_PRESSED)
		{
				return key[3];
		}
		else if(u8_KeyStatesArr[KEY_4] == KEY_PRESSED)
		{
				return key[4];
		}
		else if(u8_KeyStatesArr[KEY_5] == KEY_PRESSED)
		{
				return key[5];
		}
		else if(u8_KeyStatesArr[KEY_6] == KEY_PRESSED)
		{
				return key[6];
		}
		else if(u8_KeyStatesArr[KEY_7] == KEY_PRESSED)
		{
				return key[7];
		}
		else if(u8_KeyStatesArr[KEY_8] == KEY_PRESSED)
		{
				return key[8];
		}
		else if(u8_KeyStatesArr[KEY_9] == KEY_PRESSED)
		{
				return key[9];
		}
		else if(u8_KeyStatesArr[KEY_A] == KEY_PRESSED)
		{
				return key[10];
		}
		else if(u8_KeyStatesArr[KEY_H] == KEY_PRESSED)
		{
				return key[11];
		}
}
void getPassword()
{
		static uint64_t timeDebounce;
		
		if(statePressPrev != press)
		{
				if(HAL_GetTick() - timeDebounce > 500)
				{
						if(indexPassword < SIZE_PASSWORD)
						{
							buffPassword[indexPassword] = keyPadHandle();
							indexPassword++;
						}
						else 
						{
							indexPassword = 0;
						}
						press = 0;
						statePressPrev = press;
						timeDebounce  = HAL_GetTick();
				}
			
		}
}



