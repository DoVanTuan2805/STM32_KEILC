#include "stepFloor.h"
#include "stdlib.h"
eFloor_t floor_curr, floor_new;

static void move_to_floor(InfrStep_t const*step);

void step_floor_ctor(stepFloor_t *step, 
                    GPIO_TypeDef *gpio_port_pul,
                    uint16_t gpio_pin_pul,
                    GPIO_TypeDef *gpio_port_dir,
                    uint16_t gpio_pin_dir,
                    GPIO_TypeDef *gpio_port_en,
                    uint16_t gpio_pin_en
)
{
    static struct StepTable VTableFloor ={
        &move_to_floor
    };
    

    step_ctor(&step->inforStep, 
                gpio_port_pul, 
                gpio_pin_pul,
                gpio_port_dir, 
                gpio_pin_dir,
                gpio_port_en,
                gpio_pin_en
            );
    step->inforStep.vPtr = &VTableFloor;
    set_floor(step, FLOOR1);
}
void set_floor(stepFloor_t *step, eFloor_t floor)
{
    step->new_floor = floor;
}
eFloor_t get_floor(stepFloor_t *step) 
{
    return step->curr_floor;
}
uint16_t get_angle_floor(stepFloor_t *step)
{
    return step->angle;
}
void reset_angle_floor(stepFloor_t *step)
{
    step->angle = 0;
}
static uint16_t calculate_angle(InfrStep_t const* const step, eFloor_t current, eFloor_t new)
{
    int16_t ans = 0;
    Direction_t dir;
    uint8_t iNum1 = 0, iNum2 = 0;
    if(current != new)      // POS5 - POS0
    {
        ans = arr_pulse_to_floor[new] - arr_pulse_to_floor[current];      // -> angle   
        /*
        if(abs(ans) > 360)
        {
            //1064 / 360 = 2 --- % : 0.95
            iNum1 = abs(ans) / 360;
            iNum2 = abs(ans) % 360;
        }
        if(abs(ans) > 180)
        {
            dir = ans > 0 ? CCW : CW;
            ans = 360 - abs(ans);       // 240 -> 0 = -240
            set_dir(step,  dir);
        }
        else 
        {
            dir = ans > 0 ? CW : CCW;
            ans = abs(ans);
            set_dir(step,  dir);
        }  
        */
        dir = ans > 0 ? CW : CCW;
        set_dir(step, dir);
        ans = abs(ans);
    }
    return ans;
} 

static void move_to_floor(InfrStep_t const* const step)
{
    stepFloor_t *const stepFloor = (stepFloor_t*)step;
    floor_curr = get_floor(stepFloor);
    if(floor_curr != stepFloor->new_floor)
    {
        stepFloor->angle =  calculate_angle(step,  stepFloor->curr_floor, stepFloor->new_floor);
        stepFloor->curr_floor = stepFloor->new_floor;
    }
}