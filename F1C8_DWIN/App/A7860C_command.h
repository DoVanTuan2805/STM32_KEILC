#ifndef _A7680_COMMAND_H
#define _A7680_COMMAND_H

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#define TIMEOUT				1000
#define OK								"OK"
#define ERR								"ERROR"
#define SIM_READY					"+CPIN: READY"

#define AT 								"AT\r\n"
#define INFOR 						"ATI\r\n"
#define ENABLE_ECHO 			"ATE0\r\n"
#define DISABLE_ECHO 			"ATE1\r\n"
#define CHECK_SIM 				"AT+CPIN?\r\n"
#define SIGNAL_QUALITY 		"AT+CSQ\r\n"
#define BATERRY						"AT+CBC\r\n"
#define RESET_SIM 				"AT+CRESET\r\n"
#define IMEI 							"AT+CIMI\r\n"		

/*----------------------- MQTT (PAGE 361) -----------------------*/
#define DISMQTT 					"AT+CMQTTDISC="
#define RELEASE_CLIENT 		"AT+CMQTTREL="
#define MQTT_STOP 				"AT+CMQTTSTOP"
#define MQTT_START 				"AT+CMQTTSTART"
#define COMPLETE_START		"+CMQTTSTART: 0\r\n"

/*
	AT+CMQTTACCQ is used to acquire a MQTT client. 
	It must be called before all commands about MQTT connect and after AT+CMQTTSTART.
*/
#define ACQUIRE_CLIENT		"AT+CMQTTACCQ="
#define CON_MQTT					"AT+CMQTTCONNECT="
#define PING_MQTT					"AT+CMQTTCONNECT?\r\n"

#define SUB_TOPIC					"AT+CMQTTSUBTOPIC="
#define COMPLETE_SUB			"AT+CMQTTSUB=0\r\n"
#define PUB_TOPIC					"AT+CMQTTSUBTOPIC="
/*----------------------- END MQTT (PAGE 361) -----------------------*/


typedef struct
{
		uint8_t clientIndex;
		char* sever;
		uint16_t port;
		char* clientID;
		uint16_t keepAlive;
		uint8_t cleanSession;
} MQTT_Infor_t;

typedef struct
{
		char* topic;
		uint8_t clientIndex;
		uint8_t Qos;
} MQTT_Sub_t;

typedef struct
{
		uint8_t lengthTopic;
		char* topic;
		uint16_t lengthPayload;
		char* payload;
} MQTT_Receive_t;


void uartSend(char *data);
void uartReceive();
uint8_t sendCommand(char *cmd, char *respond1, char *respond2,uint16_t timeout);


uint8_t disConnectMQTT(uint8_t clientIndex, uint8_t timeout);
uint8_t releaseClientMQTT(uint8_t clientIndex);
uint8_t stopServiceMQTT(void);
uint8_t startServiceMQTT(void);
uint8_t acquireClientMQTT(uint8_t clientIndex, char *clientID, uint8_t serverType);
uint8_t connectMQTT(uint8_t clientIndex, char *clientAdd, uint16_t port,  uint8_t keepAlive, uint8_t cleanSession);
uint8_t mountMQTT(MQTT_Infor_t MQTT_Infor);
uint8_t pingMQTT(MQTT_Infor_t MQTT_Infor);

uint8_t subcribeTopic(MQTT_Sub_t MQTT_Sub);
void handleDataSub();

uint8_t publishTopic(uint8_t clientIndex, char* Topic , uint16_t lengthTopic, char *payLoad);



/*--------------------SMS (PAGE 202) -----------------------*/

#endif
