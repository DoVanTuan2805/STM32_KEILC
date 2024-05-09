#include "encoder.h"

#include "io_cfg.h"

#define ENCODER_TIME_DEBOUNCE	(50)

static inline uint32_t sys_tick()
{
	return HAL_GetTick();
}

void encoder_init(encoder_t *encoder, 
				uint8_t id, 
				pf_encoder_callback callback)
{
	encoder->enable = ENCODER_DISABLE;
	encoder->id		= id;
	encoder->callback = callback;
	encoder->time_debounce = sys_tick();
}
void encoder_enable(encoder_t *encoder)
{
	encoder->enable = ENCODER_ENABLE;
}

void encoder_disable(encoder_t *encoder)
{
	encoder->enable = ENCODER_DISABLE;
}

inline uint32_t systick_get()
{
	return HAL_GetTick();
}

#ifdef ENCODER_NON_IRQ
inline void encoder_timer_polling(encoder_t *encoder)
{
	static uint8_t last_encoder_A;
	uint8_t hw_encoder_A;
	if(encoder->enable == ENCODER_ENABLE)		// ENABLE ENCODER
	{
		hw_encoder_A = encoder->read_A();
		if(hw_encoder_A != last_encoder_A)
		{
			if (encoder->read_B() != hw_encoder_A)
			{
				encoder->state = ENCODER_INCREASE;
				encoder->callback(encoder);
			}
			else
			{
				encoder->state = ENCODER_DECREASE;
				encoder->callback(encoder);
			}
		}
		last_encoder_A = hw_encoder_A;
	}
}
#else

inline void encoder_irq(encoder_t * encoder, uint8_t read_B)
{
	if(encoder->enable == ENCODER_ENABLE)
	{
		if(sys_tick() - encoder->time_debounce >= ENCODER_TIME_DEBOUNCE){
			if (read_B == ENCODER_LOW)
			{
				encoder->state = ENCODER_INCREASE;
				encoder->callback(encoder);
			} 
			else if (read_B == ENCODER_HIGHT)
			{
				encoder->state = ENCODER_DECREASE;
				encoder->callback(encoder);
			} 
			encoder->time_debounce = sys_tick();
		}
	}
}

#endif