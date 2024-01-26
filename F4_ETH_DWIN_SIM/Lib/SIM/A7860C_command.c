#include "A7860C_command.h"
#include "usart.h"
#include "main.h"
#include "string.h"
#include "stdio.h"
static UART_HandleTypeDef *huart;
extern MQTT_Sub_t mqttSub;					/*	------------ FROM FREERTOS ------------*/
/*-----------CALLBACK MESSAGE DATA ---------*/
mqtt_callback_t mqtt_callback_mess;

void MQTTsetCallbackMess(mqtt_callback_t callback)
{
		mqtt_callback_mess = callback;
}

/*-----------CALLBACK MESSAGE DATA ---------*/

uint16_t indexRx;
uint8_t dataRx;
char bufferRx[128];

static uint64_t timeRespond, timeRespondNow;
static uint64_t timeMount, timeMountNow;
static bool flagRx = false;
char command[128];

char *bufferSub;
void delay(uint16_t ms)
{
	HAL_Delay(ms);
}
void SIM_init(UART_HandleTypeDef *huartx)
{
		huart = huartx;
		HAL_UART_Receive_DMA(huart, &dataRx, 1);
}
void uartSend(char *dataSend)
{
		HAL_UART_Transmit_DMA(huart, (uint8_t*)dataSend, strlen(dataSend));
}
void uartReceive()					// INTERRUPT DMA
{
		flagRx = true;
		bufferRx[indexRx++] = dataRx;		
		HAL_UART_Receive_DMA(huart, &dataRx, 1);
}
void clearRxBuffer(void)
{
    indexRx = 0;
    memset(bufferRx, '\0', strlen(bufferRx));
}
uint8_t sendCommand(char *cmd, char *respond1, char *respond2,uint16_t timeout)
{
		uartSend(cmd);
		delay(10);
		timeRespond = HAL_GetTick();
		while(1)
		{
				timeRespondNow = HAL_GetTick();
				if(flagRx == true)
				{
						flagRx = false;
						if(strstr(bufferRx, respond1) != NULL)
						{

								clearRxBuffer();
								return 0;
								
						}
						else if (strstr(bufferRx, respond2) != NULL)
						{
								clearRxBuffer();
								return 0;
								
						}
				}
				if(timeRespondNow - timeRespond >= timeout)
				{
					
					break;
				}
		}
		clearRxBuffer();
		return 1;
		
}
uint8_t disConnectMQTT(uint8_t clientIndex, uint8_t timeout)
{
	sprintf(command , DISMQTT"%d,%d\r\n", clientIndex, timeout);
	return sendCommand(command, OK, ERR, TIMEOUT);
}
uint8_t releaseClientMQTT(uint8_t clientIndex)
{
	sprintf(command , RELEASE_CLIENT"%d\r\n", clientIndex);
	return sendCommand(command, OK, ERR, TIMEOUT);
}
uint8_t stopServiceMQTT(void)
{
	sprintf(command , MQTT_STOP"\r\n");
	return sendCommand(command, OK, ERR, TIMEOUT);
}
uint8_t startServiceMQTT(void)
{
	sprintf(command , MQTT_START"\r\n");
	return sendCommand(command, OK, COMPLETE_START, TIMEOUT);
}
uint8_t acquireClientMQTT(uint8_t clientIndex, char *clientID, uint8_t serverType)
{
	sprintf(command , ACQUIRE_CLIENT"%d,\"%s\",%d\r\n", clientIndex, clientID, serverType);
	return sendCommand(command, OK, "", TIMEOUT);;
}

/*
	cleanSession 	: 1 CLEAN - 0 No CLEAN
	keepAlive 		: The range is from 1s to 64800s
*/

uint8_t connectMQTT(uint8_t clientIndex, char *clientAdd, uint16_t port,  uint8_t keepAlive, uint8_t cleanSession)
{	
		if(clientIndex > 1 )
		{
			return 1;
		}
		sprintf(command, "AT+CMQTTCONNECT=%d,\"tcp://%s:%d\",%d,%d\r\n", clientIndex, clientAdd, port, keepAlive, cleanSession);
		
		return sendCommand(command, OK, "+CMQTTCONNECT" ,TIMEOUT);
}


uint8_t mountMQTT(MQTT_Infor_t MQTT_Infor)
{
		static uint8_t checkRepond;
		timeMount = HAL_GetTick();
		while(timeMountNow - timeMount >= 10000)
		{
			timeMountNow = HAL_GetTick();
			checkRepond = sendCommand(AT, OK, OK, TIMEOUT);
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			delay(50);
			checkRepond = sendCommand(CHECK_SIM, SIM_READY, SIM_READY, TIMEOUT);
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			delay(50);
			checkRepond = disConnectMQTT(MQTT_Infor.clientIndex, 60);
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			delay(50);
			checkRepond = releaseClientMQTT(0);
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			delay(50);
			checkRepond = stopServiceMQTT();
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			delay(50);
			checkRepond = startServiceMQTT();
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			delay(50);
			checkRepond = acquireClientMQTT(MQTT_Infor.clientIndex, MQTT_Infor.clientID, 0);
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			delay(50);
			
			checkRepond = connectMQTT(MQTT_Infor.clientIndex,
																MQTT_Infor.sever, 
																MQTT_Infor.port,
																MQTT_Infor.keepAlive , 
																MQTT_Infor.cleanSession);
			
			delay(100);
			if(checkRepond == 1)		goto EXIT;
			clearRxBuffer();
			
			return 0;
		}
		EXIT:
		return 1;
}


uint8_t pingMQTT(MQTT_Infor_t MQTT_Infor)
{
		char *data = malloc(128 * sizeof(char));
		sprintf(data, "+CMQTTCONNECT: %d,\"tcp://%s:%d\",%d,%d", MQTT_Infor.clientIndex, MQTT_Infor.sever, MQTT_Infor.port, MQTT_Infor.keepAlive, MQTT_Infor.cleanSession);
		if(sendCommand(PING_MQTT, data, data, TIMEOUT) == 0)
		{
				free(data);
				return 0;
		}
		else 
		{
				free(data);
				return 1;
		}
}


uint8_t subcribeTopic(MQTT_Sub_t MQTT_Sub)
{
	uint8_t check;
	
	if(sizeof(MQTT_Sub.topic) < 1)
	{
		return 1;
	}
	if(MQTT_Sub.Qos > 1 )
	{
		return 1;
	}
	memset(command, '\0',strlen(command));
	sprintf(command, SUB_TOPIC"%d,%d,%d\r\n", MQTT_Sub.clientIndex, strlen(MQTT_Sub.topic), MQTT_Sub.Qos);	
	check = sendCommand(command, ">", ">", TIMEOUT);
	if(check == 1)			goto OUT;
	clearRxBuffer();
	delay(100);
	
	memset(command, '\0',sizeof(command));
	delay(100);
	sprintf(command, "%s\r\n", MQTT_Sub.topic);
	check = sendCommand(command, OK, OK , TIMEOUT);
	if(check == 1)			goto OUT;
	clearRxBuffer();
	delay(100);

	check = sendCommand(COMPLETE_SUB, OK, OK, TIMEOUT);
	if(check == 1)			goto OUT;
	delay(TIMEOUT);
	clearRxBuffer();
	return 0;
	
	OUT:
	return 1;
}
uint8_t unSubcribeTopic(MQTT_Sub_t MQTT_Sub)
{
	uint8_t check;
	
	if(sizeof(MQTT_Sub.topic) < 1)
	{
		return 1;
	}
	if(MQTT_Sub.Qos > 1 )
	{
		return 1;
	}
	memset(command, '\0',strlen(command));
	sprintf(command, UNSUB_TOPIC"%d,%d\r\n", MQTT_Sub.clientIndex, strlen(MQTT_Sub.topic));	
	check = sendCommand(command, ">", ">", TIMEOUT);
	if(check == 1)			goto OUT;
	clearRxBuffer();
	delay(100);
	
	memset(command, '\0',sizeof(command));
	delay(100);
	sprintf(command, "%s\r\n", MQTT_Sub.topic);
	check = sendCommand(command, OK, OK , TIMEOUT);
	if(check == 1)			goto OUT;
	clearRxBuffer();
	delay(100);

	check = sendCommand(COMPLETE_UNSUB, OK, OK, TIMEOUT);
	if(check == 1)			goto OUT;
	delay(TIMEOUT);
	clearRxBuffer();
	return 0;
	
	OUT:
	return 1;
}

MQTT_Receive_t mqttReceive;
int lengthTopic, lengthPayload;
char *topic; 
char *payload;  
void SimLoop(void)
{
		if(flagRx == true)
		{
				flagRx = false;	
				char *str = malloc(35 * sizeof(char));
				topic = malloc(100 * sizeof(char));
				payload = malloc(100 * sizeof(char));
				if(strstr(bufferRx, "+CMQTTRXSTART:") != NULL)
				{
						if(strstr(bufferRx, "+CMQTTRXTOPIC:") != NULL)
						{
								if(strstr(bufferRx, "+CMQTTRXPAYLOAD:") != NULL)
								{
										if(strstr(bufferRx, "+CMQTTRXEND: 0") != NULL)
										{
												Uart_Printf(bufferRx);
												sscanf(bufferRx, "\r\n+CMQTTRXSTART: 0,%d,%d", &lengthTopic, &lengthPayload);
												
												char *topicStart = strstr(bufferRx, "STM32_SUB_DATA");
												strncpy(topic, topicStart, lengthTopic);											// GET TOPIC 
											
												sprintf(str , "+CMQTTRXPAYLOAD: 0,%d\r\n", lengthPayload);
												char *payloadStart = strstr(bufferRx, str);			
												payloadStart += strlen(str);
												
												strncpy(payload, payloadStart, lengthPayload);
											
												mqttReceive.lengthTopic = lengthTopic;
												mqttReceive.topic = topic;
											
												mqttReceive.lengthPayload = lengthPayload;
												mqttReceive.payload = payload;
												
												mqtt_callback_mess(mqttReceive.topic, mqttReceive.payload);			/// SET CALL BACK MESSAGE
												clearRxBuffer();
										}
								}
						}	
				}
				free(str);
				free(topic);
				free(payload);
		}
}


uint8_t publishTopic(uint8_t clientIndex, char* Topic , uint16_t lengthTopic, char *payLoad);
