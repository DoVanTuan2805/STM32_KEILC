#include "stepPosition.h"
#include "stdlib.h"
 ePosition_t pos_curr;
 ePosition_t pos_new;

static uint64_t arr_pulse_pos[6] = 
{
    0, 60, 120, 180, 240, 300
};


static void move_to_position(InfrStep_t const*step);

void step_position_ctor(stepPosition_t *step, 
                    GPIO_TypeDef *gpio_port_pul,
                    uint16_t gpio_pin_pul,
                    GPIO_TypeDef *gpio_port_dir,
                    uint16_t gpio_pin_dir
)
{
    static struct StepTable VTableFloor ={
        &move_to_position
    };
    
    step_ctor(&step->inforStep, 
                gpio_port_pul, 
                gpio_pin_pul,
                gpio_port_dir, 
                gpio_pin_dir );
    step->inforStep.vPtr = &VTableFloor;
    set_position(step, POS1);

}
void set_position(stepPosition_t *step, ePosition_t position)
{
    step->new_position = position;
}
ePosition_t get_position(stepPosition_t *step) 
{
    return step->curr_position;
}
void reseet_angle_position(stepPosition_t *step)
{
    step->angle = 0;
}
uint16_t get_angle_position(stepPosition_t *step)
{
    return step->angle;
}
Direction_t dir;
static uint16_t calculate_angle(InfrStep_t const* const step, ePosition_t current, ePosition_t new)
{
    int16_t ans = 0;

    if(current != new)      // POS5 - POS0
    {
        ans = arr_pulse_pos[new] - arr_pulse_pos[current];      // -> angle
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
    }
    return ans;
} 

static void move_to_position(InfrStep_t const* const step)
{
    stepPosition_t *stepPosition = (stepPosition_t*)step;

    pos_curr = get_position(stepPosition);      // get position current
    pos_new = stepPosition->new_position;

    // 0 - 120
    if(pos_curr != pos_new)      // update position 
    {  
        stepPosition->angle = calculate_angle(step, pos_curr, pos_new);
        stepPosition->curr_position = stepPosition->new_position;
    }
}
