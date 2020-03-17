/*
 * game.c
 *
 *  Created on: Oct 31, 2019
 *      Author: plum
 */


#include "game.h"
#include "epd.h"
#include "eeprom.h"
#include "display.h"
#include "room.h"
#include "bsides_logo.h"
#include "items.h"
#include "flags.h"
#include "person.h"
#include "images.h"
#include "nfc.h"
#include <stdio.h>

#define CLOCK_EN 1

GameState g_state;
uint8_t g_flags[6];
GameLandmarks g_landmarks;
uint8_t logo, lastScreen=0;
uint16_t dval=256;
uint8_t lt, g_ece=0;
char flagstr[24];

const char* weekDay[] = {" Monday", " Tuesday", "Wednesday", "Thursday", " Friday", " Saturday", " Sunday"};
const char* months[] = {"Jan.", "Feb.", "March", "April", "May", "June", "July", "Aug.", "Sept.", "Oct.", "Nov.", "Dec."};

const MenuItem room_menu[] = {
		{"Examine", SCREEN_LOOK_MENU},
		{"Speak", SCREEN_TALK},
		{"Open", SCREEN_OPEN_MENU},
		{"Take", SCREEN_TAKE_MENU},
		{"Use", SCREEN_USE_MENU},
		{"Hit", SCREEN_HIT_MENU},
		{"Move", SCREEN_MOVE_MENU},
		{NULL, MENU_END}};

const char* messages[] = {
		NULL,
		"You can't open that",
		"You can't take that",
		"There is nobody here to talk to",
		"Those items don't go together",
		"Other than hurting your hand nothing happens",
		"As you hit the bookshelf a screwdriver falls off the top of the shelf. Someone must have left it up there.",
		"You hit the safety glass breaking it away.",
		"It is probably not a good idea to cause a scene right now.",
		"You have no items to use.",
		"I don't think the manager will let you snoop around his desk while he is sitting right there.",
		"You can't open this panel without some kind of screwdriver.",
		"The glass is already broken.",
		"You insert the batteries into the radio, you can now here static from the radio but it's not able to pick up any signals.",
		"You use the coat hanger as an antenna for the radio. However the radio is still dead, no power.",
		"You insert the batteries into the radio, suddenly it starts playing sounds.",
		"You use the coat hanger as an antenna for the radio. Suddenly you start picking up a signal.",
		"It doesn't sound like music but rather some kind of signal. You record the sound. (RTTY.OGG added to CASE FILE)",
		"You use the tape to pull the fingerprints off the equipment. Now you have some hard evidence on the saboteur!",
		"The lockbox opens before you. Inside you find an odd letter. (BLCKMAIL.RTF added to CASE FILE)",
		"You received a flashlight",
		"System restore complete!",
		"You get the strange feeling someone is watching you.",
		"Suddenly a figure emerges from the shadows.",
		"Without warning he reaches out to grab you. Instinctively you step back dodging his attack but slip on the wet floor and hit your head against the concrete knocking you out.",
		"You awaken to f1nd y@ursE]f #r%pp?? !... System is corrupt. Follow the directions in the deleted pdf on CASE FILE to restore your watch.",
		"By jumping the wires you short the power to the door releasing the cage latch.",
		"You grab the torus shaped electronic component.",
		"You grab the cube shaped electronic component.",
		"You grab the cylinder shaped electronic component.",
		"It took every bit of CO2 gas in your fire extinguisher but you were able to subdue the fire. Too bad the equipment is a completely ruined.",
		"You throw a quick punch to his jaw, the hit catches him completely off guard and he falls to the floor. Realizing he has no other options he submits."
};

bool checkFlag( flags flag) {
	uint8_t idx=flag/8;
	return (g_flags[idx] >> (flag%8)) & 1;
}

void saveFlags() {
	HAL_StatusTypeDef status;
	status = HAL_FLASHEx_DATAEEPROM_Unlock();
	for(int i=0; i< sizeof(g_flags); ++i) {
		status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, GAME_FLAGS+i, g_flags[i]);
	}
	status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, GAME_AVATAR, getAvatar());
	status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, GAME_CHECK, CHK_VALUE);
	status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, GAME_ECE_THING, g_ece);
	status = HAL_FLASHEx_DATAEEPROM_Lock();
}

char* GameFlags() {
	const char hexmap[] = "0123456789ABCDEF";
	for(int i=0; i< sizeof(g_flags); ++i) {
		flagstr[i*2] = hexmap[(g_flags[i]>>4) & 0xF];
		flagstr[i*2+1] = hexmap[g_flags[i] & 0xF];
	}
	flagstr[2*sizeof(g_flags)] = '0';
	return flagstr;
}

void setFlag( flags flag) {
	uint8_t idx=flag/8;
	g_flags[idx] |= (1<<(flag%8));
	updateLandmarks();
	saveFlags();
	GameSetNFC();
}

void SetDac(bool up) {
	if ((up) && (dval>1000)) return;
	if ((!up) && (dval==0)) return;
	if (up) dval+=16;
	else dval-=16;
	printf("DAC %d\n", dval);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dval);
}

void setScreen(uint8_t screen, uint8_t page) {
	lastScreen = g_state.screen;
	g_state.screen = screen;
	g_state.page = page;
	if (screen == SCREEN_INPUT_VOLT) {
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		SetDac(false);
		stay_awake=true;
	}
	saveState((uint32_t *)&g_state);
}

void GameInit() {
	if (*(uint8_t *)GAME_CHECK == CHK_VALUE) {
		for(int i=0; i< sizeof(g_flags); ++i) {
			g_flags[i] = (*(uint8_t *)(GAME_FLAGS+i));
		}
		g_ece = *(uint8_t *)GAME_ECE_THING;
	}
	else {
		memset(g_flags, 0, sizeof(g_flags));
		g_state.room = 0;
		setScreen(SCREEN_AVATAR, 0);
	}
	switch((*(uint32_t *)(GAME_AVATAR))) {
	case D_GUY_IMG:
		setAvatar(D_GUY_IMG);
		break;
	case D_GIRL_IMG:
		setAvatar(D_GIRL_IMG);
		break;
	default:
		setAvatar(D_GUY_IMG);
	}
	readState((uint32_t *)&g_state);
	updateLandmarks();
	GameCheckNFC();
	GameSetNFC();
	EPD_ClearScreen();
	logo = 1;
	lt = getLastTime();

	if (checkFlag(FLAG_MANAGER))
		rooms[0].image = FRONT2_ROOM;
	if (g_state.screen == SCREEN_INPUT_VOLT) {
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		SetDac(false);
		stay_awake=true;
	}
}

void LoadRooms() {
	ClearMenuItems();
	if (g_state.room == 12) {
		AddMenuItem(4, rooms[4].name);
		return;
	}
	for (uint8_t i=0; i<7; ++i) {
		AddMenuItem(i, rooms[i].name);
	}
	if (checkFlag(FLAG_KEYCARD))
		AddMenuItem(7, rooms[7].name);
	if (checkFlag(FLAG_SERVICE))
		AddMenuItem(8, rooms[8].name);
	if (checkFlag(FLAG_MAZE)) {
		AddMenuItem(9, rooms[9].name);
	}
}

bool LoadPlayerItems() {
	ClearMenuItems();
	bool i=false;
	if (checkFlag(FLAG_KEYCARD)) {
		AddMenuItem(0, items[0].name);
		i=true;
	}
	if (checkFlag(FLAG_TAPE)) {
		AddMenuItem(5, items[5].name);
		i=true;
	}
	if (checkFlag(FLAG_CAKE)) {
		AddMenuItem(19, items[19].name);
		i = true;
	}
	if (checkFlag(FLAG_SCREW)) {
		AddMenuItem(21, items[21].name);
		i = true;
	}
	if (checkFlag(FLAG_FLASHLIGHT)) {
		AddMenuItem(41, items[41].name);
		i = true;
	}
	if (checkFlag(FLAG_HANGER) && (!checkFlag(FLAG_ANTENNA))) {
		AddMenuItem(10, items[10].name);
		i=true;
	}
	if (checkFlag(FLAG_BATTERIES) && (!checkFlag(FLAG_BATT_IN))) {
		AddMenuItem(26, items[26].name);
		i = true;
	}
	if (checkFlag(FLAG_EXTINGUISH)) {
		AddMenuItem(15, items[15].name);
		i = true;
	}
	if (checkFlag(FLAG_PRINTS)) {
		AddMenuItem(36, items[36].name);
		i = true;
	}
	if (checkFlag(FLAG_ECE_PART)) {
		AddMenuItem(47, items[47].name);
		i = true;
	}
	return i;
}

bool checkRefresh() {
	if (logo)
		return true;
	if (lastScreen == g_state.screen)
		return false;

	switch (g_state.screen) {
	case SCREEN_ROOM_MENU:
	case SCREEN_MOVE_MENU:
	case SCREEN_LOOK_MENU:
	case SCREEN_TAKE_MENU:
	case SCREEN_OPEN_MENU:
	case SCREEN_HIT_MENU:
	case SCREEN_USE2_MENU:
	case SCREEN_USE_MENU:
	case SCREEN_LOOK:
	case SCREEN_OPEN:
	case SCREEN_TAKE:
	case SCREEN_MESSAGE:
		return (lastScreen == SCREEN_POW) || (lastScreen == SCREEN_TALK);
	case SCREEN_TALK:
		return true;
	case SCREEN_TALK2:
		return lastScreen != SCREEN_TALK_CHOICE;
	case SCREEN_ROOM_DESC:
		return true;
	case SCREEN_POW:

	case SCREEN_TALK_CHOICE:
		return lastScreen != SCREEN_TALK2;
	default:
		return true;
	}
}

void GameDrawState(){
	switch (g_state.screen) {
	case SCREEN_ROOM_MENU:
		ClearMenuItems();
		for (uint8_t i=0; i<7; ++i)
		{
			if (i==1) {
				if (rooms[g_state.room].personId == 0)
					continue;
				if ((g_state.room==0) && checkFlag(FLAG_MANAGER))
					continue;
			}
			if ((i==6) && (g_state.room == 10))
				continue;
			if ((i==6) && (g_state.room == 11))
				continue;
			AddMenuItem(room_menu[i].idx, room_menu[i].label);
		}
		DisplayMenu(rooms[g_state.room].name, rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_MOVE_MENU:
		LoadRooms();
		DisplayMenu("Go to Where?", rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_LOOK_MENU:
		rooms[g_state.room].loadItems();
		DisplayMenu("Inspect what?", rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_TAKE_MENU:
		rooms[g_state.room].loadItems();
		DisplayMenu("Take what?", rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_OPEN_MENU:
		rooms[g_state.room].loadItems();
		DisplayMenu("Open what?", rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_HIT_MENU:
		rooms[g_state.room].loadItems();
		DisplayMenu("Hit what?", rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_USE2_MENU:
		rooms[g_state.room].loadItems();
		DisplayMenu("On what?", rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_USE_MENU:
		if (LoadPlayerItems())
			DisplayMenu("Use what?", rooms[g_state.room].image, checkRefresh());
		else {
			setScreen(SCREEN_MESSAGE, 9);
			DisplayMessage(messages[g_state.page], rooms[g_state.room].image, checkRefresh());
		}
		break;
	case SCREEN_LOOK:
		DisplayMessage(items[g_state.selected].description, rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_OPEN:
		DisplayMessage(items[g_state.selected].openmsg, rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_TAKE:
		DisplayMessage(items[g_state.selected].takemsg, rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_TALK:
		DisplayChat(people[rooms[g_state.room].personId].image_addr, g_state.page, checkRefresh());
		break;
	case SCREEN_TALK2:
		DisplayChat(NULL, g_state.page, checkRefresh());
		break;
	case SCREEN_MESSAGE:
		if (g_state.page == 25)
			DisplayMessage(messages[g_state.page], VIRUS_IMG, checkRefresh());
		else
			DisplayMessage(messages[g_state.page], rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_ROOM_DESC:
		DisplayMessage(rooms[g_state.room].description, rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_POW:
		EPD_ClearScreen();
		EPD_DrawImageFlash(0, 52, CANVAS_WIDTH, 104, POW_IMG);
		EPD_FullRefresh();
		break;
	case SCREEN_TALK_CHOICE:
		DrawChoice(rooms[g_state.room].personId, g_state.page);
		break;
	case SCREEN_INPUT_VOLT:
		DisplayMessage("Adjust the voltage at VRef using the scroll wheel until it reads .5 Volts", NULL, checkRefresh());
		break;
	case SCREEN_INPUT_IP:
		DisplayInputIP(checkRefresh());
		break;
	case SCREEN_INPUT_KEY:
		DisplayInputKey(checkRefresh());
		break;
	case SCREEN_ECE_MENU:
		ClearMenuItems();
		AddMenuItem(1, "Torus part");
		AddMenuItem(2, "Cube part");
		AddMenuItem(3, "Cylinder part");
		DisplayMenu("Which part?", rooms[g_state.room].image, checkRefresh());
		break;
	case SCREEN_ENDING:
		DisplayMessage("In court, the evidence you provide is more than enough to make it a easy trial for the jury. They make a conviction with no need for deliberation. Congratulations! CASE CLOSED", ENDING_IMG, checkRefresh());
		break;
	default:
		printf("Unknown screen %d\n", g_state.screen);
		g_state.room = 0;
		setScreen(SCREEN_ROOM_DESC, 0);
		DisplayMessage(rooms[g_state.room].description, rooms[g_state.room].image, true);
		break;
	case SCREEN_AVATAR:
		DisplayAvatar(checkRefresh());
		break;
	}
}

void DrawLogo(uint8_t full) {
	EPD_ClearScreen();

#ifdef CLOCK_EN
	char line[20];
	RTC_DateTypeDef ds;
	RTC_TimeTypeDef ts;
	HAL_RTC_GetDate(&hrtc, &ds, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &ts, RTC_FORMAT_BIN);
	if ((!full) && (lt == ts.Minutes))
		return;
	snprintf(line, 5, "20%d", ds.Year);
	EPD_DrawLine2(40, 134, line, 0);
	//snprintf(line, 10, "%s, %s %d", weekDay[ds.WeekDay-1]);
	//EPD_DrawLine2(25, 144, line, 0);
	snprintf(line, 20, "%s, %s %d", weekDay[ds.WeekDay-1], months[ds.Month-1], ds.Date);
	EPD_DrawLine2(0, 194, line, 0);
	//snprintf(line, 9, "%2d:%02d %s", ts.Hours%12, ts.Minutes, (ts.Hours>12)?"PM":"AM");
	//EPD_DrawLine2(28, 164, line, 0);
	if (abs(lt - ts.Minutes) > 10)
		full = true;
	//printf("%02d:%02d:%02d.%03d\n", ts.Hours,ts.Minutes,ts.Seconds, ts.SubSeconds);
	EPD_DrawTime(154, ts.Hours, ts.Minutes, lt);
	lt = ts.Minutes;
#endif

	if (full || (!logo)) {
		EPD_DrawImageFlash(0, 0, CANVAS_WIDTH, 134, LOGO_IMG);
		EPD_FullRefresh();
	}
	else
		EPD_PartialRefresh(0, 154, 104, 21);
	logo = true;
}

void GameTick() {
	if (logo) {
		RTC_TimeTypeDef ts;
		RTC_DateTypeDef ds;
		HAL_RTC_GetTime(&hrtc, &ts, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &ds, RTC_FORMAT_BIN);
		if (lt != ts.Minutes)
			DrawLogo(0);
	}
}

void GamePause(void) {

	DrawLogo(!logo);
	setLastTime(lt);
	if (g_state.screen == SCREEN_USE2_MENU)
		g_state.screen = SCREEN_ROOM_MENU;
	saveState((uint32_t *)&g_state);
}

void GameEvent(uint8_t event) {
	//printf("Gameevent: %d\n", event);
	uint8_t idx;
	if (logo) {
		if ((event == EVENT_KEY_UP) || (event == EVENT_KEY_DOWN) || (event == EVENT_KEY_SELECT)) {
			GameDrawState();
			logo = 0;
		}
		return;
	}
	switch(event) {
	case EVENT_KEY_UP:
		if (g_state.screen & SCREEN_MENU) {
			DisplayMenuUp();
		}
		else if (g_state.screen == SCREEN_TALK_CHOICE)
			DisplayChoiceUp();
		else if (g_state.screen == SCREEN_INPUT_VOLT)
			SetDac(true);
		else if (g_state.screen == SCREEN_INPUT_IP)
			DisplayInputIPUp();
		else if (g_state.screen == SCREEN_INPUT_KEY)
			DisplayInputKeyUp();
		else if (g_state.screen == SCREEN_AVATAR)
			DisplayAvatarUp(0);
		break;
	case EVENT_KEY_DOWN:
		if (g_state.screen & SCREEN_MENU) {
			DisplayMenuDown();
		}
		else if (g_state.screen == SCREEN_TALK_CHOICE)
			DisplayChoiceDown();
		else if (g_state.screen == SCREEN_INPUT_VOLT)
			SetDac(false);
		else if (g_state.screen == SCREEN_INPUT_IP)
			DisplayInputIPDown();
		else if (g_state.screen == SCREEN_INPUT_KEY)
			DisplayInputKeyDown();
		else if (g_state.screen == SCREEN_AVATAR)
			DisplayAvatarUp(1);
		break;
	case EVENT_KEY_SELECT:
		switch (g_state.screen) {
		case SCREEN_MOVE_MENU:
			g_state.room = DisplayMenuSelect();
			setScreen(SCREEN_ROOM_DESC, 0);
			if (checkFlag(FLAG_FIRE_OUT) && (g_state.room == 4)){
				g_state.room = 13;
				setScreen(SCREEN_TALK, 105);
			}
			if ((g_state.room == 2) && checkFlag(FLAG_CAGE) && checkFlag(FLAG_NFC1) && checkFlag(FLAG_NFC2) && checkFlag(FLAG_NFC3) && checkFlag(FLAG_NFC4) && checkFlag(FLAG_NFC5)) {
				g_state.room = 11;
				setScreen(SCREEN_ROOM_DESC, 0);
				setFlag(FLAG_FIRE);
			}
			break;
		case SCREEN_LOOK_MENU:
			g_state.selected = DisplayMenuSelect();
			setScreen(SCREEN_LOOK, 0);
			if (g_state.selected == 35)
					setFlag(FLAG_STEGO);
			break;
		case SCREEN_TAKE_MENU:
			idx = DisplayMenuSelect();
			if (items[idx].takemsg == NULL) {
				setScreen(SCREEN_MESSAGE, 2);
			}
			else {
				if (items[idx].takeflag != 255)
					setFlag(items[idx].takeflag);
				g_state.selected = idx;
				setScreen(SCREEN_TAKE, 0);
			}
			break;
		case SCREEN_OPEN_MENU:
			idx = DisplayMenuSelect();
			if ((idx == 1) && (!checkFlag(FLAG_MANAGER))) { //Can't open desk until manager is gone
				setScreen(SCREEN_MESSAGE, 10);
			}
			else if ((idx == 12) && (!checkFlag(FLAG_SCREW))) { //Can't open panel without a screwdriver
				setScreen(SCREEN_MESSAGE, 11);
			}
			else if (items[idx].openmsg == NULL) {
				setScreen(SCREEN_MESSAGE, 1);
			}
			else {
				if (items[idx].openflag != 255)
					setFlag(items[idx].openflag);
				g_state.selected = idx;
				setScreen(SCREEN_OPEN, 0);
			}
			break;
		case SCREEN_HIT_MENU:
			g_state.selected = DisplayMenuSelect();
			if ((g_state.selected == 18) && (!checkFlag(FLAG_HIT_BOOKS))) {
				setScreen(SCREEN_MESSAGE, 6);
				setFlag(FLAG_HIT_BOOKS);
			}
			else if ((g_state.selected == 14)){
				if (!checkFlag(FLAG_FIRE))
					setScreen(SCREEN_MESSAGE, 8);
				else if (checkFlag(FLAG_GLASS))
					setScreen(SCREEN_MESSAGE, 12);
				else {
					setScreen(SCREEN_MESSAGE, 7);
					setFlag(FLAG_GLASS);
				}
			}
			else if ((g_state.selected == 4) ||
				(g_state.selected == 6) ||
				(g_state.selected == 11) ||
				(g_state.selected == 16) ||
				(g_state.selected == 22) ||
				(g_state.selected == 31)) {
				setScreen(SCREEN_MESSAGE, 8);
			}
			else
				setScreen(SCREEN_MESSAGE, 5);
			break;
		case SCREEN_USE2_MENU:
			{
				uint8_t u2= DisplayMenuSelect();
				if ((g_state.selected == 26) && (u2 == 17)) { //Battery on radio
					setFlag(FLAG_BATT_IN);
					if (checkFlag(FLAG_ANTENNA))
						setScreen(SCREEN_MESSAGE, 15);
					else
						setScreen(SCREEN_MESSAGE, 13);
				}
				else if ((g_state.selected == 10) && (u2 == 17)) { //Hanger on radio
					setFlag(FLAG_ANTENNA);
					if (checkFlag(FLAG_BATT_IN))
						setScreen(SCREEN_MESSAGE, 16);
					else
						setScreen(SCREEN_MESSAGE, 14);
				}
				else if ((g_state.selected == 21) && (u2 == 12)) { //screwdriver on panel
					setFlag(FLAG_PANEL);
					g_state.selected = 12;
					setScreen(SCREEN_OPEN, 0);
				}
				else if ((g_state.selected == 5) && (u2 == 32)) { //tape on fingerprints
					setFlag(FLAG_PRINTS);
					setScreen(SCREEN_MESSAGE, 18);
				}
				else if ((g_state.selected == 41) && (u2 == 29)) { //flashlight on tunnel
					setFlag(FLAG_MAZE);
					g_state.room = 9;
					setScreen(SCREEN_ROOM_DESC, 0);
				}
				else if ((g_state.selected == 47) && (u2 == 11)) { //electrical part on worker
					g_flags[FLAG_ECE_PART/8] &= ~(1<<(FLAG_ECE_PART%8)); // clear partflag
					if (g_ece == 3) {
						setFlag(FLAG_CAP);
						setScreen(SCREEN_TALK, 51);
					}
					else
						setScreen(SCREEN_TALK, 50);
					saveFlags();
				}
				else if ((g_state.selected == 15) && (u2 == 50)) { //extinguisher on fire
					setFlag(FLAG_FIRE_OUT);
					g_state.room = 12;
					setScreen(SCREEN_MESSAGE, 30);
				}
				else if ((g_state.selected == 19) && (u2 == 4)) { //Cake on boss
					setScreen(SCREEN_TALK, 12);
				}
				else if ((g_state.selected == 19) && (u2 == 6)) { //Cake on comp guy
					setScreen(SCREEN_TALK, 60);
				}
				else if ((g_state.selected == 19) && (u2 == 11)) { //Cake on worker
					setScreen(SCREEN_TALK, 49);
				}
				else if ((g_state.selected == 19) && (u2 == 16)) { //Cake on girl
					setScreen(SCREEN_TALK, 88);
				}
				else if ((g_state.selected == 19) && (u2 == 22)) { //Cake on it
					setScreen(SCREEN_TALK, 76);
				}
				else if ((g_state.selected == 19) && (u2 == 31)) { //Cake on eng
					setScreen(SCREEN_TALK, 98);
				}
				else {
					setScreen(SCREEN_MESSAGE, 4);
				}
			}
			break;
		case SCREEN_USE_MENU:
			g_state.selected = DisplayMenuSelect();
			setScreen(SCREEN_USE2_MENU,0);
			break;
		case SCREEN_ROOM_MENU:
			idx = DisplayMenuSelect();
			if (idx == SCREEN_TALK) {
				if (rooms[g_state.room].personId){
					g_state.page = 0;
					people[rooms[g_state.room].personId].click(g_state.page);
				}
				else {
					setScreen(SCREEN_MESSAGE, 3);
				}
			}
			else
				setScreen(idx, 0);
			break;
		case SCREEN_TALK:
		case SCREEN_TALK2:
			people[rooms[g_state.room].personId].click(g_state.page);
			break;
		case SCREEN_MESSAGE:
			if ((g_state.page == 15) || (g_state.page == 16)) {
				setScreen(SCREEN_MESSAGE, 17);
				break;
			}
			if (g_state.page == 18) {
				setScreen(SCREEN_MESSAGE, 23);
				setFlag(FLAG_CAGE);
				break;
			}
			if (g_state.page == 23) {
				setScreen(SCREEN_TALK, 102);
				break;
			}
			if (g_state.page == 21) {
				g_state.room = 10;
				setScreen(SCREEN_ROOM_DESC, 0);
				break;
			}
			if (g_state.page == 22) {
				setScreen(SCREEN_ROOM_MENU, 0);
				break;
			}
			if (g_state.page == 24) {
				setScreen(SCREEN_MESSAGE, 25);
				break;
			}
			if (g_state.page == 25) {
				//No escape this way
				break;
			}
			if (g_state.page == 26) {
				g_state.room = 9;
				setScreen(SCREEN_ROOM_DESC, 0);
				break;
			}
			if (g_state.page == 30) {
				setScreen(SCREEN_TALK, 103);
				break;
			}
			if (g_state.page == 31) {
				setScreen(SCREEN_ENDING, 0);
				break;
			}
			setScreen(SCREEN_ROOM_DESC,0);
			break;
		case SCREEN_OPEN:
			if (g_state.selected == 46)
				setScreen(SCREEN_ECE_MENU, 0);
			else
				setScreen(SCREEN_ROOM_MENU,0);
			break;
		case SCREEN_ROOM_DESC:
			if ((g_state.room == 9) && (!checkFlag(FLAG_VIRUS)) ){
				setScreen(SCREEN_MESSAGE, 22);
				break;
			}
		case SCREEN_TAKE:
		case SCREEN_LOOK:
			setScreen(SCREEN_ROOM_MENU,0);
			break;
		case SCREEN_POW:
			setScreen(SCREEN_MESSAGE, 31);
			break;
		case SCREEN_TALK_CHOICE:
			HandleChoice(rooms[g_state.room].personId, g_state.page, DisplayChoiceSelect());
			break;
		case SCREEN_INPUT_VOLT:
			if ((dval > 650) && (dval < 700)) {
				setFlag(FLAG_VOLT);
				setScreen(SCREEN_TALK, 47);
			}
			else {
				setScreen(SCREEN_TALK, 44);
			}
			HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
			stay_awake=false;
			break;
		case SCREEN_INPUT_IP:
			{
				char* x=DisplayInputIPSelect();
				if (x) {
					if (strcmp("069.142.082.097", x)==0) {
						setFlag(FLAG_PCAP);
						setScreen(SCREEN_TALK, 71);
					}
					else
						setScreen(SCREEN_TALK, 70);
				}
				else
					return;
				break;
			}
		case SCREEN_INPUT_KEY:
			{
				char* x=DisplayInputKeySelect();
				if (x) {
					if (strcmp("payday", x)==0) {
						setFlag(FLAG_RANSOM);
						setScreen(SCREEN_TALK, 73);
					}
					else
						setScreen(SCREEN_TALK, 72);
				}
				else
					return;
				break;
			}
		case SCREEN_AVATAR:
			switch(DisplayAvatarSelect()) {
			case 0:
				setAvatar(D_GUY_IMG);
				break;
			case 1:
				setAvatar(D_GIRL_IMG);
				break;
			default:
				setAvatar(D_GUY_IMG);
			}
			saveFlags();
			setScreen(SCREEN_TALK, 1);
			break;
		case SCREEN_ECE_MENU:
			g_ece = DisplayMenuSelect();
			setFlag(FLAG_ECE_PART);
			setScreen(SCREEN_MESSAGE, 26+g_ece);
			break;
		}
		GameDrawState();
		break;
	case EVENT_DOUBLE_TAP:
		if ((g_state.screen == SCREEN_LOOK) && (g_state.selected == 35)) {
			setScreen(SCREEN_MESSAGE, 19);
			setFlag(FLAG_BOX_OPEN);
			GameDrawState();
		}
	case EVENT_HIT:
		if ((g_state.screen == SCREEN_TALK) && (g_state.page > 103)) {
			setScreen(SCREEN_POW, 0);
			setFlag(FLAG_PUNCH);
			GameDrawState();
		}
		break;
	case EVENT_NFC:
		puts("checking NFC:");
		GameCheckNFC();
		GameSetNFC();
		break;
	case EVENT_WIRE_JUMP:
		if (g_state.room == 10) {
			setFlag(FLAG_JUMPED);
			setScreen(SCREEN_MESSAGE, 26);
			GameDrawState();
		}
		break;
	}

}

void GameCheckNFC() {
	int s = NFC_CheckEvent();

	if (s) {
		notifyLED(true, true, true);
    }
	 for (int i=0; i<5; ++i) {
		 if (checkFlag(FLAG_NFC_RDY) && (s & (1<<i))) {
			 printf("NFC FLAG %d found\n", i+1);
			 setFlag(FLAG_NFC1 + i);
		 }
	 }
	 if (s & (1<<6)) {
		setFlag(FLAG_FLASHLIGHT);
		printf("NFC flashlight found\n");
		setScreen(SCREEN_MESSAGE, 20);
		GameDrawState();
	}
	if ((s & (1<<7)) && checkFlag(FLAG_CAGE) && (!checkFlag(FLAG_VIRUS))) {
		setFlag(FLAG_VIRUS);
		setScreen(SCREEN_MESSAGE, 21);
		GameDrawState();
	}
	if (s & (1<<8))
		puts("Date set via NFC");
}

void GameSetNFC() {
	char line[256] = {0};
	snprintf(line, sizeof(line), "%08x: %s", NFC_GetID() ,GameFlags());

	NFC_WriteText(line);
}
void GameReset() {
	HAL_StatusTypeDef status;
	status = HAL_FLASHEx_DATAEEPROM_Unlock();
	for(int i=0; i< sizeof(g_flags); ++i) {
		status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, GAME_FLAGS+i, 0);
	}
	status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, GAME_AVATAR, 0);
	status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, GAME_CHECK, 0);
	status = HAL_FLASHEx_DATAEEPROM_Lock();
	GameInit();
}

GameLandmarks GetLandmarks() {
	return g_landmarks;
}

void updateLandmarks() {
	if (checkFlag(FLAG_NFC1) && checkFlag(FLAG_NFC2) && checkFlag(FLAG_NFC3) && checkFlag(FLAG_NFC4) && checkFlag(FLAG_NFC5))
		g_landmarks.group[0] = 4;
	else if (checkFlag(FLAG_RANSOM))
		g_landmarks.group[0] = 3;
	else if (checkFlag(FLAG_PCAP))
		g_landmarks.group[0] = 2;
	else if (checkFlag(FLAG_EMAIL))
		g_landmarks.group[0] = 1;
	else
		g_landmarks.group[0] = 0;

	if (checkFlag(FLAG_JUMPED))
		g_landmarks.group[1] = 4;
	else if (checkFlag(FLAG_BOX_OPEN))
		g_landmarks.group[1] = 3;
	else if (checkFlag(FLAG_STEGO))
		g_landmarks.group[1] = 2;
	else if (checkFlag(FLAG_MANAGER))
		g_landmarks.group[1] = 1;
	else
		g_landmarks.group[1] = 0;

	if (checkFlag(FLAG_FIRE_OUT))
		g_landmarks.group[2] = 4;
	else if (checkFlag(FLAG_CONSOLE))
		g_landmarks.group[2] = 3;
	else if (checkFlag(FLAG_VOLT))
		g_landmarks.group[2] = 2;
	else if (checkFlag(FLAG_SCREW))
		g_landmarks.group[2] = 1;
	else
		g_landmarks.group[2] = 0;

	if (checkFlag(FLAG_PUNCH))
		g_landmarks.group[3] = 4;
	else if (checkFlag(FLAG_PRINTS))
		g_landmarks.group[3] = 3;
	else if (checkFlag(FLAG_FLASHLIGHT))
		g_landmarks.group[3] = 2;
	else if (checkFlag(FLAG_ANTENNA) && checkFlag(FLAG_BATT_IN))
		g_landmarks.group[3] = 1;
	else
		g_landmarks.group[3] = 0;

}
