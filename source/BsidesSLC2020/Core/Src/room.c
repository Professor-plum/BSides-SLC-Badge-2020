/*
 * room.c
 *
 *  Created on: Nov 1, 2019
 *      Author: plum
 */


#include "room.h"
#include "items.h"
#include "display.h"
#include "flags.h"
#include "person.h"
#include "images.h"

char* name;
char* description;
itemsFunc loadItems;
uint32_t icoAddr;
uint8_t personId;

static void addItem(int id) {
	AddMenuItem(id, items[id].name);
}

void officeItems() {
	ClearMenuItems();
	addItem(3);
	addItem(1);
	addItem(2);
	if (!checkFlag(FLAG_MANAGER))
		addItem(4);
	if (checkFlag(FLAG_DESK)) {
		if (checkFlag(FLAG_KEYCARD)==0)
			addItem(0);
		if (checkFlag(FLAG_TAPE)==0)
			addItem(5);
	}
}

void controlItems() {
	ClearMenuItems();
	addItem(6);
	addItem(7);
	addItem(8);
	addItem(9);
	if (checkFlag(FLAG_HANGER)==0)
		addItem(10);
}

void repairItems() {
	ClearMenuItems();
	addItem(11);
	addItem(12);
	addItem(14);
	if (checkFlag(FLAG_GLASS)) {
		if (checkFlag(FLAG_EXTINGUISH)==0)
			addItem(15);
	}
}

void itItems() {
	ClearMenuItems();
	addItem(22);
	addItem(23);
	addItem(24);
}

void supplyItems() {
	ClearMenuItems();
	addItem(25);
	if (checkFlag(FLAG_BATTERIES)==0)
		addItem(26);
	addItem(46);
	addItem(27);
}

void breakItems() {
	ClearMenuItems();
	addItem(16);
	addItem(17);
	addItem(18);
	addItem(19);
	addItem(20);
	if (checkFlag(FLAG_HIT_BOOKS)==1)
		if (checkFlag(FLAG_SCREW) == 0)
			addItem(21);
}

void serviceItems() {
	ClearMenuItems();
	addItem(28);
	addItem(29);
	addItem(30);
	addItem(43);
}

void secretItems() {
	ClearMenuItems();
	addItem(42);
	addItem(32);
	addItem(33);
}

void meetingItems() {
	ClearMenuItems();
	addItem(31);
	addItem(37);
	addItem(39);
	addItem(38);
	if (checkFlag(FLAG_PUZZLE) == 0)
		addItem(40);
}

void backOfficeItems() {
	ClearMenuItems();
	addItem(44);
	addItem(45);
	addItem(34);
	addItem(35);
}

void cageItems() {
	ClearMenuItems();
	addItem(48);
	addItem(49);
}

void fireItems() {
	ClearMenuItems();
	addItem(50);
	addItem(14);
	if (checkFlag(FLAG_GLASS)) {
		if (checkFlag(FLAG_EXTINGUISH)==0)
			addItem(15);
	}
}

void lastItems() {
	ClearMenuItems();
	addItem(51);
	addItem(52);
}

Room rooms[ROOM_COUNT] = {
		{
				"Head office",
				"You're standing in the head office of the Tri-city Power Corp east side branch. The room is pretty bland and the walls look bare. The desk in the center is the only focal point.",
				officeItems,
				FRONT_ROOM,
				PERSON_BOSS
		},
		{
				"Control room",
				"The room is dimly lit. The primary source of light comes from the monitors on the wall. The only occupant, an over weight 20 something spins around in his chair and watches you closely.",
				controlItems,
				COMP_ROOM,
				PERSON_COMP
		},
		{
				"Repair yard",
				"A large open hanger area. There are a number of large electrical boxes, circuit breakers you presume. You unfortunately also spot the partially exposed rear of someone stooped over the equipment.",
				repairItems,
				REPAIR_ROOM,
				PERSON_WORKER
		},
		{
				"IT room",
				"The server room is rather tiny and very noisy. There are servers in a central rack with a mess of cables running from them over to switches in a rats nest. A person sits in the corner on a laptop.",
				itItems,
				IT_ROOM,
				PERSON_IT
		},
		{
				"Supply room",
				"You are in a tight little storage room next to the repair yard. There are a number of cabinets full of unfamiliar tools, parts and other odds 'n ends. Most of the tools you don't recognize.",
				supplyItems,
				SUPPLY_ROOM,
				PERSON_NONE
		},
		{
				"Break room",
				"A typical office break room. Complete with community fridge, microwave, and table. The young women sitting there glances up from her magazine to shoot you a polite gesture then returns to her reading.",
				breakItems,
				BREAK_ROOM,
				PERSON_GIRL
		},
		{
				"Meeting room",
				"The only meeting room in the building it seats about 20 around the central table. There is only one individual in here who seems to be fixated studying a large rolled out blueprints.",
				meetingItems,
				MEET_ROOM,
				PERSON_ENGINEER
		},
		{
				"Back Office",
				"It is a quaint little office. The desk is fairly clean. It holds only a terminal, a digital picture frame and a tiny little lock box.",
				backOfficeItems,
				BACK_ROOM,
				PERSON_NONE
		},
		{
				"Service tunnels",
				"You stand at the entrance to some service tunnel which apparently runs under the whole city. It's a dark dingy place, maybe a slight step up from the sewers.",
				serviceItems,
				TUNNEL_ROOM,
				PERSON_NONE
		},
		{
				"Secret Lab",
				"You found some kind of secret lab. There is a workbench dead center in the service tunnel. There are a few power cords that run around the corner providing power and light to the area.",
				secretItems,
				SECRET_ROOM,
				PERSON_ENEMY
		},
		{
				"Cage",
				"You awaken to find yourself trapped behind steal bars of another service tunnel. It appears however the bars were designed to keep people out rather then in.",
				cageItems,
				CAGE_ROOM,
				PERSON_NONE
		},
		{
				"Repair yard",
				"As you approach the yard you here a large commotion. Most, if not all, the equipment in the yard is on burning!  \"Fire! Fire!\" you hear Bert yells from somewhere behind the flames.",
				fireItems,
				FIRE_ROOM,
				PERSON_NONE
		},
		{
				"Repair yard",
				"It took every bit of CO2 gas in your fire extinguisher but you were able to subdue the flames. Too bad the equipment is a completely ruined.",
				lastItems,
				YARD2_ROOM,
				PERSON_WORKER2
		},
		{
				"",
				"",
				supplyItems,
				SUPPLY2_IMG,
				PERSON_ENEMY
		}
};
