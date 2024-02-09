#ifndef ILI9341_USER_H
#define ILI9341_USER_H

#include "stdint.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stdbool.h"
#include "Button_user.h"
#include "user_ex.h"
extern const Font Font1;

bool ArrayTFT(Screen_t screen, dataUser_t dataUser);

void initILI(void);

void cheater(dataUser_t dataUser);

void fncFloorInMain(dataUser_t dataUser);
void fncAngleInMain(dataUser_t dataUser);

void encoderRotation(dataUser_t dataUser);
void handRotation(dataUser_t dataUser);

void selectCreateModify(dataUser_t dataUser);
void createRotation(dataUser_t dataUser);

void setupParameter(dataUser_t dataUser);

void clearLCD(void);
void BlynkSlot(dataUser_t dataUser);
void clearOneSlot(uint8_t slot);

void pageInRotation(dataUser_t dataUser);
void pageInSetup(dataUser_t dataUser);

#endif