#include "saveToFlash.h"
#include "stm32f4xx_hal.h"
#include "Flash_user.h"
#include "stdlib.h"
#include "stdio.h"
#include "ILI9431_user.h"
#include "step_user.h"
extern dataUser_t dataUserInput;
extern dataSaveFlash_t dataWriteFlash, dataReadFlash;

void comfirmDataSetting(void)
{
    sprintf(dataWriteFlash.Pulse, "%llu", dataUserInput.dataSetup.pulse);
    dataWriteFlash.Speed = dataUserInput.dataSetup.speed;
    dataWriteFlash.numIndex = dataUserInput.dataSetup.numIndex;
    HAL_Delay(10);
    writeDataSetting(dataWriteFlash);
    readDataSetting(&dataReadFlash);

    refreshDataSetting();
}
void writeDataSetting(dataSaveFlash_t dataWrite)
{
    Flash_Write_Struct_Setting(AR_STORE_SETTING, dataWrite);
    HAL_Delay(50);
}
void readDataSetting(dataSaveFlash_t *dataRead)
{
    Flash_Read_Struct_Setting(AR_STORE_SETTING, dataRead, sizeof(dataSaveFlash_t));
    HAL_Delay(50);
}
void refreshDataSetting(void)
{
    dataUserInput.dataSetup.speed = dataReadFlash.Speed;
    dataUserInput.dataSetup.pulse = atoi(dataReadFlash.Pulse);

    // sscanf(dataReadFlash.Pulse, "%d", &dataUserInput.dataSetup.pulse);
    dataUserInput.dataSetup.numIndex = dataReadFlash.numIndex;

    dataUserInput.IndexRotation = dataUserInput.dataSetup.numIndex;    //(32, 64, 96)
    dataUserInput.RotationInTFT = ROUND / dataUserInput.IndexRotation; // 360

    initStep();
}