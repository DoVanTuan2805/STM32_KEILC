#include "ILI9431_user.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
float sx, sy;    // Saved H, M, S x & y multipliers
float sdeg;
uint16_t osx = 120, osy = 121 ;  // Saved H, M, S x & y coords
uint16_t x0, x1, yy0, yy1;

uint8_t angleInput;
/*	BEGIN DEDFINE SCREEN MANUAL	*/
#define Allign 10
#define HIGHT_ONE_SLOT	55
#define WIDTH_ONE_SLOT	100

#define SLOT_IN_PAGE 12
#define X_TEXT 15
#define Y_TEXT 20
#define SIZE_TEXT 1.5
/*	END DEDFINE SCREEN MANUAL	*/
void (*handleScreen[NUM_OF_SCREEN])( dataUser_t dataUser) =
{ 
		[MANUAL_SCREEN] = layoutManual, 
		[AUTO_SCREEN] = drawLineInCircle 
};

bool ArrayTFT(Screen_t screen,  dataUser_t dataUser)
{
	if(screen < NUM_OF_SCREEN)
	{
		handleScreen[screen](dataUser);
	}
}
void initILI(void)
{
		ILI9341_Init();
		ILI9341_Fill_Screen(BLACK);
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
}
void drawCircleAngle(uint8_t angle)
{
		angleInput = angle;
		ILI9341_Draw_Hollow_Circle(120, 120 , 118, GREEN);
		ILI9341_Draw_Hollow_Circle(120, 120, 110, WHITE);
	
		for(float i = 0; i<360; i+= angleInput) {
			sx = cos((i-90)*0.0174532925);
			sy = sin((i-90)*0.0174532925);
			x0 = sx*114+120;
			yy0 = sy*114+120;
			x1 = sx*100+120;
			yy1 = sy*100+120;
			ILI9341_Draw_Line(x0, yy0, x1, yy1 , WHITE);
		}
		/*
	for(int i = 0; i<360; i+= 6) {				// 60 dot
			sx = cos((i-90)*0.0174532925);
			sy = sin((i-90)*0.0174532925);
			x0 = sx*102+120;
			yy0 = sy*102+120;
			// Draw minute markers
			ILI9341_Draw_Pixel(x0, yy0, BLUE);
			
			// Draw main quadrant dots
			if(i==0 || i==180) ILI9341_Draw_Filled_Circle(x0, yy0, 3, BLUE);
			if(i==90 || i==270) ILI9341_Draw_Filled_Circle(x0, yy0, 3, BLUE);
	}
	*/
	ILI9341_Draw_Hollow_Circle(120, 121, 3, WHITE);
}
void drawLineInCircle(dataUser_t dataUser)
{
		sdeg = dataUser.encoder2 * angleInput; 

		sx = cos( (sdeg - 90)* 0.0174532925);    
		sy = sin( (sdeg - 90)* 0.0174532925);

		ILI9341_Draw_Line(osx, osy, 120, 121, BLACK);
		osx = sx * 90 + 121;    
		osy = sy * 90 + 121;
		ILI9341_Draw_Line(osx, osy, 120, 121, WHITE);
		ILI9341_Draw_Hollow_Circle(120, 121, 3, RED);
}
void drawLayout(dataUser_t dataUser)
{
		if(dataUser.page > 3 || dataUser.page == 0)
		{
			return;
		}
		char label[7];
		uint8_t slot = 1;
		ILI9341_Fill_Screen(BLACK);
		
		ILI9341_Draw_Hollow_Rectangle_Coord(Allign, Allign, ILI9341_SCREEN_WIDTH - Allign, ILI9341_SCREEN_HEIGHT - Allign, GREEN);
		ILI9341_Draw_Horizontal_Line(Allign, Allign + HIGHT_ONE_SLOT, ILI9341_SCREEN_WIDTH - 2 * Allign, GREEN);
		ILI9341_Draw_Horizontal_Line(Allign, Allign + 2 * HIGHT_ONE_SLOT, ILI9341_SCREEN_WIDTH - 2 * Allign, GREEN);
		ILI9341_Draw_Horizontal_Line(Allign, Allign + 3 * HIGHT_ONE_SLOT, ILI9341_SCREEN_WIDTH - 2 * Allign, GREEN);
		ILI9341_Draw_Horizontal_Line(Allign, Allign + 4 *HIGHT_ONE_SLOT, ILI9341_SCREEN_WIDTH - 2 * Allign, GREEN);
		ILI9341_Draw_Vertical_Line(Allign + WIDTH_ONE_SLOT, Allign ,ILI9341_SCREEN_HEIGHT - 2 * Allign, GREEN);
		ILI9341_Draw_Vertical_Line(Allign + 2 * WIDTH_ONE_SLOT, Allign ,ILI9341_SCREEN_HEIGHT - 2 * Allign, GREEN);
		
		slot = ((dataUser.page * 12) - 12);
		for(int i = 1; i <= SLOT_IN_PAGE / 3; i++)				// 4
		{
				for(int j = 1; j <= SLOT_IN_PAGE / 4; j++)			// 3
				{
						slot++;
						sprintf(label, "Góc %d", slot);
						ILI9341_Draw_Text(label, X_TEXT + ((j-1) * WIDTH_ONE_SLOT), Y_TEXT + (i - 1) * HIGHT_ONE_SLOT, RED , BLACK, Font1);
						memset(label, '\0', strlen(label));
						
				}
		}
		
}
void layoutManual(dataUser_t dataUser)
{
		drawLayout(dataUser);
}
void clearLCD(void)
{
		ILI9341_Fill_Screen(BLACK);
}
void clearOneLayout(uint8_t layout)
{
}