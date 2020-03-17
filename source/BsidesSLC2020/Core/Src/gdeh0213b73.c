/**
  ******************************************************************************
  * @file    gdeh0213b73.c
  * @author  Waylon Grange
  * @version V1.0.1
  * @date    28-7-2019
  * @brief   This file includes the driver for GDEH0213B73 EPD (E Paper Display).
  */

/* Includes ------------------------------------------------------------------*/
#include <gdeh0213b73.h>

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup GDEH0213B73
  * @brief      This file provides a set of functions needed to drive the
  *             GDEH0213B73 EPD (E Paper Display).
  * @{
  */

/** @defgroup GDEH0213B73_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup GDEH0213B73_Private_Defines
  * @{
  */
/**
  * @}
  */ 

/** @defgroup GDEH0213B73_Private_Macros
  * @{
  */
/**
  * @}
  */  

/** @defgroup GDEH0213B73_Private_Variables
  * @{
  */

extern SPI_HandleTypeDef hspi1;

/* Look-up table for the epaper (90 bytes) */
const unsigned char LUT_full[]={
		0xA0,	0x90,	0x50,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x50,	0x90,	0xA0,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0xA0,	0x90,	0x50,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x50,	0x90,	0xA0,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,

		0x0F,	0x0F,	0x00,	0x00,	0x00,
		0x0F,	0x0F,	0x00,	0x00,	0x03,
		0x0F,	0x0F,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
};

const unsigned char LUT_part[]={
		0x40,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x80,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x40,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x80,	0x00, 	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00, 	0x00,	0x00,

		0x0A,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00, 	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,
		0x00, 	0x00,	0x00,	0x00,	0x00,
};

/*EPD_DrvTypeDef   gdeh0213b73_drv =
{
		gdeh0213b73_Init,
		gdeh0213b73_WritePixel,
		gdeh0213b73_SetDisplayWindow,
		gdeh0213b73_RefreshDisplay,
		gdeh0213b73_GetEpdPixelWidth,
		gdeh0213b73_GetEpdPixelHeight,
		gdeh0213b73_DrawImage,
		gdeh0213b73_DeepSleep,
		gdeh0213b73_PartialRefresh,
};*/

/**
* @}
*/

/** @defgroup gdeh0213b73_Private_FunctionPrototypes
  * @{
  */
/**
* @}
*/ 
typedef enum {
	FullUpdate,
	PartialUpdate,
	Sleeping
} epd_state;

epd_state _state;

/** @defgroup gdeh0213b73_Private_Functions
  * @{
  */

void EPD_IO_Init(void) {

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOB, ePD1_PWR_ENn_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

}

void EPD_IO_WriteData(uint8_t RegValue) {

	  HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
	  /* Set EPD data/command line DC to High */
	  HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_SET);

	  /* Send Data */
	  HAL_SPI_Transmit(&hspi1, &RegValue, 1, HAL_MAX_DELAY);

	  HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);

}

void EPD_IO_WriteCommand(uint8_t Reg) {

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);

	/* Set EPD data/command line DC to Low */
	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET);

	  /* Send Command */
	HAL_SPI_Transmit(&hspi1, &Reg, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
}

void EPD_WaitBusy() {
	while (HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) == GPIO_PIN_SET)
		HAL_Delay(1);
}

void gdeh2013b73_SelectLUT(const uint8_t *data) {
	EPD_IO_WriteCommand(0x32);
	for (uint8_t i=0; i<100; ++i)
		EPD_IO_WriteData(data[i]);
	EPD_WaitBusy();
}
/**
  * @brief  Initialize the gdeh0213b73 EPD Component.
  * @param  None
  * @retval None
  */
void gdeh0213b73_Init(void)
{ 

  /* Initialize the gdeh0213b73 */
  EPD_IO_Init();
  _state=FullUpdate;

  EPD_WaitBusy();
  EPD_IO_WriteCommand(0x12); // soft reset
  EPD_WaitBusy();

  EPD_IO_WriteCommand(0x74); //set analog block control
  EPD_IO_WriteData(0x54);
  EPD_IO_WriteCommand(0x7E); //set digital block control
  EPD_IO_WriteData(0x3B);

  EPD_IO_WriteCommand(0x01); //Driver output control
  EPD_IO_WriteData(0xF9);
  EPD_IO_WriteData(0x00);
  EPD_IO_WriteData(0x00);

  EPD_IO_WriteCommand(0x11); //data entry mode
  EPD_IO_WriteData(0x03);

  EPD_IO_WriteCommand(0x44); //set Ram-X address start/end position
  EPD_IO_WriteData(0x00);
  EPD_IO_WriteData(0x0F);    //0x0C-->(15+1)*8=128

  EPD_IO_WriteCommand(0x45); //set Ram-Y address start/end position
  EPD_IO_WriteData(0xF9);   //0xF9-->(249+1)=250
  EPD_IO_WriteData(0x00);
  EPD_IO_WriteData(0x00);
  EPD_IO_WriteData(0x00);
  EPD_WaitBusy();

  EPD_IO_WriteCommand(0x3C); //BorderWavefrom
  EPD_IO_WriteData(0x03);

  EPD_IO_WriteCommand(0x2C); //VCOM Voltage
  EPD_IO_WriteData(0x50);    //

  EPD_IO_WriteCommand(0x03); //Gate Driving voltage Control
  EPD_IO_WriteData(0x15);    // 19V

  EPD_IO_WriteCommand(0x04); //Source Driving voltage Control
  EPD_IO_WriteData(0x41);    // VSH1 15V
  EPD_IO_WriteData(0xA8);    // VSH2 5V
  EPD_IO_WriteData(0x32);    // VSL -15V

  EPD_IO_WriteCommand(0x3A); //Dummy Line
  EPD_IO_WriteData(0x2C);

  EPD_IO_WriteCommand(0x3B); //Gate time
  EPD_IO_WriteData(0x0B);

  gdeh2013b73_SelectLUT(LUT_full);

  EPD_IO_WriteCommand(0x4E);
  EPD_IO_WriteData(0x00);
    /* Set the width counter */
  EPD_IO_WriteCommand(0x4F);
  EPD_IO_WriteData(0xF9);
  EPD_IO_WriteData(0x00);
  EPD_WaitBusy();

}

void gdeh0213b73_partInit()
{
	EPD_IO_WriteCommand(0x2C);     //VCOM Voltage
	EPD_IO_WriteData(0x26);

	EPD_WaitBusy();
	gdeh2013b73_SelectLUT(( unsigned char *)LUT_part);
	EPD_IO_WriteCommand(0x37);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x40);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);

	EPD_IO_WriteCommand(0x22);
	EPD_IO_WriteData(0xC0);
	EPD_IO_WriteCommand(0x20);
	_state = PartialUpdate;
	EPD_WaitBusy();
}

void gdeh0213b73_fullInit()
{
	EPD_IO_WriteCommand(0x2C); //VCOM Voltage
	EPD_IO_WriteData(0x50);    //

	EPD_WaitBusy();
	gdeh2013b73_SelectLUT(( unsigned char *)LUT_full);
	EPD_IO_WriteCommand(0x37);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	EPD_IO_WriteData(0x00);
	_state = FullUpdate;

	//This group might not be needed
	EPD_IO_WriteCommand(0x22);
	EPD_IO_WriteData(0xC7);
	EPD_IO_WriteCommand(0x20);
	EPD_WaitBusy();
}

/**
  * @brief  Writes 4 dots.
  * @param  HEX_Code: specifies the Data to write.
  * @retval None
  */
void gdeh0213b73_WritePixel(uint8_t HEX_Code)
{
  if (_state == Sleeping)
    gdeh0213b73_Init();
  /* Prepare the register to write data on the RAM */
  EPD_IO_WriteCommand(0x24);

  /* Send the data to write */
  EPD_IO_WriteData(HEX_Code);
}

/**
  * @brief  Sets a display window.
  * @param  Xpos: specifies the X bottom left position.
  * @param  Ypos: specifies the Y bottom left position.
  * @param  Width: display window width.
  * @param  Height: display window height.
  * @retval None
*/
void gdeh0213b73_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  if (_state == Sleeping)
	gdeh0213b73_Init();
  //Xpos = gdeh0213b73_GetEpdPixelWidth() - Xpos -1;
  //Ypos = gdeh0213b73_GetEpdPixelHeight() - Ypos - 1;
  uint16_t Xend = Xpos + Width-1;
  uint16_t Yend = Ypos + Height -1;
  /* Set X position and the height */
  EPD_IO_WriteCommand(0x44);
  EPD_IO_WriteData(Ypos/8);
  EPD_IO_WriteData(Yend/8);
  /* Set Y position and the width */
  EPD_IO_WriteCommand(0x45);
  EPD_IO_WriteData(Xpos);
  EPD_IO_WriteData(0x00);
  EPD_IO_WriteData(Xend);
  EPD_IO_WriteData(0x00);
  /* Set the height counter */
  EPD_IO_WriteCommand(0x4E);
  EPD_IO_WriteData(Ypos/8);
  /* Set the width counter */
  EPD_IO_WriteCommand(0x4F);
  EPD_IO_WriteData(Xpos);
  EPD_IO_WriteData(0x00);

  EPD_WaitBusy();
}

/**
  * @brief  Gets the EPD pixel Width.
  * @param  None
  * @retval The EPD Pixel Width
  */
uint16_t gdeh0213b73_GetEpdPixelWidth(void)
{
  return GDEH0213B73_EPD_PIXEL_WIDTH;
}

/**
  * @brief  Gets the EPD pixel Height.
  * @param  None
  * @retval The EPD Pixel Height
  */
uint16_t gdeh0213b73_GetEpdPixelHeight(void)
{
  return GDEH0213B73_EPD_PIXEL_HEIGHT;
}

/**
  * @brief  Writes to the selected EPD register.
  * @param  EPD_Reg: Address of the selected register.
  * @param  EPD_RegValue: value to write to the selected register.
  * @retval None
  */
void gdeh0213b73_WriteCommand(uint8_t EPD_Reg, uint8_t EPD_RegValue)
{
  EPD_IO_WriteCommand(EPD_Reg);
  
  EPD_IO_WriteData(EPD_RegValue);
}


/**
  * @brief  Activates display update sequence.
  * @param  None
  * @retval None
  */
void gdeh0213b73_RefreshDisplay(void)
{
  if (_state == Sleeping)
    gdeh0213b73_Init();
  else if (_state == PartialUpdate)
	gdeh0213b73_fullInit();
  /* Write on the Display update control register */
  EPD_IO_WriteCommand(0x22);
  
  /* Display update data sequence option */
  EPD_IO_WriteData(0xC7);

  /* Launching the update: Nothing should interrupt this sequence in order
     to avoid display corruption */
  EPD_IO_WriteCommand(0x20);
}


void gdeh0213b73_DeepSleep()
{
  EPD_WaitBusy();
  EPD_IO_WriteCommand(0x10);
  EPD_IO_WriteData(0x01);
  _state = Sleeping;
}

void gdeh0213b73_PartialRefresh(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize)
{
	if (_state==Sleeping)
		gdeh0213b73_Init();
    if (_state == FullUpdate) {
    	gdeh0213b73_partInit();
    }

	gdeh0213b73_SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);


	EPD_IO_WriteCommand(0x22);
	EPD_IO_WriteData(0x0C);
	EPD_IO_WriteCommand(0x20);
}

#define BIT(x,b) ((x>>b) &1)
/**
  * @brief  Displays picture..
  * @param  pdata: picture address.
  * @param  Xpos:  Image X position in the EPD
  * @param  Ypos:  Image Y position in the EPD
  * @param  Xsize: Image X size in the EPD
  * @note   Xsize have to be a multiple of 8
  * @param  Ysize: Image Y size in the EPD
  * @retval None
  */
void gdeh0213b73_DrawImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, const uint8_t *pdata)
{

  gdeh0213b73_SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);
  
  /* Prepare the register to write data on the RAM */
  EPD_IO_WriteCommand(0x24);

  /*for (uint8_t xt=0; xt< Xsize; ++xt) {
	  uint8_t xb = (xt%8);
	  for (uint8_t yt=0; yt<Ysize; yt+=8) {
		  uint8_t d=0;
		  for (uint8_t i=0; i<8; ++i) {
			  uint8_t b = BIT(pdata[(yt+i)*(Xsize/8) + xt/8], xb);
			  d |= b << (7-i);
		  }
		  EPD_IO_WriteData(~d);
	  }
  }*/

   uint32_t total = Xsize/8*Ysize;

   for (uint16_t i=0; i< total; ++i) {
	  uint8_t s,d;
	  s = ~pdata[i];
	  d = 0;
	  for (uint8_t j=0; j<8; ++j) {
		  if ((s>>j) & 1)
			  d |= (1 << (7-j));
	  }
	  EPD_IO_WriteData(d);
  }
}

/**
  * @}
  */ 

/**
  * @}
  */  

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
