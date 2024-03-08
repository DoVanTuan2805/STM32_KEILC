#include "Button_user.h"
#include "gpio.h"
#include "ILI9431_user.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "step_user.h"
Button_t bt_down, bt_up;
extern uint8_t u8COLUMS;
extern uint8_t *arrayRotationW;
extern dataPage_t dataPageW;
extern dataUser_t dataUserInput;
extern bool changeMode;
extern Screen_t screenCurr;
extern volatile uint32_t pulseStep;
extern bool stateSaveRotation;
bool changeButton;

void initButton(void)
{
	Button_Init(&bt_down, BT_DOWN_GPIO_Port, BT_DOWN_Pin);
	Button_Init(&bt_up, BT_UP_GPIO_Port, BT_UP_Pin);
}
void buttonHandle(void)
{
	bt_handle(&bt_down);
	bt_handle(&bt_up);
}
void bt_press_callback(volatile Button_t *button)
{
	if (button == &bt_up)
	{
		if (screenCurr == ENCODER_ROTATION_SCREEN)
		{
			changeButton = true;
			dataUserInput.RotationInTFT += ROUND / dataUserInput.IndexRotation;
			if (dataUserInput.RotationInTFT > ROUND)
			{
				dataUserInput.RotationInTFT = ROUND / dataUserInput.IndexRotation;
			}
		}
		else if (screenCurr == ANGLE_MAIN_SCREEN)
		{
			changeButton = true;
			dataUserInput.dataAngleInMain.indexAngle++;
			if (dataUserInput.dataAngleInMain.indexAngle > u8COLUMS)
			{
				dataUserInput.dataAngleInMain.indexAngle = 0;
			}
			if (dataUserInput.dataAngleInMain.rotation_4 != 0)
			{
				dataUserInput.RotationInTFT = dataUserInput.dataAngleInMain.rotation_4 * (ROUND / dataUserInput.IndexRotation);
				pulseStep = stepAngle(dataUserInput.RotationInTFT + dataUserInput.fCheater);
			}
		}
		else if (screenCurr == HAND_ROTATION_SCREEN)
		{
			changeButton = true;
			clearLCD();
			dataUserInput.dataSetupRotation.indexSlotInRotation = 1;
			dataUserInput.dataSetupRotation.indexPageInRotation = 1;
			dataPageW.indexArrRCur = 1;
			dataPageW.page += 1;
			if (dataPageW.page >= dataPageW.totalPage) // page now < total page
			{
				dataPageW.page = dataPageW.totalPage - 1;
			}
		}
	}
	else if (button == &bt_down)
	{
		if (screenCurr == ENCODER_ROTATION_SCREEN)
		{
			changeButton = true;
			dataUserInput.RotationInTFT -= ROUND / dataUserInput.IndexRotation;
			if (dataUserInput.RotationInTFT < ROUND / dataUserInput.IndexRotation)
			{
				dataUserInput.RotationInTFT = ROUND;
			}
		}
		else if (screenCurr == ANGLE_MAIN_SCREEN)
		{
			changeButton = true;
			dataUserInput.dataAngleInMain.indexAngle--;
			if (dataUserInput.dataAngleInMain.indexAngle < 0)
			{
				dataUserInput.dataAngleInMain.indexAngle = u8COLUMS;
			}
			if (dataUserInput.dataAngleInMain.rotation_2 != 0)
			{
				dataUserInput.RotationInTFT = dataUserInput.dataAngleInMain.rotation_2 * (ROUND / dataUserInput.IndexRotation);
				pulseStep = stepAngle(dataUserInput.RotationInTFT + dataUserInput.fCheater);
			}
		}
		else if (screenCurr == HAND_ROTATION_SCREEN)
		{
			changeButton = true;
			clearLCD();
			dataUserInput.dataSetupRotation.indexSlotInRotation = 1;
			dataUserInput.dataSetupRotation.indexPageInRotation = 1;
			dataPageW.indexArrRCur = 1;
			if (dataPageW.page > 0) // page now < total page
			{
				dataPageW.page -= 1;
			}
		}
	}
}
void bt_press_timout_callback(volatile Button_t *button)
{
	if (button == &bt_up)
	{
		if (screenCurr == HAND_ROTATION_SCREEN)
		{
			free(arrayRotationW);
			arrayRotationW = (uint8_t *)malloc((dataPageW.totalPage * u8COLUMS) * sizeof(uint8_t));
			convertToArray1D(dataPageW.dataRotation, arrayRotationW, dataPageW.totalPage, u8COLUMS);
			stateSaveRotation = true;
			changeButton = true;
			screenCurr = ENCODER_ROTATION_SCREEN;
		}
		else if (screenCurr == INPUT_TOTAL_PAGE_SCREEN)
		{
			// uint8_t col = dataPageW.totalPage * u8COLUMS;
			dataPageW.dataRotation = (uint8_t **)malloc(dataPageW.totalPage * sizeof(uint8_t *));
			if (dataPageW.dataRotation != NULL)
			{
				dataPageW.indexArrRCur = 1;
				for (int i = 0; i < dataPageW.totalPage; i++)
				{
					dataPageW.dataRotation[i] = (uint8_t *)malloc((u8COLUMS + 1) * sizeof(uint8_t));
					if (dataPageW.dataRotation[i] == NULL)
					{
						for (int j = 0; j < dataPageW.totalPage; j++)
						{
							free(dataPageW.dataRotation[j]);
						}
						free(dataPageW.dataRotation);
					}
					for (int k = 0; k < u8COLUMS + 1; k++)
					{
						dataPageW.dataRotation[i][k] = 0;
					}
				}
				screenCurr = HAND_ROTATION_SCREEN;
			}
		}
	}
}
void convertToArray1D(uint8_t **arr2D, uint8_t *arr1D, int rows, int cols)
{
	int i, j, k = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			arr1D[k++] = arr2D[i][j];
		}
	}
}
void convertToArray2D(uint8_t *arr1D, uint8_t **arr2D, int rows, int cols)
{
	int k = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			arr2D[i][j] = arr1D[k++];
		}
	}
}