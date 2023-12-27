#include "finger_user.h"

void powerFinger(bool state)
{
	bool stt = state ? 1 : 0;
	HAL_GPIO_WritePin(POWER_PORT , POWER_PIN, stt);
}

void initFinger(void)
{
		init_fingerprint();
		powerFinger(true);
		led_mode(1);		// on
}
int8_t fingerHandle(void)
{
		if (getImage() != FINGERPRINT_OK)
		{
			return -1;
		}
		else if (getImage2Tz() != FINGERPRINT_OK)
		{
			return -2;
		}
		else if (fingerFastSearch() != FINGERPRINT_OK)
		{
			return -3;
		}
		else 
		{
			return 0;
		}
}
