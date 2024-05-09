#include "view_manager.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
void screen_init()
{
	ILI9341_Init();
	ILI9341_Fill_Screen(BLACK);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
}
void screen_clear()
{
	ILI9341_Fill_Screen(BLACK);
	//ILI9341_Draw_Filled_Rectangle_Coord(5, 10, ILI9341_SCREEN_WIDTH - 20, ILI9341_SCREEN_HEIGHT - 20, BLACK);
}
   