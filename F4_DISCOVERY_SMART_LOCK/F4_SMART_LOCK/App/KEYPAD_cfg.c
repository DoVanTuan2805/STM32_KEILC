/*
 * File: KEYPAD_cfg.c
 * Driver Name: [[ 4x4 KEYPAD ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "KEYPAD.h"
#include "KEYPAD_cfg.h"
const KEYPAD_CfgType KEYPAD_CfgParam[KEYPAD_UNITS] =
{
	// KeyPAD Unit 1 Configurations
    {
    	/* ROWs Pins Info */
	  {GPIOE, GPIOE, GPIOE, GPIOE},
		{GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10},
		/* COLs Pins */
		{GPIOE, GPIOE, GPIOE},
		{GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13}
	}
};
