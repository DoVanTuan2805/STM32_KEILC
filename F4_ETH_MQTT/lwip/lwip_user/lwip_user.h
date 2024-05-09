#ifndef _LWIP_USER
#define _LWIP_USER

#include "main.h"
#include "lwipopts.h"

extern uint8_t localIP[4];
extern uint8_t gatewayIP[4];
extern uint8_t subnetmask[4];

#if LWIP_DHCP
static uint32_t DHCPfineTimer=0;	
static uint32_t DHCPcoarseTimer=0;
#endif

char ETH_IPOK(void);

uint8_t lwip_config_init(void);
void lwip_periodic_handle(void);
void lwip_dhcp_process_handle(void);

extern uint32_t sys_rand(void);
#endif
