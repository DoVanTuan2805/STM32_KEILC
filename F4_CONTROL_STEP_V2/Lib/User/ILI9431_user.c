#include "ILI9431_user.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
float sx, sy;    // Saved H, M, S x & y multipliers
float sdeg;
uint16_t osx = 120, osy = 121 ;  // Saved H, M, S x & y coords
uint16_t x0, x1, yy0, yy1;

float angleInput;

extern Screen_t screenCurr;
static volatile Screen_t prevScreen;

extern bool changeEncoder1 ,changeEncoder2;

char dataText[10];
/*	BEGIN DEDFINE SCREEN MANUAL	*/
#define Allign 10
#define HIGHT_ONE_SLOT	55
#define WIDTH_ONE_SLOT	300

#define SLOT_IN_PAGE 12
#define X_TEXT 15
#define Y_TEXT 20
#define SIZE_TEXT 1.5
/*	END DEDFINE SCREEN MANUAL	*/
#define NUM_OF_SCREEN 5
void (*handleScreen[NUM_OF_SCREEN])( dataUser_t dataUser) =
{ 
		[ENCODER_ANGLE_SCREEN] = encoderAngle, 
		[HAND_ANGLE_SCREEN] = handAngle, 
		[ENCODER_ROTATION_SCREEN] = encoderRotation, 
		[HAND_ROTATION_SCREEN] = handRotation, 
		[SETUP_SCREEN] = setupParameter 
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
void encoderAngle(dataUser_t dataUser)
{
		sdeg = dataUser.AngleInTFT;		
		ILI9341_Draw_Hollow_Circle(120, 120 , 118, GREEN);
		ILI9341_Draw_Hollow_Circle(120, 120, 110, WHITE);
		for(float i = 0; i<360; i+= 45) 
		{
			sx = cos((i-90)*0.0174532925);
			sy = sin((i-90)*0.0174532925);
			x0 = sx*114+120;
			yy0 = sy*114+120;
			x1 = sx*100+120;
			yy1 = sy*100+120;
			ILI9341_Draw_Line(x0, yy0, x1, yy1 , WHITE);
		}
		for(int i = 0; i<360; i+= 10) // 60 dot
		{				
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
	
		sx = cos( (sdeg - 90)* 0.0174532925);    
		sy = sin( (sdeg - 90)* 0.0174532925);

		ILI9341_Draw_Line(osx, osy, 120, 121, BLACK);
		osx = sx * 90 + 121;    
		osy = sy * 90 + 121;
		ILI9341_Draw_Line(osx, osy, 120, 121, WHITE);
		ILI9341_Draw_Hollow_Circle(120, 121, 3, RED);
		
		ILI9341_Draw_Filled_Rectangle_Coord(100, 141, 160, 160 ,BLACK );
		//ILI9341_Draw_Text(dataText,100, 141, RED, BLACK , Font1);
		
}
void encoderRotation(dataUser_t dataUser)
{
		ILI9341_Draw_Hollow_Circle(120, 120 , 118, GREEN);
		ILI9341_Draw_Hollow_Circle(120, 120, 110, WHITE);
	
		sprintf(dataText, "Góc : %d", (dataUser.RotationInTFT / (ROUND / dataUser.IndexRotation)) );
	
		angleInput = ROUND / dataUser.IndexRotation;			// -> 360 / 32 = 11.25
	
		sdeg = dataUser.RotationInTFT;
			
		for(float i = 0; i<360; i+= angleInput) 
		{
				sx = cos((i-90)*0.0174532925);
				sy = sin((i-90)*0.0174532925);
				x0 = sx*114+120;
				yy0 = sy*114+120;
				x1 = sx*100+120;
				yy1 = sy*100+120;
				ILI9341_Draw_Line(x0, yy0, x1, yy1 , WHITE);
		}
		
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
void handAngle(dataUser_t dataUser)
{
		pageInAngle(dataUser);
		for(int i = 1; i <= 4; i++)
		{
				ILI9341_Draw_Hollow_Rectangle_Coord(
									Allign, 
									Allign + ((i - 1) * HIGHT_ONE_SLOT), 
									Allign + WIDTH_ONE_SLOT, 
									Allign + i * HIGHT_ONE_SLOT ,
									GREEN );
		}
		ILI9341_Draw_Hollow_Rectangle_Coord(Allign, Allign, 310, 230, GREEN);
}
void handRotation(dataUser_t dataUser)
{
		pageInRotation(dataUser);
		for(int i = 1; i <= 4; i++)
		{
				ILI9341_Draw_Hollow_Rectangle_Coord(
									Allign, 
									Allign + ((i - 1) * HIGHT_ONE_SLOT), 
									Allign + WIDTH_ONE_SLOT, 
									Allign + i * HIGHT_ONE_SLOT ,
									GREEN );
		}
		ILI9341_Draw_Hollow_Rectangle_Coord(Allign, Allign, 310, 230, GREEN);
}
void setupParameter(dataUser_t dataUser)
{
		pageInSetup(dataUser);
		for(int i = 1; i <= 4; i++)
		{
				ILI9341_Draw_Hollow_Rectangle_Coord(
									Allign, 
									Allign + ((i - 1) * HIGHT_ONE_SLOT), 
									Allign + WIDTH_ONE_SLOT, 
									Allign + i * HIGHT_ONE_SLOT ,
									GREEN );
		}
		ILI9341_Draw_Hollow_Rectangle_Coord(Allign, Allign, 310, 230, GREEN);
}

void pageInAngle(dataUser_t dataUser)
{
		char textTitle[30];
		char textAngle[10];
		uint8_t iStart;
		sprintf(textTitle, "ANGLE (A)	 < PAGE : %d >", dataUser.indexPageInAngle);
		ILI9341_Draw_Filled_Rectangle_Coord(50, 25, 250,55 , BLACK);
		ILI9341_Draw_Text(textTitle,50, 30, RED, BLACK , Font1);
	
		//ILI9341_Draw_Text(textTitle,30, 85, RED, BLACK , Font1);
	
		iStart = dataUser.indexPageInAngle * 3 - 3;
 	
		for(int i = 0; i < 3; i++)				// PER 55
		{
				ILI9341_Draw_Filled_Rectangle_Coord(30, (i*55)+80, 200,(i*55)+80 + 30 , BLACK);
				sprintf(textAngle, "ANGLE : %d", iStart+=1);
				ILI9341_Draw_Text(textAngle,30, (i*55)+85, RED, BLACK , Font1);
				memset(textAngle, '\0',sizeof(textAngle));
		}
}
void pageInRotation(dataUser_t dataUser)
{
		char textTitle[30];
		char textRotation[15];
		uint8_t iStart;
		sprintf(textTitle, "ROTATION (B)  < PAGE : %d >", dataUser.indexPageInRotation);
		ILI9341_Draw_Filled_Rectangle_Coord(50, 25, 250, 55 , BLACK);
		ILI9341_Draw_Text(textTitle,50, 30, RED, BLACK , Font1);
	
		//ILI9341_Draw_Text(textTitle,30, 85, RED, BLACK , Font1);
	
		iStart = dataUser.indexPageInRotation * 3 - 3;
 	
		for(int i = 0; i < 3; i++)				// PER 55
		{
				ILI9341_Draw_Filled_Rectangle_Coord(30, (i*55)+80, 200,(i*55)+80 + 30 , BLACK);
				sprintf(textRotation, "ROTATION : ");
				ILI9341_Draw_Text(textRotation,30, (i*55)+85, RED, BLACK , Font1);
				memset(textRotation, '\0',sizeof(textRotation));
		}
}
void pageInSetup(dataUser_t dataUser)
{
		char textTitle[50];
		char textSetup[25];
		uint8_t iStart;
		sprintf(textTitle, "      SET UP      < PAGE : %d >", dataUser.indexPageInRotation);
		ILI9341_Draw_Filled_Rectangle_Coord(50, 25, 250, 55 , BLACK);
		ILI9341_Draw_Text(textTitle,50, 30, RED, BLACK , Font1);
	
		//ILI9341_Draw_Text(textTitle,30, 85, RED, BLACK , Font1);
	
		iStart = dataUser.indexPageInRotation * 3 - 3;
 	
		for(int i = 0; i < 3; i++)				// PER 55
		{
				ILI9341_Draw_Filled_Rectangle_Coord(30, (i*55)+80, 200,(i*55)+80 + 30 , BLACK);
				if(dataUser.indexPageInSetup == 1)
				{
						if( i == 0)
						{
								sprintf(textSetup, "SPEED : ");
						}
						else if( i == 1)
						{
								sprintf(textSetup, "PULSE : ");
						}
						else if( i == 2)
						{
								sprintf(textSetup, "HOME ANGLE (A) : ");
						}
				}
				else if(dataUser.indexPageInSetup == 2)
				{
						if( i == 0)
						{
								sprintf(textSetup, "LOOP MODE : ");
						}
						else if( i == 1)
						{
								sprintf(textSetup, "SET HOME ROTATION (B) : ");
						}
						else if( i == 2)
						{
								sprintf(textSetup, "RELEASE STEP (B) : ");
						}
				}
				ILI9341_Draw_Text(textSetup,30, (i*55)+85, RED, BLACK , Font1);
				memset(textSetup, '\0',sizeof(textSetup));
		}
}
void clearLCD(void)
{
		ILI9341_Fill_Screen(BLACK);
}
void BlynkSlot(dataUser_t dataUser )
{
		static uint64_t timeBlynk;
		static bool state;
		
		if(HAL_GetTick() - timeBlynk >= 100)
		{
				if(state == true)
				{
						state = false;
						if(screenCurr == HAND_ANGLE_SCREEN)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(
											Allign, 
											Allign + ((dataUser.indexSlotInAngle) * HIGHT_ONE_SLOT), 
											Allign + WIDTH_ONE_SLOT, 
											Allign + (dataUser.indexSlotInAngle+1) * HIGHT_ONE_SLOT ,
											GREEN );
						}
						else if(screenCurr == HAND_ROTATION_SCREEN)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(
										Allign, 
										Allign + ((dataUser.indexSlotInRotation) * HIGHT_ONE_SLOT), 
										Allign + WIDTH_ONE_SLOT, 
										Allign + (dataUser.indexSlotInRotation + 1) * HIGHT_ONE_SLOT ,
										GREEN );
						}
						else if(screenCurr == SETUP_SCREEN)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(
										Allign, 
										Allign + ((dataUser.indexSlotInSetup) * HIGHT_ONE_SLOT), 
										Allign + WIDTH_ONE_SLOT, 
										Allign + (dataUser.indexSlotInSetup + 1) * HIGHT_ONE_SLOT ,
										GREEN );
						}
				}
				else 
				{
						state = true;
						if(screenCurr == HAND_ANGLE_SCREEN)
						{
							ILI9341_Draw_Hollow_Rectangle_Coord(
										Allign, 
										Allign + ((dataUser.indexSlotInAngle) * HIGHT_ONE_SLOT), 
										Allign + WIDTH_ONE_SLOT, 
										Allign + (dataUser.indexSlotInAngle+1) * HIGHT_ONE_SLOT ,
										RED );
						}
						else if(screenCurr == HAND_ROTATION_SCREEN)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(
										Allign, 
										Allign + ((dataUser.indexSlotInRotation) * HIGHT_ONE_SLOT), 
										Allign + WIDTH_ONE_SLOT, 
										Allign + (dataUser.indexSlotInRotation + 1) * HIGHT_ONE_SLOT ,
										RED );
						}
						else if(screenCurr == SETUP_SCREEN)
						{
								ILI9341_Draw_Hollow_Rectangle_Coord(
										Allign, 
										Allign + ((dataUser.indexSlotInSetup) * HIGHT_ONE_SLOT), 
										Allign + WIDTH_ONE_SLOT, 
										Allign + (dataUser.indexSlotInSetup + 1) * HIGHT_ONE_SLOT ,
										RED );
						}
				}
				/// 2->4
				timeBlynk = HAL_GetTick();
		}
}
void clearOneSlot(uint8_t slot)		// 1
{
}