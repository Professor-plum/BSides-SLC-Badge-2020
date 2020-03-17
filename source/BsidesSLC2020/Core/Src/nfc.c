/*
 * nfc.c
 *
 *  Created on: Oct 19, 2019
 *      Author: plum
 */


#include "nfc.h"
#include <stdio.h>
#include <stdlib.h>
#include "tagtype5_wrapper.h"
#include "lib_NDEF_Text.h"
#include "custom_bus.h"
#include "flags.h"
#include <string.h>

ST25DV_Object_t st25dv;

bool NFC_Init() {
	ST25DV_IO_t IO = {BSP_I2C1_Init, BSP_I2C1_DeInit, BSP_I2C1_IsReady, BSP_I2C1_WriteReg16, BSP_I2C1_ReadReg16, HAL_GetTick};
	ST25DV_RegisterBusIO (&st25dv, &IO);
	if (St25Dv_Drv.Init(&st25dv) != NDEF_OK) {
		printf("NFC init failed!\n");
		return false;
	}

	//if (ST25DV_ResetMBEN_Dyn(&st25dv) != NDEF_OK)
	//	printf("NFC Reset MBEN failed!\n");
	NfcTag_SelectProtocol(NFCTAG_TYPE5);
	if( NfcType5_NDEFDetection() != NDEF_OK )
	{
		CCFileStruct.MagicNumber = NFCT5_MAGICNUMBER_E1_CCFILE;
		CCFileStruct.Version = NFCT5_VERSION_V1_0;
		CCFileStruct.MemorySize = ( ST25DV_MAX_SIZE / 8 ) & 0xFF;
		CCFileStruct.TT5Tag = 0x05;
		/* Init of the Type Tag 5 component (M24LR) */
		if( NfcType5_TT5Init( ) != NDEF_OK) {
			printf("Error setting tag type to 5!\n");
			return false;
		}
	}

	ST25DV_PASSWD passwd = {0,0};

	if (ST25DV_PresentI2CPassword(&st25dv, passwd) != NDEF_OK) {
		printf("Bad password!\n");
		return false;
	}
	if (St25Dv_Drv.ConfigIT(&st25dv, 0xC0) != NDEF_OK){ //GPO_EN | WRITE_EN | FIELD_CHANGE);
		printf("NFC GPIO failed\n");
		return false;
	}

	return true;
}


void NFC_PowerDown() {
	HAL_GPIO_WritePin(NFC_PWR_GPIO_Port, NFC_PWR_Pin, GPIO_PIN_RESET); //Turn off NFC

}

int fakeut16tochars(char* dest, char* src, uint16_t max) {
	int i;
	for(i=0; (i<max) && src[i*2+2]; ++i)
		dest[i]=src[i*2+2];
	return i;
}

int NFC_CheckEvent() {
	char line[512]={0};
	sRecordInfo_t record;
	NDEF_Text_info_t textinfo;
	int ret = 0;

	NDEF_ReadNDEF(NDEF_Buffer);
	NDEF_IdentifyNDEF(&record, NDEF_Buffer);
	if (NDEF_ReadText(&record, &textinfo ) != NDEF_OK) {
		printf("NFC busy!, can't read!\n");
		return ret;
	}

	if (textinfo.encoding == NDEF_TEXT_UTF16)
		fakeut16tochars(line, textinfo.text, sizeof(line));
	else
		strncpy(line, textinfo.text, sizeof(line));

	//printf("NFC: \"%s\"\n", line);
	if (strstr(line, "information")) {
		ret |= 1<<0;
	}
	if (strstr(line, "equipment")) {
		ret |= 1<<1;
	}
	if (strstr(line, "extensive")) {
		ret |= 1<<2;
	}
	if (strstr(line, "measures")) {
		ret |= 1<<3;
	}
	if (strstr(line, "considerable")) {
		ret |= 1<<4;
	}
	if (strstr(line, "FLASHLIGHT")) {
			ret |= 1<<6;
	}
	if (strstr(line, "EVICT")) {
			ret |= 1<<7;
	}
	if (strncasecmp(line, "Date:", 5)==0) {
		char* ptr = &line[5];
		for (int i=0; i<3; ++i, ++ptr)
			if (*ptr!=' ')
				break;
		if (SetTime(ptr))
			ret |= 1<<8;
	}
	return ret;
}


bool NFC_WriteText(char* message) {
	//ST25DV_CreateUserZone(&st25dv, 256, 256, 0, 0); // two user areas
	//NfcTag_SelectProtocol(NFCTAG_TYPE4);
	if (NDEF_WriteText(message) != NDEF_OK) {
		printf("NFC write Text failed\n");
		return false;
	}
	//NDEF_WriteURI(&URI);
	//ST25DV_WriteRFZxSS(&st25dv, ST25DV_PROT_ZONE1, RfProtZone);
	//ST25DV_SetGPO_en_Dyn(&st25dv);
	//ST25DV_ResetRFDisable_Dyn(&st25dv);
	return true;
}


uint32_t NFC_GetID() {
	ST25DV_UID uid;
	if (ST25DV_ReadUID(&st25dv,  &uid ) == NDEF_OK)
		return uid.LsbUid;
	return 0;
}
