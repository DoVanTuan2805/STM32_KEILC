#include "main.h"

#include "math.h"

#include "tim.h"

#include "process.h"
#include "cell_car.h"
#include "stepFloor.h"
#include "stepPosition.h"
#include "plate.h"
#include "home.h"

process_step_pallet_t process_step_pallet = PROCESS_STEP_PLATTE_NULL;
signal_t signal = PROCESS_SIGNAL_NULL;
process_put_get_car_t process_put_get_car = PROCESS_PUT_GET_NULL;

void process_parking()
{
    switch (signal) {
        case PROCESS_SIGNAL_IN_NON_CAR:
            read_E18();
            process_step_pallet_in();
            break;
        case PROCESS_SIGNAL_IN_HAVE_CAR:
            process_step_pallet_in();
            break;
        case PROCESS_SIGNAL_OUT_CAR:
            process_step_pallet_out();
            break;
        default:
            break;
    }
    
}

// XE VAO
void process_step_pallet_in() {
    switch (process_step_pallet) {
        case PROCESS_FIND_EMPTY_CAR:
            if (get_car() == true) {
                if (signal == PROCESS_SIGNAL_IN_NON_CAR) {
                    find_empty_car(arr_car_empty, NUM_CELL, &viTri, &soTang);
                }
                HAL_TIM_Base_Stop_IT(&htim2);
                process_step_pallet = PROCESS_GO_EMPTY_CAR;
            }
            break;

        case PROCESS_GO_EMPTY_CAR:
            set_position(&stepP, viTri);
            set_floor(&stepF, soTang);

            angle_position = get_angle_position(&stepP);
            angle_floor = get_angle_floor(&stepF);

            if (angle_position != 0 || angle_floor != 0) {
                pulseP =
                    round((float)pulseStepP * ((float)angle_position / 360.0));
                reseet_angle_position(&stepP);

                pulseF =
                    round((float)pulseStepF * ((float)angle_floor / 360.0));
                reset_angle_floor(&stepF);
                HAL_Delay(10);
                HAL_TIM_Base_Start_IT(&htim2);
            }
            if (is_complete == true) {
                process_step_pallet = PROCESS_PUT_CAR;
                process_put_get_car = PROCESS_PALLET_GO_OUT;
            }

            break;
        case PROCESS_PUT_CAR:
            if( put_car() == true)
            {
                process_step_pallet = PROCESS_GO_IN;
            }
            break;
        case PROCESS_GO_IN:
            go_home();
            process_step_pallet = PROCESS_STEP_PLATTE_NULL;  // HET QUA TRINH
            signal = PROCESS_SIGNAL_NULL;
            break;

        default:
            break;
    }
}

// XE RA
void process_step_pallet_out() {
    switch (process_step_pallet) {
        case PROCESS_GO_NON_EMPTY_CAR:
            set_position(&stepP, viTri);
            set_floor(&stepF, soTang);

            angle_position = get_angle_position(&stepP);
            angle_floor = get_angle_floor(&stepF);

            if (angle_position != 0 || angle_floor != 0) {
                pulseP =
                    round((float)pulseStepP * ((float)angle_position / 360.0));
                reseet_angle_position(&stepP);

                pulseF =
                    round((float)pulseStepF * ((float)angle_floor / 360.0));
                reset_angle_floor(&stepF);
                HAL_Delay(10);
                HAL_TIM_Base_Start_IT(&htim2);
            }
            if (is_complete) {
                process_step_pallet = PROCESS_GO_GET_CAR;
                process_put_get_car = PROCESS_PALLET_GO_OUT;
            }
            break;
        
        case PROCESS_GO_GET_CAR:
            if(get_car() == true)
            {
                process_step_pallet = PROCESS_GO_OUT;
            }
            break;

        case PROCESS_GO_OUT:
            viTri = 3;
            soTang = 0;
            set_position(&stepP, viTri);
            set_floor(&stepF, soTang);

            angle_position = get_angle_position(&stepP);
            angle_floor = get_angle_floor(&stepF);

            if (angle_position != 0 || angle_floor != 0) {
                HAL_TIM_Base_Stop_IT(&htim2);
                pulseP = round((float)pulseStepP * ((float)angle_position / 360.0));
                reseet_angle_position(&stepP);

                pulseF = round((float)pulseStepF * ((float)angle_floor / 360.0));
                reset_angle_floor(&stepF);
                HAL_Delay(10);
                HAL_TIM_Base_Start_IT(&htim2);
            }
            if (is_complete) {
                process_step_pallet = PROCESS_PUT_CAR;
                process_put_get_car = PROCESS_PALLET_GO_OUT;
            }
            break;
        
        case PROCESS_PUT_CAR:
            if(put_car() == true)
            {
                process_step_pallet = PROCESS_GO_IN;
            }
            break;

        case PROCESS_GO_IN:
            go_home();
            process_step_pallet = PROCESS_STEP_PLATTE_NULL;  // HET QUA TRINH
            signal = PROCESS_SIGNAL_NULL;
            break;

        default:
            break;
    }
}

bool get_car() {
    if (process_put_get_car == PROCESS_PALLET_GO_OUT) {
        plate_out();
        process_put_get_car = PROCESS_PALLET_INCREASE;
    }

    else if (process_put_get_car == PROCESS_PALLET_INCREASE) {
        set_dir(&stepF.inforStep, CCW);  // di len
        pulseF = 17600;
        process_put_get_car = PROCESS_PALLET_GO_IN;
    }

    else if (process_put_get_car == PROCESS_PALLET_GO_IN) {
        if (is_complete) {
            plate_in();
            process_put_get_car = PROCESS_PALLET_DECREASE;
        }
    }

    else if (process_put_get_car == PROCESS_PALLET_DECREASE) {
        // set_dir(&stepF.inforStep, CW);  // di xuong
        // pulseF = 17600;
        process_put_get_car = PROCESS_PUT_GET_NULL;
        return true;
    }

     return false;
}

bool put_car() {
    if (process_put_get_car == PROCESS_PALLET_INCREASE) {
        set_dir(&stepF.inforStep, CCW);  // di len
        pulseF = 17600;
        process_put_get_car = PROCESS_PALLET_GO_OUT;
    }

    else if (process_put_get_car == PROCESS_PALLET_GO_OUT) {
        plate_out();
        process_put_get_car = PROCESS_PALLET_DECREASE;
    }

    else if (process_put_get_car == PROCESS_PALLET_DECREASE) {
        set_dir(&stepF.inforStep, CW);  // di xuong
        pulseF = 17600;
        process_put_get_car = PROCESS_PALLET_GO_IN;
    }

    else if (process_put_get_car == PROCESS_PALLET_GO_IN) {
        if (is_complete) {
            plate_in();
            process_put_get_car = PROCESS_PUT_GET_NULL;
            return true;
        }
    }
    return false;
}
