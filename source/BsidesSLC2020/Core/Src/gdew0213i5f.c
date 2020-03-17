#include <gdew0213i5f.h>

extern SPI_HandleTypeDef hspi1;

/**
 * full screen update LUT
**/
static const unsigned char gdew0213i5f_lut_vcomDC[] = {
    0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_ww[] = {
    0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
    0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_bw[] = {
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x0F, 0x0F, 0x00, 0x00, 0x03,
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_wb[] = {
    0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_bb[] = {
    0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


/**
 * partial screen update LUT
**/
static const unsigned char gdew0213i5f_lut_vcom1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    ,0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_ww1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_bw1[] = {
    0x80, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_wb1[] = {
    0x40, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char gdew0213i5f_lut_bb1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void gdew0213i5f_Reset(void)
{
    HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void gdew0213i5f_SendCommand(uint8_t Reg)
{
    HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t*) &Reg, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void gdew0213i5f_SendData(uint8_t Data)
{
	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &Data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void gdew0213i5f_ReadBusy(void)
{
	//gdew0213i5f_SendCommand(0X71);
	for (uint16_t i=0; i<MAX_BUSY; ++i) {
		if (HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) == GPIO_PIN_SET)
			return;
		HAL_Delay(1);
	}
}

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
static void gdew0213i5f_SetFullReg(void)
{
	gdew0213i5f_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
	gdew0213i5f_SendData(0xb7);		//WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

    unsigned int count;
    gdew0213i5f_SendCommand(0x20);
    for(count=0; count<44; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_vcomDC[count]);
    }

    gdew0213i5f_SendCommand(0x21);
    for(count=0; count<42; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_ww[count]);
    }

    gdew0213i5f_SendCommand(0x22);
    for(count=0; count<42; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_bw[count]);
    }

    gdew0213i5f_SendCommand(0x23);
    for(count=0; count<42; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_wb[count]);
    }

    gdew0213i5f_SendCommand(0x24);
    for(count=0; count<42; count++) {
        gdew0213i5f_SendData(gdew0213i5f_lut_bb[count]);
    }
}

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
static void gdew0213i5f_SetPartReg(void)
{
	gdew0213i5f_SendCommand(0x82);			//vcom_DC setting
	gdew0213i5f_SendData(0x00); //Maybe 0x8? -0.10V vs -0.50V
	gdew0213i5f_SendCommand(0X50);
	gdew0213i5f_SendData(0x47);
	gdew0213i5f_ReadBusy();

    unsigned int count;
    gdew0213i5f_SendCommand(0x20);
    for(count=0; count<44; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_vcom1[count]);
    }

    gdew0213i5f_SendCommand(0x21);
    for(count=0; count<42; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_ww1[count]);
    }

    gdew0213i5f_SendCommand(0x22);
    for(count=0; count<42; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_bw1[count]);
    }

    gdew0213i5f_SendCommand(0x23);
    for(count=0; count<42; count++) {
    	gdew0213i5f_SendData(gdew0213i5f_lut_wb1[count]);
    }

    gdew0213i5f_SendCommand(0x24);
    for(count=0; count<42; count++) {
        gdew0213i5f_SendData(gdew0213i5f_lut_bb1[count]);
    }
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void gdew0213i5f_TurnOnDisplay(void)
{
	gdew0213i5f_SendCommand(0x12);		 //DISPLAY REFRESH
    HAL_Delay(10);     //!!!The delay here is necessary, 200uS at least!!!
    gdew0213i5f_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void gdew0213i5f_Init(void)
{
	gdew0213i5f_Reset();
	//gdew0213i5f_ReadBusy();

	gdew0213i5f_SendCommand(0x01);	//POWER SETTING
	gdew0213i5f_SendData(0x03);
	gdew0213i5f_SendData(0x00);
	gdew0213i5f_SendData(0x2b);
	gdew0213i5f_SendData(0x2b);
	gdew0213i5f_SendData(0x03);

	gdew0213i5f_SendCommand(0x06);	//boost soft start
	gdew0213i5f_SendData(0x17);     //A
	gdew0213i5f_SendData(0x17);     //B
	gdew0213i5f_SendData(0x17);     //C

	gdew0213i5f_SendCommand(0x04);
	gdew0213i5f_ReadBusy();

	gdew0213i5f_SendCommand(0x00);	//panel setting
    gdew0213i5f_SendData(0xbf);     //LUT from OTP，128x296
    gdew0213i5f_SendData(0x0e);     //VCOM to 0V fast

    gdew0213i5f_SendCommand(0x30);	//PLL setting
    gdew0213i5f_SendData(0x3a);     // 3a 100HZ   29 150Hz 39 200HZ	31 171HZ

    gdew0213i5f_SendCommand(0x61);	//resolution setting
    gdew0213i5f_SendData(GDEW0215I5F_WIDTH);
    gdew0213i5f_SendData((GDEW0215I5F_HEIGHT >> 8) & 0xff);
    gdew0213i5f_SendData(GDEW0215I5F_HEIGHT& 0xff);

    gdew0213i5f_SendCommand(0x82);	//vcom_DC setting
    gdew0213i5f_SendData(0x28); //from source example however
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void gdew0213i5f_Clear(void)
{
    uint8_t Width, Height;
    Width = (GDEW0215I5F_WIDTH % 8 == 0)? (GDEW0215I5F_WIDTH / 8 ): (GDEW0215I5F_WIDTH / 8 + 1);
    Height = GDEW0215I5F_HEIGHT;

    gdew0213i5f_SendCommand(0x91);
    gdew0213i5f_SendCommand(0x90);		//resolution setting
    gdew0213i5f_SendData(0);           //x-start
    gdew0213i5f_SendData(GDEW0215I5F_WIDTH-1);       //x-end

    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(0);     //y-start
    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(GDEW0215I5F_HEIGHT-1);  //y-end
    gdew0213i5f_SendData(0x28);

    gdew0213i5f_SendCommand(0x10);
    for (uint8_t j = 0; j < Height; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(0);
        }
    }

    gdew0213i5f_SendCommand(0x13);
    for (uint8_t j = 0; j < Height; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(0xff);
        }
    }
}

void gdew0213i5f_FullRefresh(void) {
	/*gdew0213i5f_SendCommand(0x90);		//resolution setting
	gdew0213i5f_SendData(0);           //x-start
	gdew0213i5f_SendData(GDEW0215I5F_WIDTH-1);       //x-end

	gdew0213i5f_SendData(0);
	gdew0213i5f_SendData(0);     //y-start
	gdew0213i5f_SendData(0);
	gdew0213i5f_SendData(GDEW0215I5F_HEIGHT-1);  //y-end
	gdew0213i5f_SendData(0x28);*/

	gdew0213i5f_SetFullReg();
	gdew0213i5f_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void gdew0213i5f_Draw(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Image)
{
    uint8_t Width, Height;

    gdew0213i5f_SendCommand(0x91);
    gdew0213i5f_SendCommand(0x90);		//resolution setting
    gdew0213i5f_SendData(x);           //x-start
    gdew0213i5f_SendData(w+x-1);       //x-end

    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(y);     //y-start
    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(y+h-1);  //y-end
    gdew0213i5f_SendData(0x28);

    Width = (w % 8 == 0)? (w / 8 ): (w / 8 + 1);
    Height = h;

    gdew0213i5f_SendCommand(0x10);
    for (uint8_t j = 0; j < Height; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(0x00);
        }
    }

    gdew0213i5f_SendCommand(0x13);
    for (uint8_t j = 0; j < Height; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(Image[i + j * Width]);
        }
    }
    gdew0213i5f_SendCommand(0x92);

}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
/*void gdew0213i5f_DrawPartial(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t pitch, const uint8_t *Image)
{
    // Set partial Windows
    gdew0213i5f_SetPartReg();
    gdew0213i5f_SendCommand(0x91);		//This command makes the display enter partial mode
    gdew0213i5f_SendCommand(0x90);		//resolution setting
    gdew0213i5f_SendData(x);           //x-start
    gdew0213i5f_SendData(w+x-1);       //x-end

    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(y);     //y-start
    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(y+h-1);  //y-end
    gdew0213i5f_SendData(0x28);

    uint8_t Width = (w % 8 == 0)? (w / 8 ): (w / 8 + 1);

    // send data
    gdew0213i5f_SendCommand(0x10);
    for (uint8_t j = 0; j < GDEW0215I5F_HEIGHT; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(flipByte(~Image[i + j * Width]));
        }
    }

    gdew0213i5f_SendCommand(0x13);
    for (uint8_t j = 0; j < GDEW0215I5F_HEIGHT; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(flipByte(Image[i + j * Width]));
        }
    }

    // Set partial refresh
    gdew0213i5f_TurnOnDisplay();
}*/

void gdew0213i5f_DrawPartial(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ox, uint8_t oy, uint8_t pitch, const uint8_t *Image, const uint8_t *oldImage) {
    /* Set partial Windows */
    gdew0213i5f_SetPartReg();
    gdew0213i5f_SendCommand(0x91);		//This command makes the display enter partial mode
    gdew0213i5f_SendCommand(0x90);		//resolution setting
    gdew0213i5f_SendData(x);           //x-start
    gdew0213i5f_SendData(w+x-1);       //x-end

    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(y);     //y-start
    gdew0213i5f_SendData(0);
    gdew0213i5f_SendData(y+h-1);  //y-end
    gdew0213i5f_SendData(0x28);

    uint8_t Width = (w % 8 == 0)? (w / 8 ): (w / 8 + 1);
    pitch/=8;

    /* send data */
    gdew0213i5f_SendCommand(0x10);
    for (uint8_t j = 0; j < GDEW0215I5F_HEIGHT; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(~oldImage[i + ox + (j+oy)* pitch]);
        }
    }

    gdew0213i5f_SendCommand(0x13);
    for (uint8_t j = 0; j < GDEW0215I5F_HEIGHT; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            gdew0213i5f_SendData(~Image[i + ox + (j+oy)* pitch]);
        }
    }

    /* Set partial refresh */
    gdew0213i5f_TurnOnDisplay();
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void gdew0213i5f_Sleep(void)
{
    gdew0213i5f_SendCommand(0X50);
    gdew0213i5f_SendData(0xf7);
    gdew0213i5f_SendCommand(0X02);  	//power off
    gdew0213i5f_SendCommand(0X07);  	//deep sleep
    gdew0213i5f_SendData(0xA5);
}
