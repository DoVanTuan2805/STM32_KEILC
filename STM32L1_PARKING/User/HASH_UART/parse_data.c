#include "parse_data.h"

char *argv[10];
char *parseData(char *uartBuffer, uint8_t pos) {
    char *temp = uartBuffer;
    char *data;
    
    uint8_t argvNum = 0;
    data = strtok((char *)temp, " ");

    while (data != NULL) {
        argv[argvNum] = data;
        data = strtok(NULL, " ");
        argvNum++; 
    }
    return argv[pos];
}