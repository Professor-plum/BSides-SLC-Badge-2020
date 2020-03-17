#include <font6x10.h>
#include "epd.h"
#include "gdew0213i5f.h"
#include "w25qxx.h"
#include <string.h>
#include "bigdigits.h"
#include "main.h"

uint8_t canvas[CANVAS_BYTES];
uint8_t oldcanvas[CANVAS_BYTES];


void EPD_ClearScreen(void) {
	memset(canvas, 0xff, CANVAS_BYTES);
}

void EPD_ClearArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	for (uint8_t j=0; j<h; ++j)
		memset(&canvas[(y+j)*(CANVAS_WIDTH/8) + (x/8)], 0xff, w/8);
}

void EPD_FillArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	for (uint8_t j=0; j<h; ++j)
		memset(&canvas[(y+j)*(CANVAS_WIDTH/8) + (x/8)], 0, w/8);
}

uint8_t flipByte(uint8_t x) {
	uint8_t ret=0;
	for (uint8_t i=0; i<8; ++i)
		if (BIT(x, 7-i))
			ret |= (1<<i);
	return ret;
}

void EPD_DrawImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *image) {
	x/=8;
	for (uint8_t j=0; j<h; ++j)
		for (uint8_t i=0; i<w/8; ++i)
			canvas[(y+j)*(CANVAS_WIDTH/8) + (x+i)] = ~flipByte(image[j*(w/8) + i]);
	//gdew0213i5f_Draw(x, y, w, h, image);
}

void EPD_DrawImageFlash(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint32_t image) {
	uint8_t r=w/8;
	for (uint8_t j=0; j<h; ++j) {
		W25qxx_ReadBytes(&canvas[(y+j)*(CANVAS_WIDTH/8) + (x/8)], image + (r * j), r);
	}
	//gdew0213i5f_Draw(x, y, w, h, image);
}

void EPD_FullRefresh(void) {
	gdew0213i5f_Init();
	//gdew0213i5f_Clear();
	gdew0213i5f_Draw(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, canvas);
	gdew0213i5f_FullRefresh();
	gdew0213i5f_Sleep();
	memcpy(oldcanvas, canvas, CANVAS_BYTES);
}

void EPD_PartialRefresh(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	gdew0213i5f_Init();
	//gdew0213i5f_Draw(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, canvas);
	gdew0213i5f_DrawPartial(x, y, w, h, x, y, CANVAS_WIDTH, canvas, oldcanvas);
	//gdew0213i5f_FullRefresh();
	gdew0213i5f_Sleep();
	memcpy(oldcanvas, canvas, CANVAS_BYTES);
}

/*void EPD_TextRefresh(void) {
	gdew0213i5f_DrawPartial(0, CANVAS_WIDTH, CANVAS_WIDTH, CANVAS_HEIGHT, canvas);
}*/


void EPD_WritePixel(uint8_t x, uint8_t y, uint8_t b) {
	if (b)
		canvas[(x + y*CANVAS_WIDTH)/8] &= ~(1 << (7-(x%8)));
	else
		canvas[(x + y*CANVAS_WIDTH)/8] |= (1 << (7-(x%8)));
}

void EPD_WriteChar(uint8_t x, uint8_t y, char c, uint8_t invert) {
	//y-=CANVAS_WIDTH;
	c-=32;
	for (uint8_t i=0; i<FONT_HEIGHT; ++i) {
		uint8_t bits= font6x10[c*FONT_HEIGHT +i];
		if (invert)
			bits = ~bits;
		for (int j=0; j<FONT_WIDTH; ++j)
			EPD_WritePixel(x+j, y+i,(bits >> (7-j))&1);
		//canvas[(x + (y+i)*CANVAS_WIDTH)/8] = bits;
	}

	/*uint16_t idx = 10*(c-' ');
	for (int i=0; i<10; ++i) {
		uint8_t col = Font8_Table[idx++];
		if (invert) col = ~col;
		gdeh0213b73_WritePixel(col);
	}*/
}

void EPD_DrawLine(uint8_t x, uint8_t y, const char* str, uint8_t count, uint8_t invert) {
	for (uint8_t i=0; i< count; ++i) {
		EPD_WriteChar(x +i*FONT_WIDTH, y, str[i], invert);
	}
}

void EPD_DrawLine2(uint8_t x, uint8_t y, const char* str, uint8_t invert) {
	EPD_DrawLine(x, y, str, strlen(str), invert);
}

uint8_t EPD_DrawString(uint8_t y, const char* str){
	uint8_t x=0, safe=0, len, count=0, start=0;
	len = strlen(str);

	//EPD_ClearText();

	while (count<len) {
		while (x<(CANVAS_WIDTH-FONT_WIDTH)) {
			x += FONT_WIDTH;
			//x += _font->glyph[str[count]-_font->first].xAdvance;
			count++;
			if ((count == len)|| (str[count] == '\n')) {
				safe = count;
				break;
			}
			if (str[count] == ' ')
				safe=count;
		}
		EPD_DrawLine(0, y, &str[start], safe-start, 0);
		start=safe+1; //don't draw the space
		count = start;
		y+=FONT_HEIGHT;
		//y += _font->yAdvance;
		x=0;
	}
	return y;

}

void EPD_DrawBigNum(uint8_t x, uint8_t y, uint8_t n) {
	for (uint8_t i=0; i<BD_HEIGHT; ++i) {
		uint8_t bits= bigDigits[(n*BD_HEIGHT+i)*2];
		for (int j=0; j<8; ++j)
			EPD_WritePixel(x+j, y+i,(bits >> (7-j))&1);
		bits= bigDigits[(n*BD_HEIGHT+i)*2+1];
		for (int j=0; j<6; ++j)
			EPD_WritePixel(x+j+8, y+i,(bits >> (7-j))&1);
	}
}

void EPD_DrawTime(uint8_t y, uint8_t hours, uint8_t min, uint8_t oldmin) {
	uint8_t x=12;

	for (uint8_t i=0; i<4; ++i)
		for (uint8_t j=y+4; j< (y+8); ++j)
			EPD_WritePixel(x+i+34,j,1);
	for (uint8_t i=0; i<4; ++i)
		for (uint8_t j=y+14; j< (y+18); ++j)
			EPD_WritePixel(x+i+34,j,1);
	EPD_DrawLine(90, y+12, hours>=12?"PM":"AM", 2, 0);
	hours %=12;
	if (hours == 0)
		hours=12;
	if (hours>9)
		EPD_DrawBigNum(x, y, hours/10);
	EPD_DrawBigNum(x+16, y, hours%10);

	//Setup old buffer
	EPD_DrawBigNum(x+32+8, y, oldmin/10);
	EPD_DrawBigNum(x+48+8, y, oldmin%10);

	memcpy(&oldcanvas[13*y], &canvas[13*y], 13*21);
	EPD_ClearArea(x+32+8, y, 30, 21);

	EPD_DrawBigNum(x+32+8, y, min/10);
	EPD_DrawBigNum(x+48+8, y, min%10);
}

void EPD_DrawLowPower(uint8_t x, uint8_t y) {
	EPD_ClearArea(x, y, 16, 8);
	for (uint8_t i=2; i<15; ++i){
		EPD_WritePixel(x+i, y+1, 1);
		EPD_WritePixel(x+i, y+6, 1);
	}
	for (uint8_t i=2; i<5; ++i){
		EPD_WritePixel(x+2, y+i, 1);
		EPD_WritePixel(x+14, y+i, 1);
	}
	for (uint8_t i=3; i<5; ++i){
		EPD_WritePixel(x+1, y+i, 1);
		EPD_WritePixel(x+12, y+i, 1);
	}
	EPD_PartialRefresh(x,y, 16, 8);
}
