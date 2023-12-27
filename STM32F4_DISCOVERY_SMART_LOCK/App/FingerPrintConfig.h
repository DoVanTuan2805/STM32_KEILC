#ifndef	_FINGERPRINTCONFIG_H_
#define	_FINGERPRINTCONFIG_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#define	_FINGERPRINT_USART						huart2
#define	_FINGERPRINT_POWER_GPIO				FINGER_ENABLE_GPIO_Port
#define	_FINGERPRINT_POWER_PIN				FINGER_ENABLE_Pin
#define	_FINGERPRINT_IRQ_GPIO					FINGER_IRQ_GPIO_Port
#define	_FINGERPRINT_IRQ_PIN					FINGER_IRQ_Pin


#endif