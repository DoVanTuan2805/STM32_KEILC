#include "drivenStep.h"
void stepNULL(InfrStep_t const *step);
void step_ctor(InfrStep_t *step, 
                GPIO_TypeDef *gpio_port_pul,
                uint16_t gpio_pin_pul,
                GPIO_TypeDef *gpio_port_dir,
                uint16_t gpio_pin_dir,
                GPIO_TypeDef *gpio_port_en,
                uint16_t gpio_pin_en
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

    step->gpio_port_en = gpio_port_en;
    step->gpio_pin_en= gpio_pin_en;

    step->u16pulse = 3200 * 2;
    step->u8speed = 10;
}
void setSpeed(InfrStep_t *step, uint8_t speed)
{
    step->u8speed = speed;
}
void setPulse(InfrStep_t *step, uint16_t pulse)
{
    step->u16pulse = pulse;
}
uint16_t getPulse(InfrStep_t *step)
{
    return step->u16pulse;
}
void set_dir(InfrStep_t const*step, Direction_t dir)
{
    HAL_GPIO_WritePin(step->gpio_port_dir, step->gpio_pin_dir, dir);
}
void move_to(InfrStep_t const *step[], int num_step)
{
    for(int i =0; i < num_step; i++)
    {
        move_step(step[i]);
    }
}
