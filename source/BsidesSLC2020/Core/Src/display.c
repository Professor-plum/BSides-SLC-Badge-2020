/*
 * Display.c
 *
 *  Created on: Oct 30, 2019
 *      Author: plum
 */


#include "display.h"
#include "images.h"
#include "w25qxx.h"

MenuItem menu_items[10];
uint8_t selected, items_count;

uint32_t pavatar;

char input[17];

void setAvatar(uint32_t img) {
	pavatar = img;
}

uint32_t getAvatar() {
	return pavatar;
}

void ClearMenuItems() {
	items_count=0;
}

void AddMenuItem(uint8_t id, const char* label) {
	menu_items[items_count].idx=id;
	strcpy(menu_items[items_count++].label, label);
}

void DisplayMessage(const char* message, uint32_t img, bool full) {
	EPD_ClearArea(0, 0, CANVAS_WIDTH, 130);
	EPD_DrawString(0, message);
	if (full) {
		if (img) {
			EPD_FillArea(0, 130, CANVAS_WIDTH, 4);
			EPD_DrawImageFlash(0, 134, CANVAS_WIDTH, 78, img);
		}
		EPD_FullRefresh();
	}
	else
		EPD_PartialRefresh(0, 0, CANVAS_WIDTH, 130);
}

void DisplayMenu(const char* header, uint32_t img, bool full) {
	int i;
	selected = 0;
	EPD_ClearArea(0, 0, CANVAS_WIDTH, 130);
	EPD_DrawLine2(0, 0, header, 0);
	for (i=0; i<items_count; ++i){
		if (menu_items[i].idx == MENU_END)
			break;
		EPD_DrawLine2(10, 15+i*10, menu_items[i].label, 0);
	}
	EPD_WriteChar(0, 15+selected*10, 0x7F, 0);
	if (full) {
		if (img){
			EPD_FillArea(0, 130, CANVAS_WIDTH, 4);
			EPD_DrawImageFlash(0, 134, CANVAS_WIDTH, 78, img);
		}
		EPD_FullRefresh();
	}
	else
		EPD_PartialRefresh(0, 0, CANVAS_WIDTH, 130);
}

void UpdateDisplayMenu() {
	EPD_ClearArea(0,15,8,(items_count)*10);
	EPD_WriteChar(0, 15+selected*10, 0x7F, 0);
	EPD_PartialRefresh(0, 15, 8, (items_count)*10);
	//EPD_TextRefresh();
}

void DisplayMenuDown() {
	if (++selected >= items_count)
		selected = 0;
	UpdateDisplayMenu();
}

void DisplayMenuUp() {
	if (selected == 0)
		selected=items_count-1;
	else
		--selected;
	UpdateDisplayMenu();
}

uint8_t DisplayMenuSelect() {
	return menu_items[selected].idx;
}

void DrawChatBar(uint8_t y) {
	//uint8_t y=127;
	y+=3;
	for (int i=0; i<CANVAS_WIDTH; ++i)
		EPD_WritePixel(i, y, 1);
	for (int i=49; i<55; ++i)
		EPD_WritePixel(i, y, 0);
	EPD_WritePixel(0, y, 0);
	EPD_WritePixel(CANVAS_WIDTH-1, y, 0);
	EPD_WritePixel(0, y-1, 1);
	EPD_WritePixel(CANVAS_WIDTH-1, y-1, 1);
	EPD_WritePixel(49, y+1, 1);
	EPD_WritePixel(54, y+1, 1);
	EPD_WritePixel(49, y+2, 1);
	EPD_WritePixel(54, y+2, 1);
	EPD_WritePixel(50, y+3, 1);
	EPD_WritePixel(53, y+3, 1);
	EPD_WritePixel(50, y+4, 1);
	EPD_WritePixel(53, y+4, 1);
	EPD_WritePixel(51, y+5, 1);
	EPD_WritePixel(52, y+5, 1);
	EPD_WritePixel(51, y+6, 1);
	EPD_WritePixel(52, y+6, 1);
}

void DisplayChat(uint32_t avatar, uint8_t dialog_id, bool full) {
	char message[256];
	W25qxx_ReadBytes(message, DIALOG_BASE + (dialog_id * 256), 256);
	EPD_ClearScreen();
	if (avatar==0)
		avatar=pavatar;
	if (full)
		EPD_DrawImageFlash(0, 134, 104, 78, avatar);
	uint8_t y = EPD_DrawString(0, message);
	DrawChatBar(y);
	if (full)
		EPD_FullRefresh();
	else
		EPD_PartialRefresh(0, 0, CANVAS_WIDTH, 134);
}

/*void DisplayUpdateChat(const char* message) {
	EPD_ClearArea(0, 0, CANVAS_WIDTH, 134);
	uint8_t y = EPD_DrawString(0, message);
	DrawChatBar(y);
	EPD_PartialRefresh(0, 0, CANVAS_WIDTH, 134);
}*/

void DisplayChoice(const char* message, const char* choices[], bool full) {
	ClearMenuItems();
	for (int i=0; choices[i]; ++i)
		AddMenuItem(i, choices[i]);
	EPD_DrawImageFlash(0, 134, 104, 78, pavatar);
	DisplayMenu(message, getAvatar(), full);
}

char* DisplayChoiceSelect() {
	return menu_items[selected].label;
}

void DisplayChoiceUp() {
	DisplayMenuUp();
}

void DisplayChoiceDown() {
	DisplayMenuDown();
}

void DisplayInputIP(bool full) {
	selected=0;
	strcpy(input,"000.000.000.000");
	EPD_ClearArea(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
	EPD_DrawString(0, "Enter the IP address where the malware calls back to");
	EPD_WriteChar(4, 150, input[0], true);
	EPD_DrawLine2(10, 150, &input[1], false);
	EPD_FullRefresh();
}

void DisplayInputIPUp() {
	char c = ++input[selected];
	if (c>'9')
		c='0';
	input[selected]=c;
	EPD_DrawLine(4, 150, &input[0], selected, false);
	EPD_WriteChar(4+selected*6, 150, input[selected], true);
	EPD_DrawLine(10+selected*6, 150, &input[selected+1], 14-selected, false);
	EPD_PartialRefresh(0, 150, CANVAS_WIDTH, 10);
}

void DisplayInputIPDown() {
	char c = --input[selected];
	if (c<'0')
		c='9';
	input[selected]=c;
	EPD_DrawLine(4, 150, &input[0], selected, false);
	EPD_WriteChar(4+selected*6, 150, input[selected], true);
	EPD_DrawLine(10+selected*6, 150, &input[selected+1], 14-selected, false);
	EPD_PartialRefresh(0, 150, CANVAS_WIDTH, 10);
}

char* DisplayInputIPSelect() {
	selected++;
	if ((selected %4) == 3)
		selected++;
	if (selected >= 16)
		return input;
	EPD_DrawLine(4, 150, &input[0], selected, false);
	EPD_WriteChar(4+selected*6, 150, input[selected], true);
	EPD_DrawLine(10+selected*6, 150, &input[selected+1], 14-selected, false);
	EPD_PartialRefresh(0, 150, CANVAS_WIDTH, 10);
	return NULL;
}

void DisplayInputKey(bool full) {
	selected=0;
	strcpy(input,"aaaaaa");
	EPD_ClearArea(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
	EPD_DrawString(0, "Enter the ransomware encryption key");
	EPD_WriteChar(34, 150, input[0], true);
	EPD_DrawLine2(40, 150, &input[1], false);
	EPD_FullRefresh();
}

void DisplayInputKeyUp() {
	char c = ++input[selected];
	if (c>'z')
		c='a';
	input[selected]=c;
	EPD_DrawLine(34, 150, &input[0], selected, false);
	EPD_WriteChar(34+selected*6, 150, input[selected], true);
	EPD_DrawLine(40+selected*6, 150, &input[selected+1], 5-selected, false);
	EPD_PartialRefresh(0, 150, CANVAS_WIDTH, 10);
}

void DisplayInputKeyDown() {
	char c = --input[selected];
	if (c<'a')
		c='z';
	input[selected]=c;
	EPD_DrawLine(34, 150, &input[0], selected, false);
	EPD_WriteChar(34+selected*6, 150, input[selected], true);
	EPD_DrawLine(40+selected*6, 150, &input[selected+1], 5-selected, false);
	EPD_PartialRefresh(0, 150, CANVAS_WIDTH, 10);
}

char* DisplayInputKeySelect() {
	selected++;
	if (selected >= 6)
		return input;
	EPD_DrawLine(34, 150, &input[0], selected, false);
	EPD_WriteChar(34+selected*6, 150, input[selected], true);
	EPD_DrawLine(40+selected*6, 150, &input[selected+1], 5-selected, false);
	EPD_PartialRefresh(0, 150, CANVAS_WIDTH, 10);
	return NULL;
}

void DisplayAvatar(bool full) {
	EPD_ClearArea(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
	EPD_DrawString(0, "Select your avatar");
	selected = 0;
	EPD_DrawImageFlash(0, 32, 104, 78, D_GUY_IMG);
	EPD_DrawImageFlash(0, 134, 104, 78, D_GIRL_IMG);
	EPD_WriteChar(0, 71, 0x7F, false);
	EPD_FullRefresh();
}

void DisplayAvatarUp(uint8_t i) {
	selected = i;
	EPD_ClearArea(0,71,8,10);
	EPD_ClearArea(0,171,8,10);
	EPD_WriteChar(0, 71+selected*100, 0x7F, 0);
	EPD_PartialRefresh(0, 71, 8, 110);
}

int DisplayAvatarSelect() {
	return selected;
}
