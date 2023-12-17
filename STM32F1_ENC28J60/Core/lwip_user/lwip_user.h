#ifndef _LWIP_USER
#define _LWIP_USER
#include "main.h"

extern uint8_t localIP[4];
extern uint8_t gatewayIP[4];
extern uint8_t subnetmask[4];

char ETH_IPOK(void);

uint8_t lwip_config_init(void);
void lwip_periodic_handle(void);
void lwip_dhcp_process_handle(void);
//uint32_t LWIP_RAND(void);
#endif
