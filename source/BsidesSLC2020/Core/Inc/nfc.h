/*
 * nfc.h
 *
 *  Created on: Oct 19, 2019
 *      Author: plum
 */

#ifndef INC_NFC_H_
#define INC_NFC_H_

#include "main.h"
#include "st25dv.h"

extern ST25DV_Object_t st25dv;

bool NFC_Init();

bool NFC_WriteText(char* message);

int NFC_CheckEvent();

void NFC_PowerDown();

uint32_t NFC_GetID();


#endif /* INC_NFC_H_ */
