/*
 * person.h
 *
 *  Created on: Dec 3, 2019
 *      Author: plum
 */

#ifndef INC_PERSON_H_
#define INC_PERSON_H_

#include "flags.h"

typedef void (*clickFunc)(uint8_t page);
typedef uint8_t (*choiceFunc)(uint8_t page, const char* choice);

#define PERSON_NONE		0
#define PERSON_BOSS		1
#define PERSON_COMP		2
#define PERSON_GIRL		3
#define PERSON_IT		4
#define PERSON_WORKER	5
#define PERSON_ENEMY	6
#define PERSON_ENGINEER	7
#define PERSON_WORKER2	8

#define PEOPLE_COUNT	9

typedef struct {
	clickFunc click;
	uint32_t image_addr;
} Person;

extern const Person people[PEOPLE_COUNT];
extern const char* dialogs[];


void DrawChoice(uint8_t person, uint8_t page);
void HandleChoice(uint8_t person, uint8_t page, const char* choice);

#endif /* INC_PERSON_H_ */
