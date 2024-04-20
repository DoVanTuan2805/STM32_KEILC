#ifndef DRIVEN_STEP
#define DRIVEN_STEP

#include "stdio.h"
#include "stdint.h"
#include "gpio.h"
typedef enum
{
    CW = 0, 
    CCW = 1     // THUAN , NGHICH
}Direction_t;   

struct StepTable;

typedef struct
{
    struct StepTable *vPtr;
    GPIO_TypeDef *gpio_port_pul;
    uint16_t gpio_pin_pul;
    GPIO_TypeDef *gpio_port_dir;
    uint16_t gpio_pin_dir;
    GPIO_TypeDef *gpio_port_en;
    uint16_t gpio_pin_en;

    uint8_t u8speed;
    uint16_t u16pulse;
}InfrStep_t;

struct StepTable
{
    void (*move) (InfrStep_t const *step);
};

static inline void move_step(InfrStep_t const *const step)
{
    (*step->vPtr->move)(step);
}

void step_ctor( InfrStep_t *step, 
                GPIO_TypeDef *gpio_port_pul,
                uint16_t gpio_pin_pul,
                GPIO_TypeDef *gpio_port_dir,
                uint16_t gpio_pin_dir,
                GPIO_TypeDef *gpio_port_en,
                uint16_t gpio_pin_en
);
void setSpeed(InfrStep_t *step, uint8_t speed);
void setPulse(InfrStep_t *step, uint16_t pulse);
uint16_t getPulse(InfrStep_t *step);
void set_dir(InfrStep_t const* step, Direction_t dir);

void move_to(InfrStep_t const *step[], int num_step);


#endif

