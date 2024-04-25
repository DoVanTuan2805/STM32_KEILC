#ifndef PROCESS_H
#define PROCESS_H

#include "stdbool.h"
typedef enum
{
    PROCESS_SIGNAL_NULL,
    PROCESS_SIGNAL_IN_CAR,          // CAT XE
    PROCESS_SIGNAL_OUT_CAR,         // LAY XE
}signal_t;
typedef enum {
    PROCESS_STEP_PLATTE_NULL,

    PROCESS_FIND_EMPTY_CAR,         // TIM CHO TRONG
    PROCESS_GO_EMPTY_CAR,           // DI TOI CHO TRONG
    PROCESS_PUT_CAR,

    PROCESS_GO_GET_CAR,             // DI TOI CHO CO XE

    PROCESS_GO_IN,
    PROCESS_GO_OUT
} process_step_pallet_t;

typedef enum {
    PROCESS_PUT_GET_NULL,
    PROCESS_PALLET_GO_OUT,    // PALLET DI RA
    PROCESS_PALLET_GO_IN,     // PALLET DI VAO
    PROCESS_PALLET_INCREASE,  // TANG PALLET
    PROCESS_PALLET_DECREASE,  // GIAM PALLET
} process_put_get_car_t;

extern signal_t signal;
extern process_step_pallet_t process_step_pallet;
extern process_put_get_car_t process_put_get_car;


extern void process_parking();
extern void process_step_pallet_in();
extern void process_step_pallet_out();
extern bool get_car();
extern bool put_car();
#endif