#include "ILI9431_user.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
float sx, sy; // Saved H, M, S x & y multipliers
float sdeg;
uint16_t osx = 120, osy = 121; // Saved H, M, S x & y coords
uint16_t x0, x1, yy0, yy1;

float angleInput;
extern dataPage_t dataPageW;
extern Screen_t screenCurr;
static volatile Screen_t prevScreen;

extern bool changeEncoder1, changeEncoder2;

/*	BEGIN DEDFINE SCREEN MANUAL	*/
#define Allign 10
#define HIGHT_ONE_SLOT 55
#define WIDTH_ONE_SLOT 300

#define SLOT_IN_PAGE 12
#define X_TEXT 15
#define Y_TEXT 20
#define SIZE_TEXT 1.5
/*	END DEDFINE SCREEN MANUAL	*/

void (*handleScreen[NUM_OF_SCREEN])(dataUser_t dataUser) =
	{
		[FLOOR_MAIN_SCREEN] = fncFloorInMain,
		[ANGLE_MAIN_SCREEN] = fncAngleInMain,
		[CHEATER_SCREEN] = cheater,
		[ENCODER_ROTATION_SCREEN] = encoderRotation,
		[HAND_ROTATION_SCREEN] = handRotation,
		[INPUT_TOTAL_PAGE_SCREEN] = inputTotalPage,
		[INPUT_TOTAL_SLOT_SCREEN] = inputSlotPage,
		[SETUP_SCREEN] = setupParameter,
};

bool ArrayTFT(Screen_t screen, dataUser_t dataUser)
{
	if (screen < NUM_OF_SCREEN)
	{
		handleScreen[screen](dataUser);
		return true;
	}
	else
	{
		return false;
	}
}
void initILI(void)
{
	ILI9341_Init();
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
}

void encoderRotation(dataUser_t dataUser)
{
	char dataText[15];
	// KEYPAD IN SCREEN ENCODER
	{
		if (dataUser.bKeypadEncoder == true)
		{
			ILI9341_Draw_Filled_Rectangle_Coord(210, 30, 260, 65, WHITE);
			sprintf(dataText, "%c%c", dataUser.arrayRotEn[0], dataUser.arrayRotEn[1]);
			ILI9341_Draw_Text(dataText, 210, 30, RED, WHITE, Font1);
		}
		else
		{
			ILI9341_Draw_Filled_Rectangle_Coord(210, 30, 240, 65, WHITE);
			// clearLCD();
		}
	}
	memset(dataText, '\0', sizeof(dataText));
	// ENCODER ROTATIION
	{
		ILI9341_Draw_Hollow_Circle(218, 143, 93, BLACK); // 118 -> 100   / 120 -> 200
		float num = (dataUser.RotationInTFT / ROUND) * dataUser.IndexRotation;
		sprintf(dataText, "%.0f", (dataUser.RotationInTFT / ROUND) * dataUser.IndexRotation);

		ILI9341_Draw_Filled_Rectangle_Coord(205, 10, 235, 30, WHITE);

		ILI9341_Draw_Text(dataText, 210, 12, RED, WHITE, Font1);
		memset(dataText, '\0', sizeof(dataText));

		angleInput = ROUND / dataUser.IndexRotation; // -> 360 / 32 = 11.25

		sdeg = dataUser.RotationInTFT;

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

		// ILI9341_Draw_Line(osx, osy, 120, 121, BLACK);
		ILI9341_Draw_Filled_Circle(osx, osy, 3, WHITE);

		osx = sx * 78 + 218; // 	Y : 200 -> position X CIRCLE
		osy = sy * 78 + 143; //	Y : 140 -> position Y CIRCLE
		// ILI9341_Draw_Line(osx, osy, 120, 121, WHITE);
		ILI9341_Draw_Filled_Circle(osx, osy, 3, RED);

		// ILI9341_Draw_Hollow_Circle(218, 143, 70, RED);
	}
	/* 	INDEX 	*/
	{
		sprintf(dataText, "%d", dataUser.IndexRotation);

		ILI9341_Draw_Filled_Rectangle_Coord(65, 10, 95, 30, WHITE);

		ILI9341_Draw_Text(dataText, 65, 8, BLUE, WHITE, Font1);
		memset(dataText, '\0', sizeof(dataText));
	}
	/*		CHEATER		*/
	{
		ILI9341_Draw_Filled_Rectangle_Coord(78, 30, 108, 53, WHITE);

		sprintf(dataText, "%.1f", dataUser.fCheater);
		if (dataUser.fCheater < -0.1)
		{
			ILI9341_Draw_Line(80, 43, 85, 43, BLUE);
		}

		ILI9341_Draw_Text(dataText, 83, 34, BLUE, WHITE, Font1);
		memset(dataText, '\0', sizeof(dataText));
	}
	/*		TANG		*/
	{
		ILI9341_Draw_Filled_Rectangle_Coord(65, 58, 108, 83, WHITE);
		sprintf(dataText, "%d", dataUser.indexFloorInRotation);
		ILI9341_Draw_Text(dataText, 65, 59, BLUE, WHITE, Font1);
		memset(dataText, '\0', sizeof(dataText));
	}
	/*	ANGLE	NUMBER */
	{
		ILI9341_Draw_Filled_Circle(25, 175, 6, RED); // POINT

		ILI9341_Draw_Filled_Rectangle_Coord(45, 125, 75, 235, WHITE);

		char dataLine[10];
		sprintf(dataLine, "%02d", dataUser.dataAngleInMain.rotation_1);
		ILI9341_Draw_Text(dataLine, 45, 125, PURPLE, WHITE, Font1);
		memset(dataLine, '\0', sizeof(dataLine));

		sprintf(dataLine, "%02d", dataUser.dataAngleInMain.rotation_2);
		ILI9341_Draw_Text(dataLine, 45, 145, PURPLE, WHITE, Font1);

		sprintf(dataLine, "%02d", dataUser.dataAngleInMain.rotation_3);
		ILI9341_Draw_Text(dataLine, 45, 165, RED, WHITE, Font1);
		memset(dataLine, '\0', sizeof(dataLine));

		sprintf(dataLine, "%02d", dataUser.dataAngleInMain.rotation_4);
		ILI9341_Draw_Text(dataLine, 45, 185, PURPLE, WHITE, Font1);
		memset(dataLine, '\0', sizeof(dataLine));

		sprintf(dataLine, "%02d", dataUser.dataAngleInMain.rotation_5);
		ILI9341_Draw_Text(dataLine, 45, 205, PURPLE, WHITE, Font1);
		memset(dataLine, '\0', sizeof(dataLine));
	}
	/*	ANGLE	NUMBER */
}

/*
	SELECT SCREEN
- CREATE NEW ROTATION
- MODIFY OLD ROTATION
*/
void inputTotalPage(dataUser_t dataUser)
{
	char text[30];
	ILI9341_Draw_Hollow_Rectangle_Coord(
		Allign,
		Allign,
		Allign + WIDTH_ONE_SLOT,
		Allign + HIGHT_ONE_SLOT,
		GREEN);
	ILI9341_Draw_Hollow_Rectangle_Coord(
		Allign,
		Allign + HIGHT_ONE_SLOT,
		Allign + WIDTH_ONE_SLOT,
		Allign + 2 * HIGHT_ONE_SLOT,
		GREEN);

	sprintf(text, "CREATE NEW ROTAION");
	ILI9341_Draw_Filled_Rectangle_Coord(50, 25, 250, 55, WHITE);
	ILI9341_Draw_Text(text, 50, 30, RED, WHITE, Font1);
	memset(text, '\0', sizeof(text));

	{
		ILI9341_Draw_Filled_Rectangle_Coord(50, (0 * 55) + 80, 250, (0 * 55) + 80 + 30, WHITE);
		if (dataUser.dataSetupRotation.changeCreatPage == true)
		{
			sprintf(text, "TOTAL PAGE (0 >> 5): %s", dataUser.dataSetupRotation.strTotalPage);
			ILI9341_Draw_Text(text, 50, (0 * 55) + 85, RED, WHITE, Font1);
			memset(text, '\0', sizeof(text));
		}
		else
		{
			sprintf(text, "TOTAL PAGE (0 >> 5): %d", dataPageW.totalPage);
			ILI9341_Draw_Text(text, 50, (0 * 55) + 85, RED, WHITE, Font1);
			memset(text, '\0', sizeof(text));
		}
	}
}
void inputSlotPage(dataUser_t dataUser)
{
}
/*
			CUSTOMIZE ROTATION
- PAGE (1- 10)
- SLOT / PAGE (MAX 100)
*/
void pageInRotation(dataUser_t dataUser)
{
	char textTitle[30];
	char textRotation[15];

	sprintf(textTitle, "          < PAGE : %d >", dataPageW.page + 1);
	// ILI9341_Draw_Filled_Rectangle_Coord(50, 25, 250, 55, WHITE);
	ILI9341_Draw_Text(textTitle, 50, 30, RED, WHITE, Font1);
	memset(textTitle, '0', sizeof(textTitle));

	for (int i = 0; i < 3; i++) // PER IN LINE 55
	{
		uint8_t num;
		ILI9341_Draw_Filled_Rectangle_Coord(30, (i * 55) + 80, 200, (i * 55) + 80 + 30, WHITE);

		if (dataUser.dataSetupRotation.changeCreatPage == true)
		{
			num = (dataUser.dataSetupRotation.indexSlotInRotation) + ((dataUser.dataSetupRotation.indexPageInRotation - 1) * 3);
			sprintf(textRotation, "ROTATION %d : %s", num, dataUser.dataSetupRotation.strRotation);
			ILI9341_Draw_Text(textRotation, 30, ((dataUser.dataSetupRotation.indexSlotInRotation - 1) * 55) + 85, RED, WHITE, Font1);
			memset(textRotation, '\0', sizeof(textRotation));

			uint8_t nums = (i + 1) + ((dataUser.dataSetupRotation.indexPageInRotation - 1) * 3);
			if (nums != num)
			{
				sprintf(textRotation, "ROTATION %d : %d", nums, dataPageW.dataRotation[dataPageW.page][nums]);
				ILI9341_Draw_Text(textRotation, 30, (i * 55) + 85, RED, WHITE, Font1);
				memset(textRotation, '\0', sizeof(textRotation));
			}
		}
		else
		{
			num = (i + 1) + ((dataUser.dataSetupRotation.indexPageInRotation - 1) * 3);
			sprintf(textRotation, "ROTATION %d : %d", num, dataPageW.dataRotation[dataPageW.page][num]);
			ILI9341_Draw_Text(textRotation, 30, (i * 55) + 85, RED, WHITE, Font1);
			memset(textRotation, '\0', sizeof(textRotation));
		}
	}
}
void handRotation(dataUser_t dataUser)
{
	pageInRotation(dataUser);
	for (int i = 1; i <= 4; i++)
	{
		ILI9341_Draw_Hollow_Rectangle_Coord(
			Allign,
			Allign + ((i - 1) * HIGHT_ONE_SLOT),
			Allign + WIDTH_ONE_SLOT,
			Allign + i * HIGHT_ONE_SLOT,
			GREEN);
	}
	ILI9341_Draw_Hollow_Rectangle_Coord(Allign, Allign, 310, 230, GREEN);
}
void setupParameter(dataUser_t dataUser)
{
	pageInSetup(dataUser);
	for (int i = 1; i <= 4; i++)
	{
		ILI9341_Draw_Hollow_Rectangle_Coord(
			Allign,
			Allign + ((i - 1) * HIGHT_ONE_SLOT),
			Allign + WIDTH_ONE_SLOT,
			Allign + i * HIGHT_ONE_SLOT,
			GREEN);
	}
	ILI9341_Draw_Hollow_Rectangle_Coord(Allign, Allign, 310, 230, GREEN);
}

/*
		SET UP
PAGE 1:
- LOOP MODE : 							indexSlotInSetup = 1
- PULSE : 									indexSlotInSetup = 2
- SPEED : 									indexSlotInSetup = 3
PAGE 2:
- HOME ANGLE (A) : 					indexSlotInSetup = 1
- SET HOME ROTATION (B) : 	indexSlotInSetup = 2
- RELEASE STEP (B) : 				indexSlotInSetup = 3
*/
void pageInSetup(dataUser_t dataUser)
{
	char textTitle[50];
	char textSetup[25];
	char dataSetup[10];
	static uint8_t lastPage;
	if (lastPage != dataUser.indexPageInSetup)
	{
		lastPage = dataUser.indexPageInSetup;
		clearLCD();
	}
	sprintf(textTitle, "      SET UP      < PAGE : %d >", dataUser.indexPageInSetup);
	ILI9341_Draw_Filled_Rectangle_Coord(50, 25, 250, 55, WHITE);
	ILI9341_Draw_Text(textTitle, 50, 30, RED, WHITE, Font1);

	// ILI9341_Draw_Text(textTitle,30, 85, RED, BLACK , Font1);

	for (int i = 0; i < 3; i++) // PER 55
	{
		// ILI9341_Draw_Filled_Rectangle_Coord(30, (i * 55) + 80, 200, (i * 55) + 80 + 30, WHITE);
		//  SHOW PAGE SET UP 1
		HAL_Delay(5);
		if (dataUser.indexPageInSetup == 1)
		{
			if (i == 0)
			{
				sprintf(textSetup, "LOOP MODE: ");
			}
			else if (i == 1)
			{
				sprintf(textSetup, "PULSE : ");
			}
			else if (i == 2)
			{
				sprintf(textSetup, "SPEED (0 >>> 99) : ");
			}

			if (dataUser.dataSetup.LoopMode == true)
			{
				sprintf(dataSetup, "ON");
			}
			else if (dataUser.dataSetup.LoopMode == false)
			{
				sprintf(dataSetup, "OFF");
			}
			// SHOW LOOP MODE
			ILI9341_Draw_Filled_Rectangle_Coord(160, 85, 250, 110, WHITE);
			ILI9341_Draw_Text(dataSetup, 160, 85, DARKGREEN, WHITE, Font1);
			memset(dataSetup, '\0', sizeof(dataSetup));

			// SHOW PULSE
			{
				ILI9341_Draw_Filled_Rectangle_Coord(160, 140, 250, 170, WHITE);
				if (dataUser.dataSetup.changePulse == false) // NOT CHANGE PULSE
				{
					sprintf(dataSetup, "%llu", dataUser.dataSetup.pulse);
					ILI9341_Draw_Text(dataSetup, 160, 140, DARKGREEN, WHITE, Font1);
					memset(dataSetup, '\0', sizeof(dataSetup));
				}
				else if (dataUser.dataSetup.changePulse == true) // CHANGING PULSE
				{
					sprintf(dataSetup, "%s", dataUser.dataSetup.arrayPulse);
					ILI9341_Draw_Text(dataSetup, 160, 140, DARKGREEN, WHITE, Font1);
					memset(dataSetup, '\0', sizeof(dataSetup));
				}
			}
			// SHOW SPEED
			{
				ILI9341_Draw_Filled_Rectangle_Coord(160, 195, 250, 225, WHITE);
				if (dataUser.dataSetup.changeSpeed == false) // NOT CHANGE SPEED
				{
					sprintf(dataSetup, "%d", dataUser.dataSetup.speed);
					ILI9341_Draw_Text(dataSetup, 160, 195, DARKGREEN, WHITE, Font1);
					memset(dataSetup, '\0', sizeof(dataSetup));
				}
				else if (dataUser.dataSetup.changeSpeed == true) // CHANGING SPEED
				{
					sprintf(dataSetup, "%s", dataUser.dataSetup.arraySpeed);
					ILI9341_Draw_Text(dataSetup, 160, 195, DARKGREEN, WHITE, Font1);
					memset(dataSetup, '\0', sizeof(dataSetup));
				}
			}
		}
		// SHOW PAGE SET UP 2
		else if (dataUser.indexPageInSetup == 2)
		{
			if (i == 0)
			{
				sprintf(textSetup, "NUM INDEX : ");
			}
			else if (i == 1)
			{
				sprintf(textSetup, "SET HOME ROTATION (B) : ");
			}
			else if (i == 2)
			{
				sprintf(textSetup, "RELEASE STEP (B) : ");
			}
			// SHOW NUM INDEX
			{
				if (dataUser.dataSetup.changeNumIndex == true)
				{
					sprintf(dataSetup, "%c%c", dataUser.dataSetup.arrayNumIndex[0], dataUser.dataSetup.arrayNumIndex[1]);
					ILI9341_Draw_Filled_Rectangle_Coord(160, 85, 250, 110, WHITE);
					ILI9341_Draw_Text(dataSetup, 160, 85, DARKGREEN, WHITE, Font1);
					memset(dataSetup, '\0', sizeof(dataSetup));
				}
				else if (dataUser.dataSetup.changeNumIndex == false)
				{
					sprintf(dataSetup, "%d", dataUser.dataSetup.numIndex);
					ILI9341_Draw_Filled_Rectangle_Coord(160, 85, 250, 110, WHITE);
					ILI9341_Draw_Text(dataSetup, 160, 85, DARKGREEN, WHITE, Font1);
					memset(dataSetup, '\0', sizeof(dataSetup));
				}
			}
		}
		ILI9341_Draw_Text(textSetup, 30, (i * 55) + 85, RED, WHITE, Font1);
		memset(textSetup, '\0', sizeof(textSetup));
	}
}
void cheater(dataUser_t dataUser)
{
	encoderRotation(dataUser);
	ILI9341_Draw_Hollow_Rectangle_Coord(5, 28, 110, 55, GREEN);
}
/*	CHINH TANG O MAN HINH CHINH */
void fncFloorInMain(dataUser_t dataUser)
{
	encoderRotation(dataUser);
	ILI9341_Draw_Hollow_Rectangle_Coord(5, 55, 110, 85, GREEN);
}
/*	CHINH GOC O MAN HINH CHINH */
void fncAngleInMain(dataUser_t dataUser)
{
	encoderRotation(dataUser);
	ILI9341_Draw_Hollow_Rectangle_Coord(5, 90, 110, 115, GREEN);
}
void clearLCD(void)
{
	ILI9341_Fill_Screen(WHITE);
}
void BlynkSlot(dataUser_t dataUser)
{
	static bool state;
	{
		if (state == true)
		{
			state = false;
			if (screenCurr == HAND_ROTATION_SCREEN)
			{
				ILI9341_Draw_Hollow_Rectangle_Coord(
					Allign,
					Allign + ((dataUser.dataSetupRotation.indexSlotInRotation) * HIGHT_ONE_SLOT),
					Allign + WIDTH_ONE_SLOT,
					Allign + (dataUser.dataSetupRotation.indexSlotInRotation + 1) * HIGHT_ONE_SLOT,
					GREEN);
			}
			else if (screenCurr == SETUP_SCREEN)
			{
				ILI9341_Draw_Hollow_Rectangle_Coord(
					Allign,
					Allign + ((dataUser.indexSlotInSetup) * HIGHT_ONE_SLOT),
					Allign + WIDTH_ONE_SLOT,
					Allign + (dataUser.indexSlotInSetup + 1) * HIGHT_ONE_SLOT,
					GREEN);
			}
			else if (screenCurr == INPUT_TOTAL_PAGE_SCREEN)
			{
				ILI9341_Draw_Hollow_Rectangle_Coord(
					Allign,
					Allign + ((2 - 1) * HIGHT_ONE_SLOT),
					Allign + WIDTH_ONE_SLOT,
					Allign + (2) * HIGHT_ONE_SLOT,
					GREEN);
			}
		}
		else
		{
			state = true;
			if (screenCurr == HAND_ROTATION_SCREEN)
			{
				ILI9341_Draw_Hollow_Rectangle_Coord(
					Allign,
					Allign + ((dataUser.dataSetupRotation.indexSlotInRotation) * HIGHT_ONE_SLOT),
					Allign + WIDTH_ONE_SLOT,
					Allign + (dataUser.dataSetupRotation.indexSlotInRotation + 1) * HIGHT_ONE_SLOT,
					RED);
			}
			else if (screenCurr == SETUP_SCREEN)
			{
				ILI9341_Draw_Hollow_Rectangle_Coord(
					Allign,
					Allign + ((dataUser.indexSlotInSetup) * HIGHT_ONE_SLOT),
					Allign + WIDTH_ONE_SLOT,
					Allign + (dataUser.indexSlotInSetup + 1) * HIGHT_ONE_SLOT,
					RED);
			}
			else if (screenCurr == INPUT_TOTAL_PAGE_SCREEN)
			{
				ILI9341_Draw_Hollow_Rectangle_Coord(
					Allign,
					Allign + ((2 - 1) * HIGHT_ONE_SLOT),
					Allign + WIDTH_ONE_SLOT,
					Allign + (2) * HIGHT_ONE_SLOT,
					RED);
			}
		}
		/// 2->4
	}
}
