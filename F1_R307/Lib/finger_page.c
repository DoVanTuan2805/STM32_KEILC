/*
 * finger_page.c
 *
 *  Created on: Nov 19, 2021
 *      Author: Steven
 */
#include "finger_page.h"
#include "stdio.h"

uint8_t current_page = 0;

PAGE finger = {
		.title = "Fingerprint",
		.update_page = finger_update,
		.on_click = finger_onclick,
		.init = NULL
};

finger_page page_mode = DEFAULT;


uint8_t macro_mode = 0;
//the function of the fingerprint is just to set macro and
//fingerprint.


//void finger_onclick(char *combination, int charNum){
//
//	if(page_mode == DEFAULT && current_page == 0){
//		if(combination[0] == 13){
//			//start scanning the shit
//			fingerprint_mode = save_fingerprint(0);
//		}
//
//		else if(combination[0] == 129){
//			//start scanning the shit
//			reset_database();
//			fingerprint_mode = -1;
//
//		}
//		else if(combination[0] == 130){
//			check_fingerprint();
//
//		}
//	}
//	finger_update();
//}
