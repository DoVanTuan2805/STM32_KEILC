#ifndef VL53_USER_H
#define VL53_USER_H

#include "main.h"
#include "vl53l0x_api.h"

extern I2C_HandleTypeDef hi2c1;


static VL53L0X_RangingMeasurementData_t RangingData;
static VL53L0X_Dev_t  vl53l0x_c; // center module
static VL53L0X_DEV    Dev = &vl53l0x_c;

static uint32_t refSpadCount;
static uint8_t isApertureSpads;
static uint8_t VhvSettings;
static uint8_t PhaseCal;

void vl53_user_init();
uint16_t getVL53SingleMode();
#endif