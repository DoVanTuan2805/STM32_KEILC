#include "lcd_user.h"
#include "stdbool.h"
#include "buton_user.h"
CLCD_I2C_Name lcd;
extern I2C_HandleTypeDef hi2c3;
extern bool stateLogin;
extern uint8_t stateCheckInOut;
static bool stateLoginPrev;
static uint8_t stateCheckInOutPrev;

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
				else 
				{
					SHOW_CHECK_IN_OUT:
						if(stateCheckInOut == 0)
						{
								//ArrayScreen(SCREEN_CHECK_INOUT);
							lcdCheckInOut();
						}
						else if (stateCheckInOut == 1)
						{
							//ArrayScreen(SCREEN_WAIT_CHECKIN);
							waitCheckIn();
						}
						else if (stateCheckInOut == 2)
						{
							//ArrayScreen(SCREEN_WAIT_CHECKOUT);
							waitCheckOut();
						}
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
		CLCD_I2C_WriteString(&lcd, "PLEASE LOGIN :   ");
		CLCD_I2C_SetCursor(&lcd, 0, 1);
		CLCD_I2C_WriteString(&lcd, "  FINGER / PASS  ");
}
void waitCheckIn()
{
		CLCD_I2C_SetCursor(&lcd, 0, 0);
		CLCD_I2C_WriteString(&lcd, "PROCESS CHECK IN ");
		CLCD_I2C_SetCursor(&lcd, 0, 1);
		CLCD_I2C_WriteString(&lcd, "  FINGER / PASS  ");
}
void waitCheckOut()
{
		CLCD_I2C_SetCursor(&lcd, 0, 0);
		CLCD_I2C_WriteString(&lcd, "PROCESS CHECK OUT");
		CLCD_I2C_SetCursor(&lcd, 0, 1);
		CLCD_I2C_WriteString(&lcd, "  FINGER / PASS  ");
}
