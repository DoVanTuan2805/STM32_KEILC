#include "vl53_user.h"

void vl53_user_init()
{
		Dev->I2cHandle = &hi2c1;
		Dev->I2cDevAddr = 0x52;
	
		VL53L0X_WaitDeviceBooted( Dev );
		VL53L0X_DataInit( Dev );
		VL53L0X_StaticInit( Dev );
		VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
		VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
		VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);
		
	  VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
		VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
		VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));
		VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));
		VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, 1);// 33000
		VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
		VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
}
uint16_t getVL53SingleMode()
{
		static uint16_t data;
		VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingData);
		data = RangingData.RangeMilliMeter;
		return data;
}