#ifndef DISPLAY_H
#define DISPLAY_H

#include "epd.h"
#include <stdint.h>
#include <stdbool.h>


typedef struct {
	char label[23];
	uint8_t idx;
} MenuItem;

#define MAX_MENU_ITEMS	10
#define MENU_END		0xFF

void setAvatar(uint32_t img);
uint32_t getAvatar();

void ClearMenuItems();
void AddMenuItem(uint8_t id, const char* label);

void DisplayMessage(const char* message, uint32_t img, bool full);
void DisplayMenu(const char* header, uint32_t img, bool full);
void DisplayMenuDown();
void DisplayMenuUp();
uint8_t DisplayMenuSelect();

void DisplayChat(uint32_t avatar, uint8_t dialog_id, bool full);
void DisplayUpdateChat(const char* message);

void DisplayChoice(const char* message, const char* choices[], bool full);
void DisplayChoiceUp();
void DisplayChoiceDown();
char* DisplayChoiceSelect();

void DisplayInputIP(bool full);
void DisplayInputIPUp();
void DisplayInputIPDown();
char* DisplayInputIPSelect();

void DisplayInputKey(bool full);
void DisplayInputKeyUp();
void DisplayInputKeyDown();
char* DisplayInputKeySelect();

void DisplayAvatar(bool full);
void DisplayAvatarUp(uint8_t i);
int DisplayAvatarSelect();


#endif
