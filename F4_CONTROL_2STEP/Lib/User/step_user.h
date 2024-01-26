#ifndef STEP_USER_H
#define STEP_USER_H
#include "stdint.h"

typedef struct
{
	float currentAngle;
	float nextAngle;
}inforStep_t;

uint32_t stepAngle(float angle);

#endif