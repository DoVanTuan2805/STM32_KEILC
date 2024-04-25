#include "parse_data.h"


char *parseData(char *uartBuffer, uint8_t pos) {
    char *data;
    char *argv[10];
    uint8_t argvNum = 0;
    data = strtok((char *)uartBuffer, " ");

    while (data != NULL) {
        argv[argvNum] = data;
        data = strtok(NULL, " ");
        argvNum++;
    }
    if (pos > argvNum) {
        return "NULL";
    }
    return argv[pos];
}