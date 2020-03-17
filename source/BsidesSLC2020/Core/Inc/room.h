/*
 * room.h
 *
 *  Created on: Nov 1, 2019
 *      Author: plum
 */

#ifndef INC_ROOM_H_
#define INC_ROOM_H_

#include "main.h"

typedef void (*itemsFunc)();

#define ROOM_COUNT	14

typedef struct {
	char* name;
	char* description;
	itemsFunc loadItems;
	uint32_t image;
	uint8_t personId;
} Room;

extern Room rooms[ROOM_COUNT];

#endif /* INC_ROOM_H_ */
