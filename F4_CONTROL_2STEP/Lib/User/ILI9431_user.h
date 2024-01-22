#ifndef ILI9341_USER_H
#define ILI9341_USER_H

#include "stdint.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stdbool.h"
#include "Button_user.h"

extern const Font Font1;

typedef struct
{
	uint8_t page;
	int8_t Angle;
	int8_t encoder2;
	uint8_t key;
}dataUser_t;

#define NUM_OF_SCREEN 2

bool ArrayTFT(Screen_t screen, dataUser_t dataUser);

void initILI(void);
void drawCircleAngle(uint8_t angle);
void drawLineInCircle( dataUser_t dataUser);

void drawLayout(dataUser_t dataUser);
void layoutManual(dataUser_t dataUser);



#endif