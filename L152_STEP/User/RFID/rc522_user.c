#include "rc522_user.h"
#include "rc522.h"

#include "gpio.h"
#include "spi.h"
#include "usart.h"


#include "string.h"
#include "stdio.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

bool  flagRFID_In = false, flagRFID_Out = false;

uint8_t CardID_I[5], lastCardID_I[5];
uint8_t CardID_II[5], lastCardID_II[5];

char *card1 = "90A04F205F";
char *card2 = "90B3962095";
char *card3 = "F372FB116B";
char *card4 = "4913433D24";
char *card5 = "23ED3CAD5F";

RC522_infor_t rfid_1 = {&hspi3, CS_1_GPIO_Port, CS_1_Pin};
RC522_infor_t rfid_2 = {&hspi3, CS_2_GPIO_Port, CS_2_Pin};

void rc522_init()
{
    TM_MFRC522_Init(&rfid_1);
    TM_MFRC522_Init(&rfid_2);
}

bool read_RC522_In()
{
    if (!TM_MFRC522_Request(&rfid_1, PICC_REQIDL, CardID_I)) {
        if (!TM_MFRC522_Anticoll(&rfid_1, CardID_I)) {
            if (memcmp(lastCardID_I, CardID_I, sizeof(lastCardID_I))) {
                flagRFID_In = true;
                memcpy(lastCardID_I, CardID_I, sizeof(lastCardID_I));
                return true;
            }
        }
    }
    return false;
}
bool read_RC522_Out()
{
    if (!TM_MFRC522_Request(&rfid_2, PICC_REQIDL, CardID_II)) {
        if (!TM_MFRC522_Anticoll(&rfid_2, CardID_II)) {
            if (memcmp(lastCardID_II, CardID_II, sizeof(lastCardID_II))) {
                flagRFID_Out = true;
                memcpy(lastCardID_II, CardID_II, sizeof(lastCardID_II));
                return true;
            }
        }
    }
    return false;
}