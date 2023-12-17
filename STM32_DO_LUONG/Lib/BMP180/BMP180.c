#include "stm32f1xx_hal.h"
#include "BMP180.h"
#include "math.h"
#include "main.h"
extern I2C_HandleTypeDef hi2c1;
#define BMP180_I2C &hi2c1

#define BMP180_ADDRESS 0xEE

//const float POW2_11  = pow(2,  11);
//const float POW2_M2  = pow(2,  -2);
//const float POW2_M4  = pow(2,  -4);
//const float POW2_M8  = pow(2,  -8);
//const float POW2_M11 = pow(2, -11);
//const float POW2_M12 = pow(2, -12);
//const float POW2_M13 = pow(2, -13);
//const float POW2_M15 = pow(2, -15);
//const float POW2_M16 = pow(2, -16);

#define POW2_11 2048.00000
#define POW2_M2 0.250000
#define POW2_M4 0.062500
#define POW2_M8 0.003906
#define POW2_M11 0.000488
#define POW2_M12 0.000244
#define POW2_M13 0.000122
#define POW2_M15 0.000031
#define POW2_M16 0.000015


short AC1 = 0;
short AC2 = 0;
short AC3 = 0;
unsigned short AC4 = 0;
unsigned short AC5 = 0;
unsigned short AC6 = 0;
short B1 = 0;
short B2 = 0;
short MB = 0;
short MC = 0;
short MD = 0;


long UT = 0;
short oss = 0;
long UP = 0;
long X1 = 0;
long X2 = 0;
long X3 = 0;
long B3 = 0;
long B5 = 0;
unsigned long B4 = 0;
long B6 = 0;
unsigned long B7 = 0;

long Press = 0;
long Temp = 0;

#define atmPress 101325

void read_calliberation_data (void)
{
	uint8_t Callib_Data[22] = {0};
	uint16_t Callib_Start = 0xAA;
	if (HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, Callib_Start, 1, Callib_Data,22, HAL_MAX_DELAY) != HAL_OK)
		//printf("I2C reading data error\n");

	AC1 = ((Callib_Data[ 0] << 8) | Callib_Data[ 1]);
	AC2 = ((Callib_Data[ 2] << 8) | Callib_Data[ 3]);
	AC3 = ((Callib_Data[ 4] << 8) | Callib_Data[ 5]);
	AC4 = ((Callib_Data[ 6] << 8) | Callib_Data[ 7]);
	AC5 = ((Callib_Data[ 8] << 8) | Callib_Data[ 9]);
	AC6 = ((Callib_Data[10] << 8) | Callib_Data[11]);
	B1  = ((Callib_Data[12] << 8) | Callib_Data[13]);
	B2  = ((Callib_Data[14] << 8) | Callib_Data[15]);
	MB  = ((Callib_Data[16] << 8) | Callib_Data[17]);
	MC  = ((Callib_Data[18] << 8) | Callib_Data[19]);
	MD  = ((Callib_Data[20] << 8) | Callib_Data[21]);

}


// Get uncompensated Temp
uint16_t Get_UTemp (void)
{
	uint8_t datatowrite = 0x2E;
	uint8_t Temp_RAW[2] = {0};
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &datatowrite, 1, 1000);
	HAL_Delay (5);  // wait 4.5 ms
	if (HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Temp_RAW, 2, 1000) != HAL_OK)
		//printf("I2C reading data error\n");
	return ((Temp_RAW[0]<<8) + Temp_RAW[1]);
	
}

float BMP180_GetTemp (void)
{
	UT = Get_UTemp();
	X1 = ((UT - AC6) * (AC5 * (POW2_M15)));
	X2 = ((MC * (POW2_11)) / (X1 + MD));
	B5 = X1 + X2;
	Temp = (B5 + 8) * (POW2_M4);
	return Temp / 10.0;
}

// Get uncompensated Pressure
uint32_t Get_UPress (int oss)   // oversampling setting 0,1,2,3
{
	uint8_t datatowrite = 0x34 + (oss<<6);
	uint8_t Press_RAW[3] = {0};
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &datatowrite, 1, 1000);
	switch (oss)
	{
		case (0):
			HAL_Delay (5);
			break;
		case (1):
			HAL_Delay (8);
			break;
		case (2):
			HAL_Delay (14);
			break;
		case (3):
			HAL_Delay (26);
			break;
	}
	if(HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Press_RAW, 3, 1000) != HAL_OK)
		//printf("I2C reading data error\n");
	return (((Press_RAW[0] << 16) + (Press_RAW[1] << 8)+Press_RAW[2]) >> (8 - oss));
	
}

float BMP180_GetPress (int oss)
{
	UP = Get_UPress(oss);
	X1 = ((UT - AC6) * (AC5 * (POW2_M15)));
	X2 = ((MC * (POW2_11)) / (X1+MD));
	B5 = X1 + X2;
	B6 = B5 - 4000;
	X1 = (B2 * (B6 * B6 * (POW2_M12))) * (POW2_M11);
	X2 = AC2 * B6 * (POW2_M11);
	X3 = X1 + X2;
	B3 = (((AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = AC3 * B6 * POW2_M13;
	X2 = (B1 * (B6 * B6 * (POW2_M12))) * (POW2_M16);
	X3 = ((X1 + X2) + 2) * POW2_M2;
	B4 = AC4 * (unsigned long)(X3 + 32768) * (POW2_M15);
	B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	if (B7 < 0x80000000)
		Press = (B7 * 2) / B4;
	else
		Press = (B7 / B4) * 2;
	X1 = (Press * (POW2_M8)) * (Press * (POW2_M8));
	X1 = (X1 * 3038) * (POW2_M16);
	X2 = (-7357 * Press) * (POW2_M16);
	Press = Press + (X1 + X2 + 3791) * (POW2_M4);

	return Press;
}


float BMP180_GetAlt (int oss)
{
	BMP180_GetPress (oss);
	return 44330 * (1 - (pow((Press / (float)atmPress), 0.19029495718)));
}

void BMP180_Start (void)
{
	read_calliberation_data();
}
