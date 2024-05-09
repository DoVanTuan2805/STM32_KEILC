#include "tcp_client.h"

err_t check_tcp_client;
uint8_t is_tcp_connect,wait_connect;
//uint8_t ipaddr_dest[4]={192,168,1,7};
uint8_t ipaddr_dest[4] = {169,254,58,141};
uint16_t port_dest=8000;
struct tcp_pcb *client_pcb;
__IO uint32_t message_count=0;
u8_t data[100];
struct client_struct *cs;

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	struct client_struct *es = NULL;
	wait_connect=0;
	if (err == ERR_OK)
  {
		es = (struct client_struct *)mem_malloc(sizeof(struct client_struct));
		if (es != NULL)
		{
			es->state = ES_CONNECTED;
			es->pcb = tpcb;
			es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)data) , PBUF_POOL);
			if (es->p_tx)
			{
				/* copy data to pbuf */
				pbuf_take(es->p_tx, (char*)data, strlen((char*)data));
				/* pass newly allocated es structure as argument to tpcb */
				tcp_arg(tpcb, es);
				/* initialize LwIP tcp_recv callback function */
				tcp_recv(tpcb, tcp_client_recv);
				/* initialize LwIP tcp_sent callback function */
				tcp_sent(tpcb, tcp_client_sent);
				/* initialize LwIP tcp_poll callback function */
				tcp_poll(tpcb, tcp_client_poll, 1);
				/* send data */
				tcp_client_send(tpcb,es);
				is_tcp_connect=1;
				return ERR_OK;
			}
		}
		else
		{
			tcp_client_connection_close(tpcb, es);
			return ERR_MEM;
		}
  }
  else
  {
    tcp_client_connection_close(tpcb, es);
  }
  return err;
}
void net_cmd(char* buf_str)
{

}
static void tcp_error(void * arg, err_t err)
{
	wait_connect=0;
}
void tcp_client_connect(void) {
    ip_addr_t DestIPaddr;
    client_pcb = tcp_new();
    if (client_pcb != NULL) {
        IP4_ADDR(&DestIPaddr, ipaddr_dest[0], ipaddr_dest[1], ipaddr_dest[2], ipaddr_dest[3]);
        check_tcp_client = tcp_connect(client_pcb, &DestIPaddr, port_dest, tcp_client_connected);
        tcp_err(client_pcb, tcp_error);
    }
}
void tcp_client_disConnect(void)
{
	tcp_recv(client_pcb, NULL);
	tcp_sent(client_pcb, NULL);
	tcp_poll(client_pcb, NULL,0);
	tcp_close(client_pcb);
}
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	struct client_struct *es;
  err_t ret_err;
	es = (struct client_struct *)arg;
	if (p == NULL)
	{
    es->state = ES_CLOSING;
		if(es->p_tx == NULL)
		{
			tcp_client_connection_close(tpcb, es);
		}
		ret_err = ERR_OK ;
	}
	else if(err != ERR_OK)
	{
		if (p != NULL)
		{
			pbuf_free(p);
		}
		ret_err = err;
	}
	else if(es->state == ES_CONNECTED)
	{
		message_count++;
		tcp_recved(tpcb, p->tot_len);
		es->p_tx = p;
		char buff[50];
		strncpy(buff,es->p_tx->payload,es->p_tx->len);
		buff[es->p_tx->len] = '\0';
		Uart_Printf(buff);
		ret_err = ERR_OK;
	}
	else if (es->state == ES_RECEIVED)
	{
    ret_err = ERR_OK;
	}
	else
	{
	  /* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);
		/* free pbuf and do nothing */
		pbuf_free(p);
		ret_err = ERR_OK;
	}
  return ret_err;
}

//----------------------------------------------------------
static void tcp_client_send(struct tcp_pcb *tpcb, struct client_struct * es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;
  while ((wr_err == ERR_OK) &&
  (es->p_tx != NULL) &&
  (es->p_tx->len <= tcp_sndbuf(tpcb)))
  {
    ptr = es->p_tx;
		wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
		if (wr_err == ERR_OK)
		{
			es->p_tx = ptr->next;
			if(es->p_tx != NULL)
			{
				pbuf_ref(es->p_tx);
			}
			pbuf_free(ptr);
		}
		else if(wr_err == ERR_MEM)
		{
      es->p_tx = ptr;
		}
		else
		{
		/* other problem ?? */
		}
  }
}
//----------------------------------------------------------
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct client_struct *es;
  LWIP_UNUSED_ARG(len);
  es = (struct client_struct *)arg;
  if(es->p_tx != NULL)
  {
    tcp_client_send(tpcb, es);
  }
  return ERR_OK;
}
//----------------------------------------------------------
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
	struct client_struct *es;
	es = (struct client_struct*)arg;
	if (es != NULL)
	{
	  if (es->p_tx != NULL)
		{

		}
		else
		{
			if(es->state == ES_CLOSING)
			{
				tcp_client_connection_close(tpcb, es);

			}
		}
		ret_err = ERR_OK;
	}
	else
	{
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}
  return ret_err;
}
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client_struct * es)
{
  /* remove callbacks */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL,0);
  if (es != NULL)
  {
    mem_free(es);
  }
  /* close tcp connection */
  tcp_close(tpcb);
	is_tcp_connect=0;
}
void sendstring(char* buf_str)
{
	tcp_sent(client_pcb, tcp_client_sent);
  tcp_write(client_pcb, (void*)buf_str, strlen(buf_str), 1);
  tcp_output(client_pcb);
}