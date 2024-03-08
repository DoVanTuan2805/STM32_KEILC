#ifndef SAVE_TO_FLASH_H
#define SAVE_TO_FLASH_H
#include "user_ex.h"
void comfirmDataSetting(void);
void writeDataSetting(dataSaveFlash_t dataWrite);
void readDataSetting(dataSaveFlash_t *dataRead);
void refreshDataSetting(void);
#endif