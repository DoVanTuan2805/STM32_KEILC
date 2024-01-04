/*
 * fingerprint.c
 *
 *  Created on: Nov 18, 2021
 *      Author: Steven
 */
// TODO clean up and migrate to new model

#include "fingerprint.h"
//#include "lcd.h"

uint8_t dataRx[50] = {0};
uint8_t lock;

typedef struct 
{
		uint16_t fingerID;
		uint16_t confidence;
}FingerInfor_t;

static FingerInfor_t fingerInfor;

F_Packet fpacket = {
    .start_code = FINGERPRINT_STARTCODE,
    .address = 0xFFFFFFFF,
    .type = FINGERPRINT_COMMANDPACKET,
};

F_Packet rpacket;

uint16_t status_reg = 0x0;    ///< The status register (set by getParameters)
uint16_t system_id = 0x0;     ///< The system identifier (set by getParameters)
uint16_t capacity = 64;       ///< The fingerprint capacity (set by getParameters)
uint16_t security_level = 0;  ///< The security level (set by getParameters)
uint32_t device_addr = 0xFFFFFFFF;              ///< The device address (set by getParameters)
uint16_t packet_len = 64;    ///< The max packet length (set by getParameters)
uint16_t baud_rate = 57600;  ///< The UART baud rate (set by getParameters)

void Uart_Receive_DMA()
{
//		//HAL_UART_Receive(&UART_HANDLER, dataRx, sizeof(dataRx), 100);  // logically hitting all the 9 data
//		//HAL_UARTEx_ReceiveToIdle_DMA(&UART_HANDLER, dataRx, sizeof(dataRx));
//    //__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
//		timeReceiveHanle = HAL_GetTick();
//		HAL_UART_Receive_IT(&UART_HANDLER, dataRx, sizeof(dataRx));
//		lock = 0;
}
void init_fingerprint() {
    uint8_t temp = FINGERPRINT_READSYSPARAM;
    setup_packet(&temp, 1);
    receive();
    // insert init;
}

void setup_packet(uint8_t *data, uint8_t size) {
    // setup the length, datasize, and checksum
    if (size > 64)
        return;

    // length
    uint16_t packet_length = 2 + size;
    fpacket.length = size;  // packet size

    // data and checksum
    uint16_t sum = ((packet_length) >> 8) + ((packet_length)&0xFF) + fpacket.type;

    for (int i = 0; i < size; i++) {
        fpacket.data[i] = data[0];
        sum += *data;
        data++;
    }
    fpacket.checksum = sum;
    //
}

void setup_received(uint8_t *data) {
    // Data format
    /*
     * 2 bytes of header
     * 4 bytes of address
     * 1 byte of package identifier (type)
     * 2 byte of length
     * n amount of data
     * 2 checksum (which technically u dunnit)
     *
     * */
    rpacket.start_code = ((uint16_t)data[0] << 8) + ((uint16_t)data[1]);
    rpacket.address = ((uint32_t)data[2] << 24) + ((uint32_t)data[3] << 16) + ((uint32_t)data[4] << 8) + ((uint32_t)data[5]);
    rpacket.type = data[6];
    rpacket.length = ((uint16_t)data[7] << 8) + ((uint16_t)data[8]);

    // get all the data
    for (int i = 0; i < rpacket.length - 2; i++) {
        rpacket.data[i] = data[9 + i];
    }
}

void send() {
    // first, send start command
    uint8_t value;

    for (int i = 0; i < 2; i++) {
        value = (fpacket.start_code >> (8 * (1 - i)));
        HAL_UART_Transmit(&UART_HANDLER, &value, 1, 100);
    }

    // second, send chip addr
    for (int i = 0; i < 4; i++) {
        value = 0xFF;
        HAL_UART_Transmit(&UART_HANDLER, &value, 1, 100);
    }

    // third, send package identifier
    value = fpacket.type;
    HAL_UART_Transmit(&UART_HANDLER, &value, 1, 100);

    // fourth, send package length
    uint16_t packet_length = fpacket.length + 2;
    for (int i = 0; i < 2; i++) {
        value = (packet_length >> (8 * (1 - i)));
        HAL_UART_Transmit(&UART_HANDLER, &value, 1, 100);
    }

    // fifth, send package data
    for (int i = 0; i < fpacket.length; i++) {
        value = fpacket.data[i];
        HAL_UART_Transmit(&UART_HANDLER, &value, 1, 100);
    }

    // lastly checksum
    for (int i = 0; i < 2; i++) {
        value = (fpacket.checksum >> (8 * (1 - i)));
        HAL_UART_Transmit(&UART_HANDLER, &value, 1, 100);
    }
}

uint8_t receive() {
    send();
//		Uart_Receive_DMA();
//		setup_received(dataRx);
		//if(HAL_GetTick() - timeReceiveHanle > 100)
		//HAL_UART_Receive_DMA(&UART_HANDLER, dataRx, sizeof(dataRx));
		HAL_UART_Receive(&UART_HANDLER, dataRx, sizeof(dataRx), 800);
		setup_received(dataRx);
		memset(dataRx, '\0', sizeof(dataRx));	
}

uint8_t save_fingerprint(uint8_t id) {					// THEM VAN TAY
    uint8_t temp;
		

    // 1. GetImage
    temp = FINGERPRINT_GETIMAGE;
    setup_packet(&temp, 1);
    receive();

    if (rpacket.data[0])
        return rpacket.data[0];

    // 2. Conv image to value image2Tz
    uint8_t data[4] = {FINGERPRINT_IMAGE2TZ, 1};
    setup_packet(data, 2);
    receive();

    if (rpacket.data[0])
        return rpacket.data[0];

    // 3. GetImage
    setup_packet(&temp, 1);
    receive();

    if (rpacket.data[0])
        return rpacket.data[0];

    
    data[1] = 2;
    setup_packet(data, 2);
		receive();
    //receive(fpacket, rpacket);
		// 4. Conv image to value image2Tz
		
    if (rpacket.data[0])
        return rpacket.data[0];
		
    // 5. Create the model
    temp = FINGERPRINT_REGMODEL;
    setup_packet(&temp, 1);
    receive();
    if (rpacket.data[0])
        return rpacket.data[0];

		data[0] = FINGERPRINT_STORE;
		data[1] = 0x01;
		data[2] = 0x00;
		data[3] = (uint8_t)(id & 0xFF);
		setup_packet(data, 4);
		receive();
//	LCD_SetCursor(0, 0);
//	  LCD_Printf("Saved");
    return rpacket.data[0];
}



uint8_t delete_fingerprint(uint8_t id)					// XOA VAN TAY THEO ID
{
		uint8_t data[5];
		data[0] = FINGERPRINT_DELETE;
		data[1] = (uint8_t)(id >> 8);
		data[2] = (uint8_t)(id & 0xFF);
		data[3] = 0x00;
		data[4] = 0x01;
		setup_packet(data, 5);
		receive();
    return rpacket.data[0];
}
uint8_t getImage(void)
{
		uint8_t temp;
		temp = FINGERPRINT_GETIMAGE;
    setup_packet(&temp, 1);
		receive();
		if (rpacket.data[0] == FINGERPRINT_PACKETRECIEVEERR)
		{
				return 1;
		}
		else if (rpacket.data[0] == FINGERPRINT_OK)
		{
				return 0;
		}
		else 
		{
				return 2;
		}
}
uint8_t getImage2Tz(void)
{
		uint8_t data[2] = {FINGERPRINT_IMAGE2TZ, 2};
    setup_packet(data, 2);
    receive();
		if (rpacket.data[0] == FINGERPRINT_PACKETRECIEVEERR)
		{
				return 1;
		}
		else if (rpacket.data[0] == FINGERPRINT_OK)
		{
				return 0;
		}
		else 
		{
				return 2;
		}
}		
uint8_t fingerFastSearch(void)
{
		uint8_t data[6];
		data[0] = FINGERPRINT_HISPEEDSEARCH;
    data[1] = 0x02;
    data[2] = data[3] = 0x00;
    data[4] = 0x00;
    data[5] = 0xA3;
    setup_packet(data, 6);
    receive();	
		if (rpacket.data[0] == FINGERPRINT_NOTFOUND)
		{
				return 1;
		}
		else if(rpacket.data[0] == FINGERPRINT_PACKETRECIEVEERR)
		{
				return 2;
		}
		else 
		{
			fingerInfor.fingerID = 0xFFFF;
			fingerInfor.confidence = 0xFFFF;

			fingerInfor.fingerID = rpacket.data[1];
			fingerInfor.fingerID <<= 8;
			fingerInfor.fingerID |= rpacket.data[2];

			fingerInfor.confidence = rpacket.data[3];
			fingerInfor.confidence <<= 8;
			fingerInfor.confidence |= rpacket.data[4];
			
			memset(dataRx, '\0', sizeof(dataRx));	
			return 0;
		}
}	
uint16_t getFingerID(void)				// ID TRONG BO NHO ROM
{
	return fingerInfor.fingerID;
}
uint16_t getConfidence(void)				// 
{
	return fingerInfor.confidence;
}
void led_mode(uint8_t control) {
		uint8_t temp;
    if ((control == 0) | (control == 1)) {
				if(control == 1)
				{
						temp = FINGERPRINT_LEDON;
				}
				else 
				{
						temp = FINGERPRINT_LEDOFF;
				}
        setup_packet(&temp, 1);
        receive();
    }
}

uint16_t get_template_number() {
    uint8_t temp;
    temp = FINGERPRINT_TEMPLATECOUNT;
    setup_packet(&temp, 1);
    receive();
    return (((uint16_t)rpacket.data[1]) << 8) + ((uint16_t)rpacket.data[2]);
}

void reset_database() {				// XOA HET VAN TAY
    uint8_t temp;
    // 1. GetImage
    temp = FINGERPRINT_EMPTY;
    setup_packet(&temp, 1);
    send();
}

