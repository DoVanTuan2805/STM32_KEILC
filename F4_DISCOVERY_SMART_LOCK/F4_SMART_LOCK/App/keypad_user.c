#include "keypad_user.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "lcd_user.h"
#include "ds1307.h"
#include "sd_user.h"
#define NUM_PASSWORD 3
#define SIZE_PASSWORD 4

char passWord[NUM_PASSWORD][SIZE_PASSWORD] =
{
	{1, 2, 2, 4},			// ID : 0
	{1, 2, 3, 4}, 		// ID : 1
	{1, 2, 4, 4}			// ID : 2
};

uint64_t timeWaitShowMainLcd;

static volatile uint8_t index;
volatile uint8_t press;
volatile uint8_t statePressPrev;
volatile uint8_t indexPassword = 0;
char buffPassword[SIZE_PASSWORD];

extern uint64_t timeWaitLogin;							// from file main.c
extern uint64_t timeWaitCheckInOut;					// from file main.c
extern bool stateLogin;								// from file main.c
extern uint8_t stateCheckInOut;				// from file main.c

extern uint8_t stateLcd;						// from file lcd_user.c
volatile bool completePassWord;
volatile bool correctPassWord = false;

extern char timeDs1307[DS1307_MINIMAL_BUFFER_LENGTH];			// from file main.c
char dataCheck[DS1307_MINIMAL_BUFFER_LENGTH + 22];

char Id[4];
	
char *correctCheckOut 	= "   CORRECT CHECK OUT\n";
char *incorrectCheckOut = " INCORRECT CHECK OUT\n";

char *correctCheckIn 		= "  CORRECT CHECK IN \n";
char *incorrectCheckIn 	= " INCORRECT CHECK IN \n";
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
					timeWaitLogin = HAL_GetTick();
					timeWaitCheckInOut = HAL_GetTick();
					if(stateLogin == true)
					{
						if (stateCheckInOut != 0)
						{
							getPassword();
						}
					}
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
							buffPassword[indexPassword] = (char)keyPadHandle();
							indexPassword++;
							completePassWord = false;
						}
						else 
						{
							completePassWord = true;
							indexPassword = 0;
						}
						press = 0;
						statePressPrev = press;
						timeDebounce  = HAL_GetTick();
				}
		}
}

bool handlePassword()
{
		bool result;
		static uint8_t i;
		if(completePassWord == true)
		{
				for( i = 0 ; i <  NUM_PASSWORD; i++ )
				{
						result = memcmp(passWord[i],buffPassword, SIZE_PASSWORD);
						if(result == 0)	
						{
							//memset(buffPassword , '\0', sizeof(buffPassword));
							clearLCD();
							timeWaitShowMainLcd = HAL_GetTick();
							break;
						}
						else
						{
							clearLCD();
							timeWaitShowMainLcd = HAL_GetTick();
						}
				}
				correctPassWord = result == 0 ? true : false;
		}
		memset(dataCheck, '\0', sizeof(dataCheck));
		getDateTime((char*)timeDs1307, DS1307_MINIMAL_BUFFER_LENGTH);
		memcpy(dataCheck, timeDs1307, strlen(timeDs1307));
		if(correctPassWord == true)
		{
				sprintf(Id, " %2d", i);
				strcat(dataCheck, Id);
				if(stateCheckInOut == 1)
				{
						strcat(dataCheck, correctCheckIn);
				}
				else if (stateCheckInOut == 2)
				{
						strcat(dataCheck, correctCheckOut);
				}
				HAL_UART_Transmit(&huart3, (uint8_t*)dataCheck, strlen(dataCheck), HAL_MAX_DELAY);
				writeTimeToSD(dataCheck);
				
		}
		else if (correctPassWord == false)
		{
				strcat(dataCheck, " xx");
				if(stateCheckInOut == 1)
				{
						strcat(dataCheck, incorrectCheckIn);
				}
				else if (stateCheckInOut == 2)
				{
						strcat(dataCheck, incorrectCheckOut);
				}
				HAL_UART_Transmit(&huart3, (uint8_t*)dataCheck, strlen(dataCheck), HAL_MAX_DELAY);
				writeTimeToSD(dataCheck);
		}
		
		return correctPassWord;
}



