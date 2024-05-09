#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stdio.h"
#include "stdint.h"

#include "gpio.h"

typedef void (*pf_encoder_callback)(void*);

#define ENCODER_DISABLE	(0)
#define ENCODER_ENABLE	(1)

#define ENCODER_DECREASE	(0)
#define ENCODER_INCREASE	(1)
#define ENCODER_NON_ACTIVE	(2)

#define ENCODER_LOW			(0)
#define ENCODER_HIGHT		(1)

#define ENCODER_DIR_CW		(0)
#define ENCODER_DIR_CCW		(1)

#define ENCODER_COUNTER_INCREASE	(1)
#define ENCODER_COUNTER_DECREASE	(0)

typedef struct
{
	uint8_t id;
	uint8_t enable;
	uint8_t state;
	uint32_t time_debounce;
	pf_encoder_callback callback;
}encoder_t;

extern void encoder_init(encoder_t *encoder, 
				uint8_t id, 
				pf_encoder_callback callback);

extern void encoder_enable(encoder_t *encoder);
extern void encoder_disable(encoder_t *encoder);

#ifdef ENCODER_NON_IRQ
extern void encoder_timer_polling(encoder_t *encoder);
#else
extern void encoder_irq(encoder_t * encoder, uint8_t read_B);
#endif

#endif