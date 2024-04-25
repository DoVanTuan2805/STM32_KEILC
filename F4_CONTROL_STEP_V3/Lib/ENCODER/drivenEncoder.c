#include "drivenStep.h"
void stepNULL(InfrStep_t const *step);


void step_ctor(InfrStep_t *step, 
                GPIO_TypeDef *gpio_port_pul,
                uint16_t gpio_pin_pul,
                GPIO_TypeDef *gpio_port_dir,
                uint16_t gpio_pin_dir
    )
{
    struct StepTable VTable = {
        &stepNULL
    };
    step->vPtr = &VTable;

    step->gpio_port_pul = gpio_port_pul;
    step->gpio_pin_pul= gpio_pin_pul;

    step->gpio_port_dir = gpio_port_dir;
    step->gpio_pin_dir= gpio_pin_dir;


    step->u64pulse = 3200 * 2;
    step->u8speed = 10;
}
void setSpeed(InfrStep_t *step, uint8_t speed)
{
    step->u8speed = speed;
}
void setPulse(InfrStep_t *step, uint64_t pulse)
{
    step->u64pulse = pulse;
}
uint64_t getPulse(InfrStep_t *step)
{
    return step->u64pulse;
}
void set_dir(InfrStep_t const*step, Direction_t dir)
{
    HAL_GPIO_WritePin(step->gpio_port_dir, step->gpio_pin_dir, dir);
}
void set_pul(InfrStep_t const*step, Direction_t dir)
{
	HAL_GPIO_WritePin(step->gpio_port_pul, step->gpio_pin_pul, dir);
}
void move_to(InfrStep_t const *step[], int num_step)
{
    for(int i =0; i < num_step; i++)
    {
        move_step(step[i]);
    }
}
