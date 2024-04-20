#ifndef STEP_POSITION
#define STEP_POSITION

#include "drivenStep.h"
typedef enum {
    POS1, POS2, POS3, POS4, POS5, POS6
}ePosition_t;


typedef struct 
{
    InfrStep_t inforStep;
    ePosition_t curr_position;
    ePosition_t new_position;
    uint16_t angle;
}stepPosition_t ;
void step_position_ctor(stepPosition_t *step, 
                    GPIO_TypeDef *gpio_port_pul,
                    uint16_t gpio_pin_pul,
                    GPIO_TypeDef *gpio_port_dir,
                    uint16_t gpio_pin_dir,
                    GPIO_TypeDef *gpio_port_en,
                    uint16_t gpio_pin_en
);
void set_position(stepPosition_t *step, ePosition_t position);
ePosition_t get_position(stepPosition_t *step);

void reseet_angle_position(stepPosition_t *step);
uint16_t get_angle_position(stepPosition_t *step);

void set_speed_position(stepPosition_t *step, uint8_t speed);
uint8_t get_speed_position(stepPosition_t *step);

#endif