#include "sd_user.h"
#include "string.h"
#include "usart.h"

extern UART_HandleTypeDef huart3;

static char *fileTimeName = "dataTime.txt";

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
void initSDCard()
{

		fresult = f_mount(&fs, "/", 1);					// KET NOI VOI THE NHO
		fresult == FR_OK  
				? send_uart ("SD CARD MOUNTED successfully...\n") 
				: send_uart ("SD CARD MOUNTED error...\n");
		
		fresult = f_open(&fil, fileTimeName, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);				// CREAT FILE				// NEU CHUA CO FILE "fileTimeName" THI SE TAO FILE NAY
		//fresult = f_open(&fil, fileTimeName, FA_OPEN_EXISTING | FA_READ | FA_WRITE);				// CREAT FILE
		fresult == FR_OK ? send_uart ("SD CARD OPEN successfully...\n") : send_uart ("SD CARD OPEN error...\n");
		f_close(&fil);
	
		fresult = f_mount(NULL, "/", 1);		// NGAT KET NOI VOI THE NHO
  	fresult == FR_OK  
				? send_uart ("SD CARD UNMOUNTED successfully...\n") 
				: send_uart ("SD CARD UNMOUNTED error...\n");
}
void writeTimeToSD(char *buffTime)
{
		fresult = f_mount(&fs, "/", 1);
		fresult == FR_OK  
				? send_uart ("SD CARD MOUNTED successfully...\n") 
				: send_uart ("SD CARD MOUNTED error...\n");
	
		
		/*	UPDATE FILE */
  	fresult = f_open(&fil, fileTimeName,  FA_OPEN_EXISTING | FA_READ | FA_WRITE);				// MO FILE
  	fresult = f_lseek(&fil, f_size(&fil));						// MOVE TO END FILE
  	fresult = f_puts(buffTime, &fil);									// PUT DATA
  	f_close (&fil);
  	
	
		/* READ FILE */
		fresult = f_open(&fil, fileTimeName, FA_READ);					
		fresult = f_read(&fil, buffer, f_size(&fil), &br);
		send_uart(buffer);
		memset(buffer, '\0', sizeof(buffer));			// clear buffer
		f_close(&fil);
		
		
  	/* Unmount SDCARD */
  	fresult = f_mount(NULL, "/", 1);
  	fresult == FR_OK  
				? send_uart ("SD CARD UNMOUNTED successfully...\n") 
				: send_uart ("SD CARD UNMOUNTED error...\n");
}
void deleteFile(char *name)
{
		fresult = f_mount(&fs, "/", 1);
		fresult == FR_OK  
				? send_uart ("SD CARD MOUNTED successfully...\n") 
				: send_uart ("SD CARD MOUNTED error...\n");
	
		fresult = f_unlink(name);
  	fresult == FR_OK  
				? send_uart ("SD CARD REMOVE successfully...\n") 
				: send_uart ("SD CARD REMOVE error...\n");
	
		fresult = f_mount(NULL, "/", 1);
		fresult == FR_OK  
				? send_uart ("SD CARD UNMOUNTED successfully...\n") 
				: send_uart ("SD CARD UNMOUNTED error...\n");
}


