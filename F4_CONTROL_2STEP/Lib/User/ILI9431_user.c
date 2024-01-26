#include "ILI9431_user.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
float sx, sy;    // Saved H, M, S x & y multipliers
float sdeg;
uint16_t osx = 120, osy = 121 ;  // Saved H, M, S x & y coords
uint16_t x0, x1, yy0, yy1;

uint16_t angleInput;
extern Screen_t screen_s;
static volatile Screen_t prevScreen;

extern bool changeEncoder1 ,changeEncoder2;
extern EncoderNew_t encoderCurr;
EncoderNew_t encoderPrev;

char dataText[10];
/*	BEGIN DEDFINE SCREEN MANUAL	*/
#define Allign 10
#define HIGHT_ONE_SLOT	58
#define WIDTH_ONE_SLOT	104

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
void drawCircleAngle(dataUser_t dataUser)
{		
		
		if(encoderPrev != encoderCurr)
		{
				clearLCD();
				encoderPrev = encoderCurr;
		}
		ILI9341_Draw_Hollow_Circle(120, 120 , 118, GREEN);
		ILI9341_Draw_Hollow_Circle(120, 120, 110, WHITE);
		if(changeEncoder1 == true)
		{
				changeEncoder1 = false;
				
				sprintf(dataText, "Góc : %d", dataUser.AngleTFT);
			
				angleInput = dataUser.numAngle;
				sdeg = dataUser.AngleTFT * angleInput;
			
				for(float i = 0; i<360; i+= angleInput) {
					sx = cos((i-90)*0.0174532925);
					sy = sin((i-90)*0.0174532925);
					x0 = sx*114+120;
					yy0 = sy*114+120;
					x1 = sx*100+120;
					yy1 = sy*100+120;
					ILI9341_Draw_Line(x0, yy0, x1, yy1 , WHITE);
				}
		}
		else if (changeEncoder2 == true)
		{
				changeEncoder2 = false;
			
				angleInput = dataUser.OneRound;
				sdeg = dataUser.encoder2;
			
				for(float i = 0; i<360; i+= 45) {
					sx = cos((i-90)*0.0174532925);
					sy = sin((i-90)*0.0174532925);
					x0 = sx*114+120;
					yy0 = sy*114+120;
					x1 = sx*100+120;
					yy1 = sy*100+120;
					ILI9341_Draw_Line(x0, yy0, x1, yy1 , WHITE);
				}
				for(int i = 0; i<360; i+= 10) {				// 60 dot
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
		}
		ILI9341_Draw_Hollow_Circle(120, 121, 3, WHITE);
}
void drawLineInCircle(dataUser_t dataUser)
{
		drawCircleAngle(dataUser);
	
		sx = cos( (sdeg - 90)* 0.0174532925);    
		sy = sin( (sdeg - 90)* 0.0174532925);

		ILI9341_Draw_Line(osx, osy, 120, 121, BLACK);
		osx = sx * 90 + 121;    
		osy = sy * 90 + 121;
		ILI9341_Draw_Line(osx, osy, 120, 121, WHITE);
		ILI9341_Draw_Hollow_Circle(120, 121, 3, RED);
		
		ILI9341_Draw_Filled_Rectangle_Coord(100, 141, 160, 160 ,BLACK );
		ILI9341_Draw_Text(dataText,100, 141, RED, BLACK , Font1);
		
}
void drawLayout(dataUser_t dataUser)
{
		if(dataUser.page > 3 || dataUser.page == 0)
		{
			return;
		}
		
		clearLCD();
		char label[7];
		uint8_t slotInPage = 1;
		
		
		for(int k = 1; k <= 4; k++)
		{
			for	(int f = 1; f <= 3; f++)
			{
					ILI9341_Draw_Hollow_Rectangle_Coord((f-1)*WIDTH_ONE_SLOT, (k-1)*HIGHT_ONE_SLOT, f *WIDTH_ONE_SLOT, k *HIGHT_ONE_SLOT, GREEN);
			}
		}
		
		slotInPage = ((dataUser.page * 12) - 12);
		
		
		for(int i = 1; i <= SLOT_IN_PAGE / 3; i++)				// 4
		{
				for(int j = 1; j <= SLOT_IN_PAGE / 4; j++)			// 3
				{
						slotInPage++;
						sprintf(label, "Góc %d", slotInPage);
						ILI9341_Draw_Text(label, X_TEXT + ((j-1) * WIDTH_ONE_SLOT - 5), Y_TEXT + (i - 1) * HIGHT_ONE_SLOT, RED , BLACK, Font1);
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
void BlynkSlot(uint8_t slot)
{
		static uint64_t timeBlynk;
		static bool state;
		if(HAL_GetTick() - timeBlynk >= 500)
		{
				if(state == true)
				{
						state = false;
						if(slot < 0)		return;
						else if(slot <= 3)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord((slot - 1)*WIDTH_ONE_SLOT , 0*HIGHT_ONE_SLOT, slot * WIDTH_ONE_SLOT, HIGHT_ONE_SLOT, RED);
						}
						else if(slot <= 6)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(((slot - 3) - 1)*WIDTH_ONE_SLOT, 1*HIGHT_ONE_SLOT, (slot - 3) * WIDTH_ONE_SLOT, 2*HIGHT_ONE_SLOT, RED);
						}
						else if(slot <= 9)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(((slot - 6) - 1)*WIDTH_ONE_SLOT, 2*HIGHT_ONE_SLOT, (slot - 6) * WIDTH_ONE_SLOT, 3*HIGHT_ONE_SLOT, RED);
						}
						else if(slot <= 12)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(((slot - 9) - 1)*WIDTH_ONE_SLOT, 3*HIGHT_ONE_SLOT, (slot - 9) * WIDTH_ONE_SLOT, 4*HIGHT_ONE_SLOT, RED);
						}
				}
				else
				{
						state = true;
						if(slot < 0)		return;
						else if(slot <= 3)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord((slot - 1)*WIDTH_ONE_SLOT , 0*HIGHT_ONE_SLOT, slot * WIDTH_ONE_SLOT, HIGHT_ONE_SLOT, GREEN);
						}
						else if(slot <= 6)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(((slot - 3) - 1)*WIDTH_ONE_SLOT, 1*HIGHT_ONE_SLOT, (slot - 3) * WIDTH_ONE_SLOT, 2*HIGHT_ONE_SLOT, GREEN);
						}
						else if(slot <= 9)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(((slot - 6) - 1)*WIDTH_ONE_SLOT, 2*HIGHT_ONE_SLOT, (slot - 6) * WIDTH_ONE_SLOT, 3*HIGHT_ONE_SLOT, GREEN);
						}
						else if(slot <= 12)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(((slot - 9) - 1)*WIDTH_ONE_SLOT, 3*HIGHT_ONE_SLOT, (slot - 9) * WIDTH_ONE_SLOT, 4*HIGHT_ONE_SLOT, GREEN);
						}
				}
				timeBlynk = HAL_GetTick();
		}
}
void clearOneSlot(uint8_t slot)		// 1
{
		if(slot < 0)		return;
		else if(slot <= 3)
		{
				ILI9341_Draw_Filled_Rectangle_Coord((slot - 1)*WIDTH_ONE_SLOT + 5 , 0*HIGHT_ONE_SLOT +5, slot * WIDTH_ONE_SLOT, HIGHT_ONE_SLOT, BLACK);
		}
		else if(slot <= 6)
		{
				ILI9341_Draw_Filled_Rectangle_Coord(((slot - 3) - 1)*WIDTH_ONE_SLOT + 5, 1*HIGHT_ONE_SLOT + 5, (slot - 3) * WIDTH_ONE_SLOT, 2*HIGHT_ONE_SLOT, BLACK);
		}
		else if(slot <= 9)
		{
				ILI9341_Draw_Filled_Rectangle_Coord(((slot - 6) - 1)*WIDTH_ONE_SLOT + 5, 2*HIGHT_ONE_SLOT + 5, (slot - 6) * WIDTH_ONE_SLOT, 3*HIGHT_ONE_SLOT, BLACK);
		}
		else if(slot <= 12)
		{
				ILI9341_Draw_Filled_Rectangle_Coord(((slot - 9) - 1)*WIDTH_ONE_SLOT + 5, 3*HIGHT_ONE_SLOT + 5, (slot - 9) * WIDTH_ONE_SLOT, 4*HIGHT_ONE_SLOT, BLACK);
		}
		//ILI9341_Draw_Hollow_Rectangle_Coord((f-1)*WIDTH_ONE_SLOT, (k-1)*HIGHT_ONE_SLOT, f *WIDTH_ONE_SLOT, k *HIGHT_ONE_SLOT, GREEN);
}