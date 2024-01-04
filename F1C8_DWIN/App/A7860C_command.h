#ifndef _A7680_COMMAND_H
#define _A7680_COMMAND_H

#define S1 ","
#define S2 "\n"
#define SIM_OK				"OK"

#define A7680_INFOR 			"ATI\n"
#define A7680_ECHO(x) 		"ATE" + x + "\n"
#define CHECK_SIM 				"AT+CPIN?\n"
#define SIGNAL_QUALITY 		"AT+CSQ\n"
#define READ_VOLT 				"AT+CBC\n"

/*--------------------SMS (PAGE 202) -----------------------*/		

/*--------------------MQTT (PAGE 361) -----------------------*/		
#define DISCON_MQTT(x, y) 			"AT+CMQTTDISC=" + x + "," + y "\n"

#define CON_TO_MQTT(client_id, server_addr, keep_alive, clean_session) \										
"AT+CMQTTCONNECT=" + client_id + S1 + server_addr + S1 + keep_alive + S1 + clean_session + S2

#define CHECK_CON_MQTT					"AT+CMQTTCONNECT=?\n"
#define CHECK_CON_MQTT					"AT+CMQTTCONNECT=?\n"


#endif