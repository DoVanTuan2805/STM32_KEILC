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
#include "gpio.h"
const KEYPAD_CfgType KEYPAD_CfgParam[KEYPAD_UNITS] =
{
	// KeyPAD Unit 1 Configurations
    {
    	/* ROWs Pins Info */
	  {R1_GPIO_Port, R2_GPIO_Port, R3_GPIO_Port, R4_GPIO_Port},
		{R1_Pin, R2_Pin, R3_Pin, R4_Pin},
		/* COLs Pins */
		{C1_GPIO_Port, C2_GPIO_Port, C3_GPIO_Port, C4_GPIO_Port},
		{C1_Pin, C2_Pin, C3_Pin, C4_Pin}
	}
};
