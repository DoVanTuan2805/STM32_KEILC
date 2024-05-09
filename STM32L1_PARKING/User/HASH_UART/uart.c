#include "uart.h"

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "stdbool.h"
#include "string.h"

#include "main.h"
#include "process.h"
#include "parse_data.h"

#define SIZE_DATA 128
volatile uint8_t buffer_data[SIZE_DATA] = {};
volatile uint8_t buffer_data_ESP[SIZE_DATA] = {};
volatile uint8_t i_STM = 0, i_ESP = 0;

bool flag_rx_stm, flag_rx_esp;
void uart_receive_stm(uint8_t rx_data) {
    if (rx_data == '\n') {
        i_STM = 0;
        flag_rx_stm = true;
    } 
	else {
        buffer_data[i_STM++] = rx_data;
    }
}
void uart_receive_esp(uint8_t rx_data) {
    if (rx_data == '\n') {
        i_ESP = 0;
        flag_rx_esp = true;
    } 
	else {
        buffer_data_ESP[i_ESP++] = rx_data;
    }
}
void uart_handle_stm() {
    if (flag_rx_stm) {
        if (strcmp(parseData((char *)buffer_data, 0), "IN") == 0) {
            if (strcmp(parseData((char *)buffer_data, 1), "0") != 0 ||
                strcmp(parseData((char *)buffer_data, 2), "0") != 0)
            {
                viTri = atoi(parseData((char *)buffer_data, 1));
                soTang = atoi(parseData((char *)buffer_data, 2));
                signal = PROCESS_SIGNAL_IN_HAVE_CAR;
                process_step_pallet = PROCESS_FIND_EMPTY_CAR;
                process_put_get_car = PROCESS_PALLET_GO_OUT;
            }
            else
            {
                signal = PROCESS_SIGNAL_IN_NON_CAR;
                process_step_pallet = PROCESS_FIND_EMPTY_CAR;
                process_put_get_car = PROCESS_PALLET_GO_OUT;
            }

        } else if (strcmp(parseData((char *)buffer_data, 0), "OUT") == 0) {
            viTri = atoi(parseData((char *)buffer_data, 1));
            soTang = atoi(parseData((char *)buffer_data, 2));

            signal = PROCESS_SIGNAL_OUT_CAR;
            process_step_pallet = PROCESS_GO_NON_EMPTY_CAR;
            process_put_get_car = PROCESS_PALLET_GO_OUT;
        }
        flag_rx_stm = false;
    }
}
void uart_handle_esp() {
    if (flag_rx_esp) {
        if (strcmp(parseData((char *)buffer_data_ESP, 0), "IN") == 0) {

            if (strcmp(parseData((char *)buffer_data_ESP, 1), "0") != 0 ||
                strcmp(parseData((char *)buffer_data_ESP, 2), "0") != 0) {

                viTri = atoi(parseData((char *)buffer_data_ESP, 1));
                soTang = atoi(parseData((char *)buffer_data_ESP, 2));
                signal = PROCESS_SIGNAL_IN_HAVE_CAR;
                process_step_pallet = PROCESS_FIND_EMPTY_CAR;
                process_put_get_car = PROCESS_PALLET_GO_OUT;
            }
            else 
            {
                signal = PROCESS_SIGNAL_IN_NON_CAR;
                process_step_pallet = PROCESS_FIND_EMPTY_CAR;
                process_put_get_car = PROCESS_PALLET_GO_OUT;
            }
        } 
		else if (strcmp(parseData((char *)buffer_data_ESP, 0), "OUT") == 0) {
            viTri = atoi(parseData((char *)buffer_data_ESP, 1));
            soTang = atoi(parseData((char *)buffer_data_ESP, 2));

            signal = PROCESS_SIGNAL_OUT_CAR;
            process_step_pallet = PROCESS_GO_NON_EMPTY_CAR;
            process_put_get_car = PROCESS_PALLET_GO_OUT;
        }
        flag_rx_esp = false;
    }
}