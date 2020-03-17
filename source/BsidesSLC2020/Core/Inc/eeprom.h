/*
 * eeprom.h
 *
 *  Created on: Oct 31, 2019
 *      Author: plum
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "main.h"

#define DATA_EEPROM_START_ADDR     	0x08080000
#define GAME_FLAGS					(DATA_EEPROM_START_ADDR+4)
#define GAME_CHECK					(DATA_EEPROM_START_ADDR+40)
#define CHK_VALUE					0x34
#define GAME_AVATAR					(DATA_EEPROM_START_ADDR+44)
#define GAME_ECE_THING				(DATA_EEPROM_START_ADDR+48)
#define GAME_STATE_REG				RTC_BKP_DR1
#define ADC_CAL_REG					RTC_BKP_DR2
#define LAST_TIME_DISPLAY			RTC_BKP_DR3
#define CLOCK_SET					RTC_BKP_DR4


void saveState(uint32_t* state);
void readState(uint32_t* state);
uint8_t getLastTime();
void setLastTime(uint8_t t);

#endif /* INC_EEPROM_H_ */
