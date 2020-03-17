/*
 * items.h
 *
 *  Created on: Nov 15, 2019
 *      Author: plum
 */

#ifndef INC_ITEMS_H_
#define INC_ITEMS_H_

#include <stdint.h>
#include "flags.h"

typedef struct {
	char* name;
	char* description;
	char* takemsg;
	char* openmsg;
	uint8_t takeflag, openflag;
} Item;

extern const Item items[];
#endif /* INC_ITEMS_H_ */
