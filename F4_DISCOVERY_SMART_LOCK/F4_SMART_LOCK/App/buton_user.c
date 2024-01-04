#include "buton_user.h"
#include "lcd_user.h"
#include "main.h"
#include "keypad_user.h"
#include "sd_user.h"
#include "ds1307.h"
Button_t bt1, bt2, bt3, detectFinger;
uint8_t fingerId, confidence; 
uint16_t templateCount;

extern uint8_t stateCheckInOut;			// from file main.c
extern bool stateLogin;							// from file main.c
extern uint64_t timeWaitLogin;			// from file main.c
extern uint64_t timeWaitCheckInOut;	// from file main.c
extern char timeDs1307[DS1307_MINIMAL_BUFFER_LENGTH];			// from file main.c
extern uint64_t timeWaitShowMainLcd;
extern uint8_t indexPassword;				// from file keypad_user.c
extern bool correctPassWord;				// from file keypad_user.c
extern bool completePassWord;			// file keypad_user
extern char buffPassword[4];				

extern char dataCheck[DS1307_MINIMAL_BUFFER_LENGTH + 22];
extern char Id[4];

extern char *correctCheckOut;
extern char *incorrectCheckOut;

extern char *correctCheckIn;
extern char *incorrectCheckIn;


int8_t fingerCheck;
bool completeFinger;
uint64_t timeWaitShowMainFinger;

bool completePassWordPrev;

void initButton(void)
{
		Button_Init(&bt1, BT1_GPIO_Port, BT1_Pin);
		Button_Init(&bt2, BT2_GPIO_Port, BT2_Pin);
		Button_Init(&bt3, BT3_GPIO_Port, BT3_Pin);
		Button_Init(&detectFinger, FINGER_DETECT_GPIO_Port, FINGER_DETECT_Pin);
}
void handleButton(void)
{
		bt_handle(&bt1);
		bt_handle(&bt2);
		bt_handle(&bt3);
		bt_handle(&detectFinger);
		if(HAL_GetTick() - timeWaitLogin > 5000)
		{
			stateLogin = false;
			indexPassword = 0;
		}
		if(HAL_GetTick() - timeWaitCheckInOut > 5000)
		{
			stateCheckInOut = 0;
			indexPassword = 0;
			//stateLogin = false;
		}
		if(completeFinger == true)
		{
				if(correctPassWord == true)
				{
					checkInOutComplete();
				}
				else if(correctPassWord == false){
					checkInOutError();
				}
				if(HAL_GetTick() - timeWaitShowMainFinger > 2000)				// 2s reset main LCD
				{
						indexPassword = 0;
						completeFinger = false;
						stateLogin = false;
						stateCheckInOut = 0;
						lcdNonLogin();
				}
		}
		if(completePassWord == true)
		{
				if(completePassWordPrev != completePassWord)
				{
						handlePassword();	
						completePassWordPrev = completePassWord;
						memset(buffPassword, '\0', sizeof(buffPassword));
				}
				
				if(correctPassWord == true)
				{
					checkInOutComplete();
				}
				else if(correctPassWord == false){
					checkInOutError();
				}
				if(HAL_GetTick() - timeWaitShowMainLcd > 2000)			// 2s reset main LCD
				{
					completePassWord = false;
					completePassWordPrev = completePassWord;
					stateLogin = false;
					stateCheckInOut = 0;
					indexPassword = 0;
					
				}
		}
}
void bt_press_callback(Button_t *button) {
		if(button == &bt1)
		{
			HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
			timeWaitLogin = HAL_GetTick();
			stateLogin = true;
		}
		else if (button == &bt2)
		{
				HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
				if(stateLogin == true)
				{
					timeWaitLogin = HAL_GetTick();
					timeWaitCheckInOut = HAL_GetTick();
					stateCheckInOut = 1;			// CHECK IN
				}
		}
		else if (button == &bt3)
		{
				HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
				if(stateLogin == true)
				{
					timeWaitLogin = HAL_GetTick();
					timeWaitCheckInOut = HAL_GetTick();
					stateCheckInOut = 2;		// CHECK IN
				}
		}
		else if (button == &detectFinger)
		{
			if(stateLogin == true)			// trang thai dang nhap
			{
					if(stateCheckInOut != 0)			// ktra co dang check in/out ?
					{
							// "abcxyz"
						
							fingerCheck = fingerHandle();
							//templateCount = get_template_number();
							memset(Id,'\0', strlen(Id));												// XOA DU LIEU TRUOC DO
							memset(dataCheck,'\0', sizeof(dataCheck));					// XOA DU LIEU TRUOC DO
							memcpy(dataCheck, timeDs1307, sizeof(timeDs1307));				// COPY THOI GIAN DOC DUOC VAO BIEN "dataCheck"
							//HAL_UART_Transmit(&huart3,(uint8_t*)dataCheck , strlen((char*)dataCheck), HAL_MAX_DELAY);
							if(fingerCheck == FINGERPRINT_OK)				// VAN TAY DUNG
							{
									completeFinger = true;
									correctPassWord = true;

									fingerId = getFingerID();
									confidence = getConfidence();
									//delete_fingerprint(fingerId);
									sprintf(Id, " %2d", fingerId);					// DUA DU LIEU "fingerId" VAO BIEN "Id"
									strcat(dataCheck, Id);									// GHEP CHUOI "Id" VAO BIEN "dataCheck"
									if(stateCheckInOut == 1) 		// CHECK IN
									{
											strcat(dataCheck, correctCheckIn);			
									}
									else if (stateCheckInOut == 2) 		// CHECK OUT
									{
											strcat(dataCheck, correctCheckOut);
									}
									HAL_UART_Transmit(&huart3,(uint8_t*)dataCheck , strlen((char*)dataCheck), HAL_MAX_DELAY);
									writeTimeToSD(dataCheck);					// GHI VAO THE NHO
							}
							else 																	// VAY TAY SAI 
							{
									completeFinger = true;
									correctPassWord = false;
								
									strcat(dataCheck, " xx");					// ID
									if(stateCheckInOut == 1) 		// CHECK IN
									{
											strcat(dataCheck, incorrectCheckIn);
									}
									else if (stateCheckInOut == 2) 		// CHECK OUT
									{
											strcat(dataCheck, incorrectCheckOut);
									}
									HAL_UART_Transmit(&huart3,(uint8_t*)dataCheck , strlen((char*)dataCheck), HAL_MAX_DELAY);
									writeTimeToSD(dataCheck);				// GHI VAO THE NHO
							}
							
							timeWaitShowMainFinger = HAL_GetTick();			// THOI GIAN CHO DE VE MAN HINH TRUOC KHI LOGIN
							timeWaitLogin = HAL_GetTick();							
							timeWaitCheckInOut = HAL_GetTick();					
							clearLCD();
					}
			}
		}
		
}
