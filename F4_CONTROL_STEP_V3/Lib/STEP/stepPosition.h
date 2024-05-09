#ifndef STEP_POSITION
#define STEP_POSITION

#include "drivenStep.h"
typedef struct 
{
    InfrStep_t inforStep;
    uint16_t curr_position;
    uint16_t new_position;
    uint16_t angle;
}stepPosition_t ;

extern stepPosition_t step_position;				// variable control step

void step_position_ctor(stepPosition_t *step, 
                    GPIO_TypeDef *gpio_port_pul,
                    uint16_t gpio_pin_pul,
                    GPIO_TypeDef *gpio_port_dir,
                    uint16_t gpio_pin_dir
);
void position_init(uint16_t num_position);			// (0->360)

void set_position(stepPosition_t *step, uint16_t position);
uint16_t get_position(stepPosition_t *step);

void reseet_angle_position(stepPosition_t *step);
uint16_t get_angle_position(stepPosition_t *step);

void set_speed_position(stepPosition_t *step, uint8_t speed);
uint8_t get_speed_position(stepPosition_t *step);

void step_position_handler();
#endif