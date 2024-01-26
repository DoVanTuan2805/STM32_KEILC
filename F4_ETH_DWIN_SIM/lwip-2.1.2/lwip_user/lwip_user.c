#include "lwip_user.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/udp.h"
#include "lwip/icmp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h" 
#include "lwip/timeouts.h"
#include "enc28j60.h"
#include "ethernetif.h"
#include <stdio.h>
#if defined(__CC_ARM) || defined(__ARMCC_VERSION) /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */


//-------Neu khong su dung DHCP thi hay define ip vao day--------

//---------------------------------------------------------------

#define LWIP_MAX_DHCP_TRIES 4					
struct netif lwip_netif;			

#define lwip_localtime HAL_GetTick()		

uint32_t TCPTimer=0;		
uint32_t ARPTimer=0;		

extern const uint8_t macaddr[6];
	


uint8_t lwip_config_init(void)
{
	struct netif *Netif_Init_Flag;		
	ip4_addr_t ipaddr;  			
	ip4_addr_t netmask; 		
	ip4_addr_t gw;      			
	mem_init();
	memp_init();
	lwip_init();

#if LWIP_DHCP		//Su dung dhcp
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				
	IP4_ADDR(&ipaddr,localIP[0],localIP[1],localIP[2],localIP[3]);
	IP4_ADDR(&netmask,subnetmask[0],subnetmask[1] ,subnetmask[2],subnetmask[3]);
	IP4_ADDR(&gw,gatewayIP[0],gatewayIP[1],gatewayIP[2],gatewayIP[3]);
#endif
	Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);
	if(Netif_Init_Flag==NULL)return 3;
	else
	{
		netif_set_default(&lwip_netif); 
		if (netif_is_link_up(&lwip_netif))
		{
			/* When the netif is fully configured this function must be called */
      netif_set_up(&lwip_netif);
		}
		else
		{
			/* When the netif link is down this function must be called */
      netif_set_down(&lwip_netif);
		}
	}
	/* Set the link callback function, this function is called on change of link status*/
  //netif_set_link_callback(&gnetif, ethernetif_update_config);
#if LWIP_DHCP			//Su dung dhcp
	/* Start DHCP negotiation for a network interface (IPv4) */
	dhcp_start(&lwip_netif);
#endif
	char buff[50];
	sprintf(buff,"IP: %i.%i.%i.%i\r\n",localIP[0],localIP[1],localIP[2],localIP[3]);
	Uart_Printf(buff);
	sprintf(buff,"Gw: %i.%i.%i.%i\r\n",gatewayIP[0],gatewayIP[1],gatewayIP[2],gatewayIP[3]);
	Uart_Printf(buff);
	sprintf(buff,"Mask: %i.%i.%i.%i\r\n",subnetmask[0],subnetmask[1],subnetmask[2],subnetmask[3]);
	Uart_Printf(buff);
	return 0;//OK
}

char ETH_IPOK(void)
{
	if(lwip_netif.ip_addr.addr != 0)
	{
		localIP[0]=lwip_netif.ip_addr.addr&0x000000FF;
		localIP[1]=(lwip_netif.ip_addr.addr>>8)&0x000000FF;
		localIP[2]=(lwip_netif.ip_addr.addr>>16)&0x000000FF;
		localIP[3]=(lwip_netif.ip_addr.addr>>24)&0x000000FF;
		
		gatewayIP[0]=lwip_netif.netmask.addr&0x000000FF;
		gatewayIP[1]=(lwip_netif.netmask.addr>>8)&0x000000FF;
		gatewayIP[2]=(lwip_netif.netmask.addr>>16)&0x000000FF;
		gatewayIP[3]=(lwip_netif.netmask.addr>>24)&0x000000FF;
		
		subnetmask[0]=	lwip_netif.gw.addr 				& 0x000000FF;
		subnetmask[1]=	(lwip_netif.gw.addr>>8) 	& 0x000000FF;
		subnetmask[2]=	(lwip_netif.gw.addr>>16) 	& 0x000000FF;
		subnetmask[3]=	(lwip_netif.gw.addr>>24) 	& 0x000000FF;
		return 1;
	}
	else return 0;
}

void lwip_periodic_handle(void)
{
	ethernetif_input(&lwip_netif );
	sys_check_timeouts();
}

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

/* USER CODE BEGIN 7 */
  sd = 0; // dummy code
/* USER CODE END 7 */

  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
/* USER CODE BEGIN 8 */
/* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */
  recved_bytes = 0; // dummy code
/* USER CODE END 9 */
  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 10 */
  recved_bytes = 0; // dummy code
/* USER CODE END 10 */
  return recved_bytes;
}
#endif /* MDK ARM Compiler */


uint32_t sys_get_tick_ms(void)
{
  return HAL_GetTick();
}
uint32_t sys_now(void)
{
	return sys_get_tick_ms();
}
uint32_t sys_jiffies(void)
{
  return sys_get_tick_ms();
}

uint32_t sys_rand()
{
    return SysTick->VAL;
}
