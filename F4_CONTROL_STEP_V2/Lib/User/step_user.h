#ifndef STEP_USER_H
#define STEP_USER_H
#include "stdint.h"

#define MID 180
#define END 360
typedef struct
{
	uint8_t speed;
	uint32_t pulseOneRoud;
	float currentAngle;
	float nextAngle;
} inforStep_t;
void initStep(void);
void setPulseOneRound(uint32_t u32Pulse);
void setSpeedStep(uint32_t u32Speed);
uint32_t stepAngle(float angle);

#endif