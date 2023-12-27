#include "lcd_user.h"
#include "stdbool.h"
#include "buton_user.h"
#include "keypad_user.h"
CLCD_I2C_Name lcd;
extern I2C_HandleTypeDef hi2c3;
extern bool stateLogin;								// from file main.c
extern uint8_t stateCheckInOut;				// from file main.c
static bool stateLoginPrev;						// file button_user
static uint8_t stateCheckInOutPrev;		// file button_user

extern bool correctPassWord;			// file keypad_user
extern bool completePassWord;

extern uint8_t indexPassword;
//bool ArrayScreen( Screen_t screen)
//{
//	if(screen < NUM_OF_SCREEN)
//	{
//			return handleScreen[screen];
//	}
//	else 
//	{
//			return false;
//	}
//}

void lcdHanle()
{
		handlePassword();
		if (completePassWord == false)		// 
		{
				if (stateCheckInOutPrev != stateCheckInOut)
				{
						stateCheckInOutPrev = stateCheckInOut;
						goto SHOW_CHECK_IN_OUT;
				}
				if (stateLoginPrev != stateLogin)
				{	
						stateLoginPrev = stateLogin;
					
						if(stateLogin == false)
						{
								//ArrayScreen(SCREEN_NON_LOGIN);
								lcdNonLogin();
						}
						else if (stateLogin == true)
						{
							SHOW_CHECK_IN_OUT:
								if(stateCheckInOut == 0)
								{
										//ArrayScreen(SCREEN_CHECK_INOUT);
									lcdCheckInOut();
								}
								else if (stateCheckInOut == 1)			// CHECK IN
								{
									//ArrayScreen(SCREEN_WAIT_CHECKIN);
									waitCheckIn();
								}
								else if (stateCheckInOut == 2)		// CHECK OUT
								{
									//ArrayScreen(SCREEN_WAIT_CHECKOUT);
									waitCheckOut();
								}
						}
				}
				enteringPassword();
		}
		else if (completePassWord == true)
		{
				if(correctPassWord == true)
				{
					checkInOutComplete();
				}
				else if(correctPassWord == false){
					checkInOutError();
				}
		}
}

void lcdInit(void)
{
		CLCD_I2C_Init(&lcd, &hi2c3,  0X4E, 16, 2);
		CLCD_I2C_Clear(&lcd);
		CLCD_I2C_SetCursor(&lcd, 0, 0);
//		CLCD_I2C_WriteString(&lcd, "PLS CHOOSE CHECK ");
//		CLCD_I2C_SetCursor(&lcd, 0, 1);
//		CLCD_I2C_WriteString(&lcd, "     IN/OUT      ");
		stateCheckInOutPrev = stateCheckInOut;
		stateLoginPrev = true;
}

void lcdNonLogin()
{
		CLCD_I2C_SetCursor(&lcd, 0, 0);
		CLCD_I2C_WriteString(&lcd, "PLS CHOOSE LOGIN ");
		CLCD_I2C_SetCursor(&lcd, 0, 1);
		CLCD_I2C_WriteString(&lcd, "                 ");
}
void lcdCheckInOut()
{
		CLCD_I2C_SetCursor(&lcd, 0, 0);
		CLCD_I2C_WriteString(&lcd, " PLEASE LOGIN :  ");
		CLCD_I2C_SetCursor(&lcd, 0, 1);
		CLCD_I2C_WriteString(&lcd, "  FINGER / PASS  ");
}
void waitCheckIn()
{
		CLCD_I2C_SetCursor(&lcd, 0, 0);
		CLCD_I2C_WriteString(&lcd, "PROCESS CHECKIN ");
		CLCD_I2C_SetCursor(&lcd, 0, 1);
		CLCD_I2C_WriteString(&lcd, "  FINGER / PASS  ");
}
void waitCheckOut()
{
		CLCD_I2C_SetCursor(&lcd, 0, 0);
		CLCD_I2C_WriteString(&lcd, "PROCESS CHECKOUT");
		CLCD_I2C_SetCursor(&lcd, 0, 1);
		CLCD_I2C_WriteString(&lcd, "  FINGER / PASS  ");
}
void checkInOutComplete()
{
		if(stateCheckInOut == 1)
		{
				CLCD_I2C_SetCursor(&lcd, 0, 0);
				CLCD_I2C_WriteString(&lcd, "COMPLETE IN ");

		} 
		else if(stateCheckInOut == 2 )
		{
				CLCD_I2C_SetCursor(&lcd, 0, 0);
				CLCD_I2C_WriteString(&lcd, "COMPLETE OUT");
		}
}
void checkInOutError()
{
	if(stateCheckInOut == 1 )
		{
				CLCD_I2C_Clear(&lcd);
				CLCD_I2C_SetCursor(&lcd, 0, 0);
				CLCD_I2C_WriteString(&lcd, "ERROR CHECKIN   ");
		} 
		else if(stateCheckInOut == 2 )
		{
				CLCD_I2C_SetCursor(&lcd, 0, 0);
				CLCD_I2C_WriteString(&lcd, "ERROR CHECKOUT  ");
		}
}
void enteringPassword()
{
		static uint8_t indexPasswordPrev;
		if(indexPassword != 0)
		{
				if (indexPasswordPrev != indexPassword)
				{
						clearLCD();
						indexPasswordPrev = indexPassword;
						if(stateCheckInOut == 1)
						{
								CLCD_I2C_SetCursor(&lcd, 0, 0);
								CLCD_I2C_WriteString(&lcd, "PROCESS CHECKIN");
						}
						else if(stateCheckInOut == 2)
						{
								CLCD_I2C_SetCursor(&lcd, 0, 0);
								CLCD_I2C_WriteString(&lcd, "PROCESS CHECKOUT");
						}
						CLCD_I2C_SetCursor(&lcd, 0, 1);
						if(indexPassword == 1)
						{
							CLCD_I2C_WriteString(&lcd, "*               ");
						}
						else if (indexPassword == 2)
						{
							CLCD_I2C_WriteString(&lcd, "**              ");
						}
						else if (indexPassword == 3)
						{
							CLCD_I2C_WriteString(&lcd, "***             ");
						}
						else if (indexPassword == 4)
						{
							CLCD_I2C_WriteString(&lcd, "****            ");
						}
				}
		}
}
void clearLCD()
{
	CLCD_I2C_Clear(&lcd);
}