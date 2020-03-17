#ifndef GDEW0215I5F
#define GDEW0215I5F

#include "main.h"

#define GDEW0215I5F_WIDTH	104
#define GDEW0215I5F_HEIGHT	212
#define MAX_BUSY			5000

#define BIT(x, b) (((x) >> (b)) & 1)

void gdew0213i5f_Init(void);
void gdew0213i5f_Clear(void);
void gdew0213i5f_Draw(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *data);
void gdew0213i5f_FullRefresh(void);
void gdew0213i5f_DrawPartial(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ox, uint8_t oy, uint8_t pitch, const uint8_t *data, const uint8_t *olddata);
void gdew0213i5f_Sleep(void);


#endif
