#include "LCD_user.h"

bool ArrayScreen( Screen_t screen, int a, int b)
{
	if(screen < NUM_OF_SCREEN)
	{
			return handleScreen[screen](a, b);
	}
	else 
	{
			return false;
	}
}
bool screen1(int a, int b)
{}
bool screen2(int a, int b)
{}
bool screen3(int a, int b)
{}
bool screen4(int a, int b)
{}

