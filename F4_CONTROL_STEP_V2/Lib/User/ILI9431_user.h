#ifndef ILI9341_USER_H
#define ILI9341_USER_H

#include "stdint.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stdbool.h"
#include "Button_user.h"

extern const Font Font1;

#define ROUND 360
typedef struct
{
		uint16_t Rotation[30]; 
		uint16_t Angle[30]; 
		uint16_t Speed;
		uint32_t Pulse;
}dataSaveFlash_t;

typedef struct
{
		bool LoopMode;
		bool ReleaseStepB;
		bool ReleaseStepB;
	
		uint8_t indexPageInSetup;			// SETTING
		uint8_t indexSlotInSetup;			// SETTING

		uint8_t indexSlotInAngle;					// SETTING
		uint8_t indexSlotInRotation;			// SETTING
	
		uint8_t indexPageInAngle;					// SETTING
		uint8_t indexPageInRotation;			// SETTING
	
		uint16_t AngleInTFT;								// SHOW 
		uint16_t RotationInTFT;						// SHOW 
	
		uint16_t IndexRotation;							//EX: 1 -> (32, 64, 96)
		uint16_t IndexAngle;								// 1 -> 360
	
		
}dataUser_t;



bool ArrayTFT(Screen_t screen, dataUser_t dataUser);

void initILI(void);
void encoderAngle(dataUser_t dataUser);
void encoderRotation(dataUser_t dataUser);
void handAngle(dataUser_t dataUser);
void handRotation(dataUser_t dataUser);
void setupParameter(dataUser_t dataUser);

void clearLCD(void);
void BlynkSlot(dataUser_t dataUser );
void clearOneSlot(uint8_t slot);

void pageInAngle(dataUser_t dataUser);
void pageInRotation(dataUser_t dataUser);
void pageInSetup(dataUser_t dataUser);
#endif