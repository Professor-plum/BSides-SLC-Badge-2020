/*
 * mems.h
 *
 *  Created on: Sep 20, 2019
 *      Author: plum
 */

#ifndef MEMS_H_
#define MEMS_H_

#include "main.h"
#include "lis2dh12.h"
#include "custom_bus.h"

bool MEMS_Init();

uint8_t MEMS_Process();

void MEMS_PowerDown();

void MEMS_Click();

void MEMS_Clear();

#endif /* MEMS_H_ */
