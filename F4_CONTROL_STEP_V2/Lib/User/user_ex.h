#ifndef USER_EX_H
#define USER_EX_H
#include "stdbool.h"
#include "stdint.h"
static float ROUND = 360;

// #define ANGLE_ON

#define ADDRESS_STORAGE 0x08060000

#define NUM_OF_SCREEN 11
typedef enum
{
    CHEATER_SCREEN,

    FLOOR_MAIN_SCREEN,
    FLOOR_SETUP_SCREEN,

    ANGLE_MAIN_SCREEN,
    ANGLE_SETUP_SCREEN,

    ENCODER_ROTATION_SCREEN,
    HAND_ROTATION_SCREEN,

    SELECT_CREAT_MODIFY_SCREEN,
    CREATE_ROTATION_SCREEEN,
    MODIFY_ROTATION_SCREEN,

    SETUP_SCREEN,
} Screen_t;

#define MAX_SPEED 100
#define MIN_SPEED 0
typedef struct
{
    char Speed[3];
    char Pulse[6];
} dataSaveFlash_t;

typedef struct
{
    uint8_t indexAngle;
    float rotation_1;
    float rotation_2;
    float rotation_3;
    float rotation_4;
    float rotation_5;
} dataAngleInMain_t;

typedef struct
{
    bool changePulse;
    bool changeSpeed;
    bool LoopMode; // STATUS LOOP MODE

    uint8_t indexArraySpeed;
    char arraySpeed[3];

    uint8_t indexArrayPulse;
    char arrayPulse[6];

    uint8_t speed;
    uint64_t pulse;
} dataSetup_t;

typedef struct
{
    uint8_t indexPage;
    uint8_t nSlot;
    char **dataRotation;
} dataPage_t;

typedef struct
{
    bool changeCreatPage;
    bool changeModifyPage;

    uint8_t nPage;
    char strPage[2];
    uint8_t indexStrPage;

    uint8_t nSlot;
    char strSlot[2];
    uint8_t indexStrSlot;

    uint8_t indexSlotUser;

    uint8_t indexSlotInRotation; // INDEX SLOT (1 -> 3)
    uint8_t indexPageInRotation; // INDEX PAGE (nPage /  3)
    dataPage_t page;
} dataSetupRotation_t;

typedef struct
{

    bool bCheater;
    bool ReleaseStepB;

    uint8_t indexPageInSetup; // SETTING
    uint8_t indexSlotInSetup; // SETTING

    uint8_t indexFloorInRotation; // SHOW

    uint8_t indexCreateModify; // SELECT CREATE OR MODIFY
    uint8_t indexCreate;
    uint8_t indexModify;

    uint16_t RotationInTFT; // SHOW
    uint16_t IndexRotation; // EX: 1 -> (32, 64, 96)

    float fCheater;
    dataSetupRotation_t dataSetupRotation;
    dataAngleInMain_t dataAngleInMain;
    dataSetup_t dataSetup;
} dataUser_t;

#endif
