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

		fresult = f_open(&fil, fileTimeName, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
		//f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... \n", &fil);
		f_close(&fil);
		
		fresult = f_open(&fil, fileTimeName, FA_READ);
		f_gets(buffer, f_size(&fil), &fil);
		send_uart(buffer);
		memset(buffer, '\0', sizeof(buffer));			// clear buffer
		f_close(&fil);
		
		/*********************UPDATING an existing file ***************************/

  	/* Open the file with write access */
  	fresult = f_open(&fil, fileTimeName, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
  	/* Move to offset to the end of the file */
  	fresult = f_lseek(&fil, f_size(&fil));			

  	/* write the string to the file */
  	fresult = f_puts(buffTime, &fil);
  	f_close (&fil);
  	memset(buffer, '\0', sizeof(buffer));			// clear buffer

  	/* Open to read the file */
  	fresult = f_open (&fil, fileTimeName, FA_READ);
		//fresult = f_lseek(&fil, 20);		// tro toi vi tri 10
  	/* Read string from the file */
  	fresult = f_read (&fil, buffer, f_size(&fil), &br);
  	if (fresult == FR_OK)send_uart ("\nBelow is the data from updated file2.txt : \n");
  	send_uart(buffer);
  	send_uart("\n\n");
  	/* Close file */
  	f_close(&fil);
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


