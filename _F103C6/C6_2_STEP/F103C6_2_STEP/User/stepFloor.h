#ifndef STEP_FLOOR
#define STEP_FLOOR
#include "drivenStep.h"
typedef enum {
    FLOOR1,
    FLOOR2,
    FLOOR3
}eFloor_t;

static uint64_t arr_pulse_to_floor[3] = 
{
    0, 3600, 7200
};

typedef struct 
{
    InfrStep_t inforStep;
    eFloor_t new_floor;
    eFloor_t curr_floor;
    uint16_t angle;
}stepFloor_t ;
void step_floor_ctor(stepFloor_t *step, 
                    GPIO_TypeDef *gpio_port_pul,
                    uint16_t gpio_pin_pul,
                    GPIO_TypeDef *gpio_port_dir,
                    uint16_t gpio_pin_dir,
                    GPIO_TypeDef *gpio_port_en,
                    uint16_t gpio_pin_en
);
void set_floor(stepFloor_t *step, eFloor_t floor);
eFloor_t get_floor(stepFloor_t *step);

uint16_t get_angle_floor(stepFloor_t *step);
void reset_angle_floor(stepFloor_t *step);

void set_speed_floor(stepFloor_t *step, uint8_t speed);
uint8_t get_speed_floor(stepFloor_t *step);


#endif