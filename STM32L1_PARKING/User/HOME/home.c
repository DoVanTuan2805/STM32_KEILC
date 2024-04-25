#include "home.h"

#include "main.h"

#include "tim.h"
#include "gpio.h"

#include "stepPosition.h"
#include "stepFloor.h"
bool tc_home = false, ctht_home = false;

void go_home() {
    HAL_TIM_Base_Start_IT(&htim2);
//	set_dir(&stepP.inforStep, CW);		// thuan
    while (1) {
        tc_home = HAL_GPIO_ReadPin(TC_HOME_GPIO_Port, TC_HOME_Pin);
        if (tc_home == true) {
            pulseP = 0;
			
            while (1) {
				set_dir(&stepF.inforStep, CW);			// di xuong
                ctht_home =
                    HAL_GPIO_ReadPin(CTHT_HOME_GPIO_Port, CTHT_HOME_Pin);
                if (ctht_home == 0) {
                    set_dir(&stepF.inforStep, CCW);			// di len
					HAL_Delay(20);
                    pulseF = 17600;
                    encoder_read = 0;
                    break;
                }
                pulseF = 1600;
            }
            tc_home = false;
            break;
        }
        pulseP = 1600;
    }
    viTri = 0;
    soTang = 0;
    set_position(&stepP, viTri);
    set_floor(&stepF, soTang);
}