#ifndef UART_H
#define UART_H

#include "stdbool.h"
#include "stdint.h"

extern void uart_receive_esp(uint8_t rx_data);
extern void uart_handle_esp();

extern void uart_receive_stm(uint8_t rx_data);
extern void uart_handle_stm();


#endif