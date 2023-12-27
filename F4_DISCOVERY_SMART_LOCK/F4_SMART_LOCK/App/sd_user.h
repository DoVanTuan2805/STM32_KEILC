#ifndef __SD_USER_H
#define __SD_USER_H

#include "fatfs.h"
#include "fatfs_sd.h"

static char *fileTimeName = "dataTime.txt";

void deleteFile(char *name);
void writeTimeToSD(char *buff);

#endif