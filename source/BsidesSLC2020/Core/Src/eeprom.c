/*
 * eeprom.c
 *
 *  Created on: Oct 31, 2019
 *      Author: plum
 */


#include "eeprom.h"
#include <stm32l0xx_hal_flash_ex.h>

void saveState(uint32_t* state) {
	HAL_RTCEx_BKUPWrite(&hrtc, GAME_STATE_REG, *state);
}

void readState(uint32_t* state) {
	*state = HAL_RTCEx_BKUPRead(&hrtc, GAME_STATE_REG);
}

uint8_t getLastTime() {
	return HAL_RTCEx_BKUPRead(&hrtc, LAST_TIME_DISPLAY);
}

void setLastTime(uint8_t t) {
	HAL_RTCEx_BKUPWrite(&hrtc, LAST_TIME_DISPLAY, (uint32_t)t);
}
