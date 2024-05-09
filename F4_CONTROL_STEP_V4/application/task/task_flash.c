#include "ak.h"

#include "app.h"
#include "task_list.h"

#include "flash.h"

#include "task_flash.h"
#include "scr_device.h"

#define ROWS    (4)
#define COLS	(20)

#define AR_STORE_ANGLE      (0x08040000)
#define AR_STORE_SETTING    (0x08060000)
void convertToArray1D(uint16_t arr2D[ROWS][COLS], uint16_t arr1D[], uint8_t rows, uint8_t cols) {
    uint8_t i, j, k = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            arr1D[k++] = arr2D[i][j];
        }
    }
}

void convertToArray2D(uint16_t arr1D[], uint16_t arr2D[ROWS][COLS], uint8_t rows, uint8_t cols) {
    uint8_t k = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr2D[i][j] = arr1D[k++];
        }
    }
}

void angle_config_clear() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            angle_config[i][j] = 0;
        }
    }
}


void task_flash_handle(ak_msg_t *msg) {
    uint16_t arr_1D[ROWS * COLS];
    switch (msg->sig) {
        case FLASH_SETUP_SIG: {
            angle_config_clear();
            break;
        }

        case FLASH_WRITE_CFG_ANGLE_SIG: {
            convertToArray1D(angle_config, arr_1D, ROWS, COLS);
            Flash_Write_Array(AR_STORE_ANGLE, arr_1D, (ROWS * COLS));
            memset(arr_1D, 0, sizeof(arr_1D));
            break;
        }

        case FLASH_READ_CFG_ANGLE_SIG: {
			Flash_Read_Array(AR_STORE_ANGLE, arr_1D, (ROWS * COLS));
            convertToArray2D(arr_1D, angle_config, ROWS, COLS);
            break;
        }

        case FLASH_WRITE_SETTING_SIG: {
            Flash_Write_Array(AR_STORE_SETTING, (uint16_t*)&user_step_info.user_setting, sizeof(user_step_info.user_setting));
            break;
        }

        case FLASH_READ_SETTING_SIG: {
            Flash_Read_Array(AR_STORE_SETTING, (uint16_t*)&user_step_info.user_setting, sizeof(user_step_info.user_setting));
            break;
        }

        default:
            break;
    }
}