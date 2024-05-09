#ifndef _ROW_API_TCP_CLIENT_H_
#define _ROW_API_TCP_CLIENT_H_
//-----------------------------------------------
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lwip_user.h" 
#include "lwip/tcp.h"
//-----------------------------------------------
#include "uart.h"
//-----------------------------------------------
enum client_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};
struct client_struct
{
  enum client_states state; /* connection status */
  struct tcp_pcb *pcb; /* pointer on the current tcp_pcb */
  struct pbuf *p_tx; /* pointer on pbuf to be transmitted */
};
void net_ini(void);
void net_cmd(char* buf_str);
void tcp_client_connect(void);
void tcp_client_disConnect(void);
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_client_send(struct tcp_pcb *tpcb, struct client_struct * es);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client_struct * es);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
void sendstring(char* buf_str);

#endif /* _ROW_API_TCP_CLIENT_H_ */