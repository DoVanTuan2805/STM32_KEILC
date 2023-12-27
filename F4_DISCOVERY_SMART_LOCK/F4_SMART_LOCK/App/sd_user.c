#include "sd_user.h"
#include "string.h"
#include "usart.h"

extern UART_HandleTypeDef huart3;

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

#define BUFFER_SIZE 1028
char buffer[BUFFER_SIZE];  // to store strings..


void send_uart (char *string)
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart3, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}

void writeTimeToSD(char *buffTime)
{
		fresult = f_mount(&fs, "/", 1);
  	if (fresult == FR_OK) 
			send_uart ("SD CARD MOUNTED successfully...\n");

		fresult = f_open(&fil, fileTimeName, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);				// CREAT FILE
		f_close(&fil);
		
		/* READ FILE */
		fresult = f_open(&fil, fileTimeName, FA_READ);					
		f_gets(buffer, f_size(&fil), &fil);
		send_uart(buffer);
		memset(buffer, '\0', sizeof(buffer));			// clear buffer
		f_close(&fil);
		
		/*	UPDATE FILE */
  	fresult = f_open(&fil, fileTimeName, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
  	fresult = f_lseek(&fil, f_size(&fil));						// MOVE TO END FILE
  	fresult = f_puts(buffTime, &fil);									// PUT DATA
  	f_close (&fil);
  	memset(buffer, '\0', sizeof(buffer));			// clear buffer
		
  	/* Unmount SDCARD */
  	fresult = f_mount(NULL, "/", 1);
  	if (fresult == FR_OK) send_uart ("SD CARD UNMOUNTED successfully...\n");
}
void deleteFile(char *name)
{
		fresult = f_unlink(name);
  	if (fresult == FR_OK) 
			send_uart("file1.txt removed successfully...\n");
}


