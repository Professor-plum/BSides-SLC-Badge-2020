/*
 * person.c
 *
 *  Created on: Dec 3, 2019
 *      Author: plum
 */


#include "person.h"
#include "faces.h"
#include "game.h"
#include "images.h"
#include "Display.h"
#include "room.h"

void bossUpdate(uint8_t page){
	switch (page) {
		case 0:
			setScreen(SCREEN_TALK, 3);
			break;
		case 1:
			setScreen(SCREEN_TALK, 2);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 10:
		case 11:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 9:
			setScreen(SCREEN_TALK2, 22);
			break;
		case 13:
			setScreen(SCREEN_TALK, 4);
			break;
		case 14:
			setScreen(SCREEN_TALK, 5);
			break;
		case 15:
			setScreen(SCREEN_TALK, 6);
			break;
		case 16:
			setScreen(SCREEN_TALK, 7);
			break;
		case 17:
			setScreen(SCREEN_TALK, 8);
			break;
		case 18:
			setScreen(SCREEN_TALK, 9);
			break;
		case 19:
			setScreen(SCREEN_TALK, 11);
			break;
		case 22:
			setScreen(SCREEN_TALK, 10);
			break;
		case 12:
			setFlag(FLAG_MANAGER);
			rooms[0].image = FRONT2_ROOM;
		case 20:
		default:
			setScreen(SCREEN_ROOM_DESC, 0);
	}
}

void compUpdate(uint8_t page){
	switch (page) {
		case 0:
			setScreen(SCREEN_TALK, 52);
			break;
		case 57:
			setFlag(FLAG_EMAIL);
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 58:
		case 59:
		case 60:
		case 61:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 13:
			setScreen(SCREEN_TALK, 53);
			break;
		case 14:
			setScreen(SCREEN_TALK, 54);
			break;
		case 15:
			setScreen(SCREEN_TALK, 55);
			break;
		case 16:
			setScreen(SCREEN_TALK, 56);
			break;
		case 17:
			setScreen(SCREEN_TALK, 57);
			break;
		case 18:
			setScreen(SCREEN_TALK, 58);
			break;
		case 27:
			setScreen(SCREEN_TALK, 61);
			break;
		case 20:
		default:
			setScreen(SCREEN_ROOM_DESC, 0);
	}
}

void girlUpdate(uint8_t page){
	switch (page) {
		case 0:
			setScreen(SCREEN_TALK, 82);
			break;
		case 85:
			setFlag(FLAG_NAME);
		case 82:
		case 83:
		case 84:
		case 86:
		case 88:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 13:
			setScreen(SCREEN_TALK, 83);
			break;
		case 14:
			setScreen(SCREEN_TALK, 84);
			break;
		case 15:
			setScreen(SCREEN_TALK, 85);
			break;
		case 16:
			setScreen(SCREEN_TALK, 86);
			break;
		case 17:
			setScreen(SCREEN_TALK, 87);
			break;
		case 20:
		default:
			setScreen(SCREEN_ROOM_DESC, 0);
	}
}

void itUpdate(uint8_t page){
	switch (page) {
		case 0:
			setScreen(SCREEN_TALK, 62);
			break;
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 70:
		case 72:
		case 74:
		case 76:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 68:
			setScreen(SCREEN_INPUT_IP, 0);
			break;
		case 69:
			setScreen(SCREEN_INPUT_KEY, 0);
			break;
		case 71:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 73:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 75:
			setFlag(FLAG_CAMERA);
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 13:
			setScreen(SCREEN_TALK, 63);
			break;
		case 14:
			setScreen(SCREEN_TALK, 64);
			break;
		case 15:
			setScreen(SCREEN_TALK, 65);
			break;
		case 16:
			setScreen(SCREEN_TALK, 66);
			break;
		case 17:
			setScreen(SCREEN_TALK, 67);
			break;
		case 18:
			setScreen(SCREEN_TALK, 68);
			break;
		case 19:
			setScreen(SCREEN_TALK, 69);
			break;
		case 23:
			if (checkFlag(FLAG_CONSOLE))
				setScreen(SCREEN_TALK, 75);
			else
				setScreen(SCREEN_TALK, 74);
			break;
		case 20:
		default:
			setScreen(SCREEN_ROOM_DESC, 0);
	}
}

void workerUpdate(uint8_t page){
	switch (page) {
		case 0:
			setScreen(SCREEN_TALK, 32);
			break;
		case 36:
		case 37:
			setScreen(SCREEN_TALK, ++page);
			break;
		case 38:
			setFlag(FLAG_MISSING);
		case 32:
		case 33:
		case 34:
		case 35:
		case 39:
		case 40:
		case 41:
		case 42:
		case 44:
		case 45:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 13:
			setScreen(SCREEN_TALK, 33);
			break;
		case 14:
			setScreen(SCREEN_TALK, 34);
			break;
		case 15:
			setScreen(SCREEN_TALK, 35);
			break;
		case 16:
		case 24:
			setScreen(SCREEN_TALK, 36);
			break;
		case 17:
			setScreen(SCREEN_TALK, 39);
			break;
		case 18:
			setScreen(SCREEN_TALK, 40);
			break;
		case 20:
			if (checkFlag(FLAG_MISSING))
				setScreen(SCREEN_ROOM_DESC, 0);
			else
				setScreen(SCREEN_TALK, 46);
			break;
		case 21:
			if (checkFlag(FLAG_VOLT))
				setScreen(SCREEN_TALK, 45);
			else if (checkFlag(FLAG_CAP))
				setScreen(SCREEN_TALK, 43);
			else
				setScreen(SCREEN_TALK, 42);
			break;
		case 28:
			setFlag(FLAG_NFC_RDY);
			setScreen(SCREEN_TALK, 48);
			break;
		case 43:
			setScreen(SCREEN_INPUT_VOLT, 0);
			break;
		case 46:
			setScreen(SCREEN_TALK2, 24);
			break;
		default:
			setScreen(SCREEN_ROOM_DESC, 0);
	}
}

void engUpdate(uint8_t page) {

	switch (page) {
		case 0:
			setScreen(SCREEN_TALK, 92);
			break;
		case 92:
		case 93:
		case 94:
		case 95:
		case 96:
		case 97:
		case 98:
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 25:
			setScreen(SCREEN_TALK, 99);
			break;
		case 99:
			setScreen(SCREEN_TALK2, 26);
			break;
		case 26:
			setScreen(SCREEN_TALK, 100);
			break;
		case 100:
			setFlag(FLAG_SERVICE);
			setScreen(SCREEN_TALK_CHOICE, 0);
			break;
		case 13:
			setScreen(SCREEN_TALK, 93);
			break;
		case 14:
			setScreen(SCREEN_TALK, 94);
			break;
		case 15:
			setScreen(SCREEN_TALK, 95);
			break;
		case 16:
			setScreen(SCREEN_TALK, 96);
			break;
		case 17:
			setScreen(SCREEN_TALK, 97);
			break;
		case 18:
			setScreen(SCREEN_TALK, 98);
			break;
		case 20:
		default:
			setScreen(SCREEN_ROOM_DESC, 0);
	}
}

void enemyUpdate(uint8_t page){

	switch (page) {

			case 102:
				setScreen(SCREEN_MESSAGE, 24);
				break;
			case 105:
				setScreen(SCREEN_TALK, 106);
				break;
			case 106:
				setScreen(SCREEN_TALK2, 29);
				break;
			case 29:
				setScreen(SCREEN_TALK, 107);
				break;
			case 107:
				break;//do nothing here
			default:
				setScreen(SCREEN_ROOM_DESC, 0);

	}
}

void worker2Update(uint8_t page) {
	switch (page) {
		case 0:
			setScreen(SCREEN_TALK, 103);
			break;
		case 103:
			setScreen(SCREEN_TALK, 104);
			break;
		default:
			setScreen(SCREEN_ROOM_DESC, 0);
	}
}

const Person people[PEOPLE_COUNT] = {
		{},
		{bossUpdate, BOSS_IMG},
		{compUpdate, COMP_IMG},
		{girlUpdate, GIRL_IMG},
		{itUpdate, ITGUY_IMG},
		{workerUpdate, WORKER_IMG},
		{enemyUpdate, ENEMY_IMG},
		{engUpdate, ENG_IMG},
		{worker2Update, WORKER_IMG},
};

void DrawChoice(uint8_t person, uint8_t page) {
	char* choices[10] = {0};
	char* message = "Select topic";
	uint8_t i=0;
	choices[i++] = "their position";
	choices[i++] = "damage";
	choices[i++] = "suspects";
	if (checkFlag(FLAG_MISSING))
		choices[i++] = "missing items";
	if (checkFlag(FLAG_NAME))
		choices[i++]="PH4N70M";
	switch (person) {
	case PERSON_BOSS:
		if (checkFlag(FLAG_EMAIL))
			choices[i++]="phishing email";
		if (checkFlag(FLAG_PCAP))
			choices[i++]="ransomware";
		break;
	case PERSON_COMP:
		choices[i++]="error screen";
		if (checkFlag(FLAG_PCAP))
			choices[i++]="ransomware";
		break;
	case PERSON_GIRL:
		break;
	case PERSON_IT:
		if (checkFlag(FLAG_EMAIL) && (!checkFlag(FLAG_PCAP)))
				choices[i++]="phishing email";
		if (checkFlag(FLAG_PCAP) && (!checkFlag(FLAG_RANSOM)))
			choices[i++]="ransomware";
		if (checkFlag(FLAG_RANSOM))
			choices[i++]="cameras";
		break;
	case PERSON_WORKER:
		choices[i++] = "offer help";
		if (checkFlag(FLAG_PCAP))
			choices[i++]="ransomware";
		if (checkFlag(FLAG_CAMERA))
			choices[i++]="cameras";
		break;
	case PERSON_ENGINEER:
		if (checkFlag(FLAG_BOX_OPEN))
			choices[i++]="blackmail";
		break;
	case PERSON_ENEMY:
		break;
	}
	choices[i++] = "goodbye";
	DisplayChoice(message, choices, true);
}

void HandleChoice(uint8_t person, uint8_t page, const char* choice) {

	if (strcmp(choice, "goodbye") == 0) {
		setScreen(SCREEN_TALK2, 20);
	}
	else if (strcmp(choice, "their position") == 0) {
		setScreen(SCREEN_TALK2, 13);
	}
	else if (strcmp(choice, "damage") == 0) {
			setScreen(SCREEN_TALK2, 14);
		}
	else if (strcmp(choice, "suspects") == 0) {
		setScreen(SCREEN_TALK2, 15);
	}
	else if (strcmp(choice, "missing items") == 0) {
		setScreen(SCREEN_TALK2, 16);
	}
	else if (strcmp(choice, "PH4N70M") == 0) {
		setScreen(SCREEN_TALK2, 17);
	}
	else if (strcmp(choice, "phishing email") == 0) {
		setScreen(SCREEN_TALK2, 18);
	}
	else if (strcmp(choice, "ransomware") == 0) {
		setScreen(SCREEN_TALK2, 19);
	}
	else if (strcmp(choice, "offer help") == 0) {
		setScreen(SCREEN_TALK2, 21);
	}
	else if (strcmp(choice, "blackmail") == 0) {
		setScreen(SCREEN_TALK2, 25);
	}
	else if (strcmp(choice, "error screen") == 0) {
		setScreen(SCREEN_TALK2, 27);
	}
	else if (strcmp(choice, "cameras") == 0) {
		if (person == 5)
			setScreen(SCREEN_TALK2, 28);
		else
			setScreen(SCREEN_TALK2, 23);
	}
}
