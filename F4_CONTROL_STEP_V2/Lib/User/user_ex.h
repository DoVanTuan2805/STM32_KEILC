#ifndef USER_EX_H
#define USER_EX_H
#include "stdbool.h"
#include "stdint.h"

#define _CHEATER 0.1
#define ROUND 360.0

#define AR_STORE_SETTING 0x08040000
#define AR_STORE_ROTATION 0x08060000

#define NUM_OF_SCREEN 8
typedef enum
{
    CHEATER_SCREEN,

    FLOOR_MAIN_SCREEN,
    ANGLE_MAIN_SCREEN,

    ENCODER_ROTATION_SCREEN,
    HAND_ROTATION_SCREEN,

    INPUT_TOTAL_SLOT_SCREEN,
    INPUT_TOTAL_PAGE_SCREEN,
    SETUP_SCREEN,
} Screen_t;

typedef struct
{
    char Pulse[6];
    uint8_t numIndex;
    uint8_t Speed;
    uint8_t totalPage;

} __attribute__((packed)) dataSaveFlash_t;

typedef struct
{
    uint8_t indexAngle;
    uint8_t rotation_1;
    uint8_t rotation_2;
    uint8_t rotation_3;
    uint8_t rotation_4;
    uint8_t rotation_5;
} __attribute__((packed)) dataAngleInMain_t;

typedef struct
{
    bool changePulse;
    bool changeSpeed;
    bool changeNumIndex;
    bool LoopMode; // STATUS LOOP MODE

    char arraySpeed[2];
    char arrayNumIndex[2];
    char arrayPulse[6];

    uint8_t indexArraySpeed;
    uint8_t indexArrayPulse;
    uint8_t indexArrayNumI;

    uint8_t speed;
    uint8_t numIndex;
    uint8_t totalPage;
    uint64_t pulse;
} __attribute__((packed)) dataSetup_t;

static uint8_t u8COLUMS = 24;
typedef struct
{
    uint8_t totalPage;
    uint8_t page;
    uint8_t indexArrRCur;
    uint8_t **dataRotation;
} __attribute__((packed)) dataPage_t;

typedef struct
{
    bool changeCreatPage;

    char strTotalPage[1];
    uint8_t indexStrTotalPage; // index array string Total page

    char strRotation[2];
    uint8_t indexStrRotation;

    uint8_t indexSlotInRotation; // INDEX SLOT (1 -> 3)
    uint8_t indexPageInRotation; // INDEX PAGE (nPage /  3)
} __attribute__((packed)) dataSetupRotation_t;

typedef struct
{
    bool bKeypadEncoder;
    char arrayRotEn[2];
    uint8_t indexRotEn;

    uint8_t indexPageInSetup; // SETTING
    uint8_t indexSlotInSetup; // SETTING

    uint8_t indexFloorInRotation; // SHOW

    uint8_t IndexRotation; // EX: 1 -> (32, 64, 96)
    float RotationInTFT;   // SHOW
    float fCheater;
    dataSetupRotation_t dataSetupRotation;
    dataAngleInMain_t dataAngleInMain;
    dataSetup_t dataSetup;
} __attribute__((packed)) dataUser_t;

#endif
