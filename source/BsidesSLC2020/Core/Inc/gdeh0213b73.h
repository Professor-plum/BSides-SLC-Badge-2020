/**
  ******************************************************************************
  * @file    gdeh0213b73.h
  * @author  Waylon Grange
  * @version V1.0.1
  * @date    21-July-2014
  * @brief   This file contains all the functions prototypes for the ePaper
  *          driver.
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GDEH0213B73_H
#define __GDEH0213B73_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "epd.h"
#include "main.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup GDEH0213B73
  * @{
  */

/** @defgroup GDEH0213B73_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup GDEH0213B73_Exported_Constants
  * @{
  */

/**
  * @brief  GDEH0213B73 Size
  */
#define  GDEH0213B73_EPD_PIXEL_WIDTH    ((uint16_t)128)
#define  GDEH0213B73_EPD_PIXEL_HEIGHT   ((uint16_t)250)

/**
  * @}
  */

/** @defgroup SPI_EPD_Exported_Functions
  * @{
  */
void     gdeh0213b73_Init(void);
void     gdeh0213b73_WriteReg(uint8_t EPD_Reg, uint8_t EPD_RegValue);

void     gdeh0213b73_WritePixel(uint8_t HEX_Code);

void     gdeh0213b73_DrawImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, const uint8_t *pdata);
void     gdeh0213b73_RefreshDisplay(void);

void     gdeh0213b73_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
uint16_t gdeh0213b73_GetEpdPixelWidth(void);
uint16_t gdeh0213b73_GetEpdPixelHeight(void);

void     gdeh0213b73_DeepSleep(void);
void     gdeh0213b73_PartialRefresh(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize);


void 	 gdeh0213b73_EnablePartial(uint8_t partial);
/* EPD driver structure */
//extern EPD_DrvTypeDef   gdeh0213b73_drv;

/* EPD IO functions */
void      EPD_IO_Init(void);
void      EPD_IO_WriteData(uint8_t RegValue);
void      EPD_IO_WriteCommand(uint8_t Reg);
void  	EPD_WaitBusy();
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __GDEH0213B73_H */

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
