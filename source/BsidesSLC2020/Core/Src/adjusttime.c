#include "adjusttime.h"
#include "main.h"

RTC_DateTypeDef at_ds;
RTC_TimeTypeDef at_ts;
uint8_t at_sel;

void AdjustTimeDraw() {

}

void AdjustTimeShow() {
	at_sel = 0;
	//HAL_RTC_GetDate(&hrtc, &at_ds, RTC_FORMAT_BIN);
	//HAL_RTC_GetTime(&hrtc, &at_ts, RTC_FORMAT_BIN);

	//full draw
}

void AdjustTimeMove(bool up) {
	uint8_t amax=0, amin=0;
	uint8_t* item;
	switch (at_sel) {
	case 0:
		item = & at_ds.Date;
		amax=31;
		amin=1;
		break;
	case 1:
		item = &at_ds.Month;
		amax=12;
		amin=1;
		break;
	case 2:
		item = &at_ds.Year;
		amax = 99;
		break;
	case 3:
		item = &at_ts.Hours;
		amax = 24;
		amin=1;
		break;
	case 4:
		item = &at_ts.Minutes;
		amax=59;
		break;
	default:
		return;
	}
	if (up) {
		if (*item == amax)
			*item = amin;
		else
			(*item)++;
	}
	else {
		if (*item == amin)
			*item = amax;
		else
			(*item)--;
	}
	AdjustTimeDraw();

	//update selected
}

bool AdjustTimeSelect() {
	at_sel++;
	if (at_sel >= 5) {
		//HAL_RTC_SetDate(&hrtc, &at_ds, RTC_FORMAT_BIN);
		//HAL_RTC_SetTime(&hrtc, &at_ts, RTC_FORMAT_BIN);
		return true;
	}

	//update selected and last
	return false;
}
