#include "Sensor.h"

float getTempChannel(uint8_t channel)
{
		float TempChannel[5] = {30.9, 20.1, 22.5, 28.6, 29.8};
		return TempChannel[channel - 1];
}
