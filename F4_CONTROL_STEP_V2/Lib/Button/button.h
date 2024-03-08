/*
 * button.h
 *
 *  Created on: Mar 28, 2023
 *      Author: admin
 */
#ifndef BUTTON_H
#define BUTTON_H
#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif
	/* C++ detection */
	typedef struct
	{
		GPIO_TypeDef *PORT;
		uint16_t PIN;
		uint8_t btn_filter;
		uint8_t btn_last;
		uint8_t btn_current;
		uint32_t t_debounce;
		uint32_t time_start_press;
		uint8_t is_press;
	} Button_t;
	void Button_Init(volatile Button_t *button, GPIO_TypeDef *PORT, uint16_t PIN);
	void bt_handle(volatile Button_t *button);

#ifdef __cplusplus
}
#endif

#endif
