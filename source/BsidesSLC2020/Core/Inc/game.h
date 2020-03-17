#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "flags.h"

#define EVENT_NONE			0
#define EVENT_KEY_UP		1
#define EVENT_KEY_DOWN		2
#define EVENT_KEY_SELECT	3
#define EVENT_DOUBLE_TAP	4
#define EVENT_HIT			5
#define EVENT_NFC			6
#define EVENT_WIRE_JUMP		8



#define SCREEN_MENU		0x40

#define SCREEN_LOOK				1
#define SCREEN_TALK				2
#define SCREEN_OPEN				3
#define SCREEN_TAKE				4
#define SCREEN_ROOM_MENU		SCREEN_MENU
#define SCREEN_LOOK_MENU		SCREEN_MENU + SCREEN_LOOK
#define	SCREEN_OPEN_MENU		SCREEN_MENU + SCREEN_OPEN
#define SCREEN_TAKE_MENU		SCREEN_MENU + SCREEN_TAKE
#define SCREEN_USE_MENU			SCREEN_MENU + 5
#define SCREEN_HIT_MENU			SCREEN_MENU + 6
#define SCREEN_MOVE_MENU		SCREEN_MENU + 7
#define SCREEN_USE2_MENU		SCREEN_MENU + 8
#define SCREEN_ECE_MENU			SCREEN_MENU + 9
#define SCREEN_ROOM_DESC		9
#define SCREEN_MESSAGE			10
#define SCREEN_POW				11
#define SCREEN_TALK_CHOICE		12
#define SCREEN_TALK2			13

#define SCREEN_INPUT_IP		0x30
#define	SCREEN_INPUT_VOLT	0x31
#define SCREEN_INPUT_KEY	0x32
#define SCREEN_AVATAR		0x33
#define SCREEN_ENDING		0x34

typedef struct {
	uint8_t room, screen, page, selected;
} GameState;

typedef struct {
	uint8_t group[4];
} GameLandmarks;

#define MAX_ITEMS	6

void GameInit();
void GameEvent(uint8_t event);
void GamePause(void);
bool checkFlag(flags flag);
void setFlag(flags flag);
void setScreen(uint8_t screen, uint8_t page);
void DrawLogo(uint8_t full);
void GameTick();
void GameReset();
GameLandmarks GetLandmarks();
void GameDrawState();
char* GameFlags();
void GameCheckNFC();
void GameSetNFC();

#endif
