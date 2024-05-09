#include "io_cfg.h"

#include "gpio.h"
#include "tim.h"


inline uint8_t button_up_read()
{
    return HAL_GPIO_ReadPin(BT_UP_GPIO_Port, BT_UP_Pin);
}

inline uint8_t button_down_read()
{
    return HAL_GPIO_ReadPin(BT_DOWN_GPIO_Port, BT_DOWN_Pin);
}

inline void step_toggle()
{
    HAL_GPIO_TogglePin(PUL2_GPIO_Port, PUL2_Pin);
}

inline void step_set_dir(uint8_t dir)
{
}