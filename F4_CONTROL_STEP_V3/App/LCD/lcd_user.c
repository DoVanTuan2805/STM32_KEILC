#include "lcd_user.h"

#include "math.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "mainTFT.h"

#include "config.h"

Screen_t screen_state;

void lcd_init()
{
	ILI9341_Init();
	ILI9341_Fill_Screen(BLACK);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	screen_encoder_step(user_data);
//	ILI9341_Draw_Image((const char *)layoutMain, SCREEN_HORIZONTAL_2);
}

void screen_encoder_step(user_data_t user_data)
{
	float sx, sy; // Saved H, M, S x & y multipliers
	float sdeg;
	uint16_t osx = 120, osy = 121; // Saved H, M, S x & y coords
	uint16_t x0, x1, yy0, yy1;

	ILI9341_Draw_Hollow_Circle(218, 143, 93, WHITE); // 118 -> 100   / 120 -> 200


	ILI9341_Draw_Filled_Rectangle_Coord(205, 10, 235, 30, BLACK);


	float angleInput = CIRCLE_RAD / 32; // -> 360 / 32 = 11.25

	sdeg = 2;

	for (float i = 0; i < 360; i += angleInput)
	{
		sx = cos((i - 90) * 0.0174532925);
		sy = sin((i - 90) * 0.0174532925);
		x0 = sx * 90 + 218;	 // 114
		yy0 = sy * 90 + 143; // 114
		x1 = sx * 86 + 218;	 // 100
		yy1 = sy * 86 + 143; // 100
		ILI9341_Draw_Line(x0, yy0, x1, yy1, RED);
	}

	sx = cos((sdeg - 90) * 0.0174532925);
	sy = sin((sdeg - 90) * 0.0174532925);


	ILI9341_Draw_Filled_Circle(osx, osy, 3, BLACK);

	osx = sx * 78 + 218; // 	Y : 200 -> position X CIRCLE
	osy = sy * 78 + 143; //	Y : 140 -> position Y CIRCLE

	ILI9341_Draw_Filled_Circle(osx, osy, 3, RED);
}

void screen_cheater(user_data_t user_data)
{
}

void screen_floor_hand(user_data_t user_data)
{
}

void screen_angle_hand(user_data_t user_data)
{
}

void screen_setting(user_data_t user_data)
{
	
}

void (*handleScreen[NUM_OF_SCREEN])(user_data_t user_data) =
	{
		[SCREEN_ENCOER_STEP] = screen_encoder_step,
		[SCREEN_CHEATER] = screen_cheater,
		[SCREEN_FLOOR_HAND] = screen_floor_hand,
		[SCREEN_ANGLE_HAND] = screen_angle_hand,
		[SCREEN_SETTING] = screen_setting,
};

uint8_t ArrayTFT(Screen_t screen, user_data_t user_data)
{
	if (screen < NUM_OF_SCREEN)
	{
		handleScreen[screen](user_data);
		return 1;
	}
	else
	{
		return 0;
	}
}
