#include "stepPosition.h"
#include "stdlib.h"

stepPosition_t step_position = {};
volatile uint16_t pulse_step;
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
    set_position(step, 0);

}

void set_position(stepPosition_t *step, uint16_t position)
{
    step->new_position = position;
}

uint16_t get_position(stepPosition_t *step) 
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

static uint16_t calculate_angle(InfrStep_t const* const step, uint16_t current_position, uint16_t new_position)
{
    int16_t ans = 0;
	Direction_t dir;
    if(current_position != new_position)      // POS5 - POS0
    {
        ans = new_position - current_position;      // -> angle
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
	
	uint16_t pos_curr;
	uint16_t pos_new;
	
    pos_curr = get_position(stepPosition);      // get position current
    pos_new = stepPosition->new_position;

    // 0 - 120
    if(pos_curr != pos_new)      // update position 
    {  
        stepPosition->angle = calculate_angle(step, pos_curr, pos_new);
        stepPosition->curr_position = stepPosition->new_position;
		
		pulse_step =  getPulse(&step_position.inforStep) * get_angle_position(&step_position);
		reseet_angle_position(&step_position);
	}
}
void step_position_handler()
{
	static Direction_t dir;
	if(pulse_step != 0)
	{
		pulse_step--;
		dir ^= 1;
		set_pul(&step_position.inforStep, dir);
	}
}
