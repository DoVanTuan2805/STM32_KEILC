#include "stdlib.h"
#include <stdio.h>
#include <math.h>

#include "gpio.h"
#include "tim.h"

#include "ak.h"
#include "timer.h"

#include "app.h"
#include "task_list.h"

#include "scr_device.h"
#include "task_step.h"

#include "io_cfg.h"
#include "system.h"

#define STEP_NON_COMPLETE   (0)
#define STEP_COMPLETE       (1)

#define STEP_CCW        (1)
#define STEP_CW         (0)

#define STEP_MAX_SPEED      (70)
#define STEP_MIN_SPEED      (150)

volatile uint64_t pulse_to_step;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static inline void step_calculate()
{
	static volatile float last_step_deg;
	uint8_t dir;
	float ans;
#define ANGLE_NEW           (user_step_info.user_scr_main.step_deg + user_step_info.user_scr_main.cheater)
    if(ANGLE_NEW  != last_step_deg)
    {
        ans = ANGLE_NEW - last_step_deg;
        if (fabs(ans) > 180) {
            dir = ans > 0 ? STEP_CCW : STEP_CW;
            ans = 360 - fabs(ans);  // 240 -> 0 = -240
            HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, dir);
        } 
        else {
            dir = ans > 0 ? STEP_CW : STEP_CCW;
            ans = fabs(ans);
			HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, dir);
        }
        pulse_to_step = (user_step_info.user_setting.pulse * ans) / 360;
    } 
    last_step_deg = ANGLE_NEW;       // reset
}

void step_init() {
    timer10_irq_disable();
    step_calculate();
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);			// MUST SET ENABLE 
}

void step_set_speed()
{
    uint8_t _speed = map(user_step_info.user_setting.speed, 0, 9, STEP_MIN_SPEED, STEP_MAX_SPEED);
    htim10.Instance = TIM10;
    htim10.Init.Prescaler = 84 - 1;
    htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim10.Init.Period = _speed - 1;
    htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim10) != HAL_OK) {
        Error_Handler();
    }
}

void step_run()
{
    static uint8_t step_state = 0;
    if(pulse_to_step != 0)
    {
        step_state = STEP_NON_COMPLETE;
        pulse_to_step--;
        step_toggle();
    }
    if(pulse_to_step == 0 && step_state == STEP_NON_COMPLETE)
    {
        step_state = STEP_COMPLETE;
        timer10_irq_disable();
    }
}

void task_step_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        case STEP_INIT_SIG: {
            step_init();
            break;
        }

        case STEP_UPDATE_DEG_SIG: {
            step_set_speed();
			step_calculate();
            task_post_pure_msg(TASK_STEP_ID, STEP_ENABLE_SIG);
            break;
        }

        case STEP_DISABLE_SIG: {
            timer10_irq_disable();
            break;
        }

        case STEP_LOOP_MODE: {
            if (pulse_to_step == 0) {
                step_set_speed();
                timer10_irq_enable();
                pulse_to_step = user_step_info.user_setting.pulse;
            }
            break;
        }

        case STEP_ENABLE_SIG: {
            timer10_irq_enable();
            break;
        }

        default:
            break;
    }
}