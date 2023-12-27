#ifndef FINGER_USER_H
#define FINGER_USER_H

#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "fingerprint.h"
#include "main.h"

#define POWER_PORT 	FINGER_POWER_GPIO_Port
#define POWER_PIN 	FINGER_POWER_Pin

void initFinger(void);
int8_t fingerHandle(void);

#endif