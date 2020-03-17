#ifndef EPD_H
#define EPD_H
#include "main.h"

#define CANVAS_WIDTH	104
#define CANVAS_HEIGHT	212
#define	CANVAS_BYTES	(CANVAS_HEIGHT*CANVAS_WIDTH)/8

//void EPD_Init(void);
//void EPD_Sleep(void);
//void EPD_ClearText(void);
void EPD_ClearScreen(void);
void EPD_ClearArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void EPD_FillArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void EPD_DrawImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *image);
void EPD_DrawImageFlash(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint32_t addr);
void EPD_FullRefresh(void);
void EPD_PartialRefresh(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void EPD_WritePixel(uint8_t x, uint8_t y, uint8_t b);

void EPD_WriteChar(uint8_t x, uint8_t y, char c, uint8_t invert);
void EPD_DrawLine(uint8_t x, uint8_t y, const char* str, uint8_t count, uint8_t invert);
void EPD_DrawLine2(uint8_t x, uint8_t y, const char* str, uint8_t invert);
uint8_t EPD_DrawString(uint8_t y, const char* str);

void EPD_DrawTime(uint8_t y, uint8_t hours, uint8_t min, uint8_t oldmin);

void EPD_DrawLowPower(uint8_t x, uint8_t y);

#endif
