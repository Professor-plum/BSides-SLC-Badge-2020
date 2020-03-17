/**
  ******************************************************************************
  * @file           : usbd_comp_if.c
  * @author  				: TOMAS
  * @version 				: V2.4.0t
  * @date    				: 6-June-2015
  * @brief          :
  ******************************************************************************
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_comp_if.h"
#include "usbd_comp.h"
#include "fakeDisk.h"
#include "w25qxx.h"
#include "console.h"
#include "flags.h"
#include "game.h"
/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CDC
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_CDC_Private_TypesDefinitions
  * @{
  */
  /* USER CODE BEGIN 0 */
  /* USER CODE END 0 */
/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Defines
  * @{
  */
  /* USER CODE BEGIN 1 */
/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  512
#define APP_TX_DATA_SIZE  512

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x800
#define STORAGE_BLK_SIZ                  0x200
  /* USER CODE END 1 */
/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Macros
  * @{
  */
  /* USER CODE BEGIN 2 */
char Rx_Buffer[256];
volatile uint8_t rxidx=0;
  /* USER CODE END 2 */
/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Variables
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/* Received Data over USB are stored in this buffer       */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/* Send Data over USB CDC are stored in this buffer       */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];
/* USB handler declaration */
/* Handle for USB Full Speed IP */
USBD_HandleTypeDef  *hUsbDevice_0;

extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t g_writable=false;


USBD_CDC_LineCodingTypeDef LineCoding = {
	115200,
	/* baud rate */
	0x00,
	/* stop bits-1 */
	0x00,
	/* parity - none */
	0x08    /* nb. of bits 8 */
};
/**
  * @}
  */

//extern uint8_t Tx_Buffer[2048];

/** @defgroup USBD_CDC_Private_FunctionPrototypes
  * @{
  */
#if USE_CDC
static int8_t CDC_Init_FS     (void);
static int8_t CDC_DeInit_FS   (void);
static int8_t CDC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS  (uint8_t* pbuf, uint32_t *Len);
#endif
#if USE_MSC
static int8_t STORAGE_Init_FS (uint8_t lun);
static int8_t STORAGE_GetCapacity_FS (uint8_t lun,
                           uint32_t *block_num,
                           uint16_t *block_size);
static int8_t  STORAGE_IsReady_FS (uint8_t lun);
static int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun);
static int8_t STORAGE_Read_FS (uint8_t lun,
                        uint8_t *buf,
                        uint32_t blk_addr,
                        uint16_t blk_len);
static int8_t STORAGE_Write_FS (uint8_t lun,
                        uint8_t *buf,
                        uint32_t blk_addr,
                        uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS (void);

const int8_t  STORAGE_Inquirydata_FS[] = {//36

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1',                     /* Version      : 4 Bytes */
};
#endif
USBD_COMP_ItfTypeDef USBD_Interface_fops_FS =
{
#if USE_CDC
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS,
#endif
#if USE_MSC
	STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS,
#endif
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  CDC_Init_FS
  *         Initializes the CDC media low layer over the FS USB IP
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
#if USE_CDC
static int8_t CDC_Init_FS(void)
{
 // hUsbDevice_0 = &hUsbDeviceFS;
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
	USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
	consoleConnect();
	puts("CDC Init");

	//CDC_Transmit_FS("Connected\r\n", 11);
	return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  CDC_DeInit_FS
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
puts("CDC deInit");
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  CDC_Control_FS
  *         Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
	printf("CDC control %d\n", cmd);
	/* USER CODE BEGIN 5 */
	switch (cmd)
	{
	case CDC_SEND_ENCAPSULATED_COMMAND:

		break;

	case CDC_GET_ENCAPSULATED_RESPONSE:

		break;

	case CDC_SET_COMM_FEATURE:

		break;

	case CDC_GET_COMM_FEATURE:

		break;

	case CDC_CLEAR_COMM_FEATURE:

		break;

		/*******************************************************************************/
		/* Line Coding Structure                                                       */
		/*-----------------------------------------------------------------------------*/
		/* Offset | Field       | Size | Value  | Description                          */
		/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
		/* 4      | bCharFormat |   1  | Number | Stop bits                            */
		/*                                        0 - 1 Stop bit                       */
		/*                                        1 - 1.5 Stop bits                    */
		/*                                        2 - 2 Stop bits                      */
		/* 5      | bParityType |  1   | Number | Parity                               */
		/*                                        0 - None                             */
		/*                                        1 - Odd                              */
		/*                                        2 - Even                             */
		/*                                        3 - Mark                             */
		/*                                        4 - Space                            */
		/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
		/*******************************************************************************/
	case CDC_SET_LINE_CODING:
		LineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | \
								 (pbuf[2] << 16) | (pbuf[3] << 24));
		LineCoding.format     = pbuf[4];
		LineCoding.paritytype = pbuf[5];
		LineCoding.datatype   = pbuf[6];
		printf("Line coding %d, %d, %d, %d (%d)\n", LineCoding.bitrate, LineCoding.format, LineCoding.paritytype, LineCoding.datatype, length);
		break;

	case CDC_GET_LINE_CODING:
		pbuf[0] = (uint8_t)(LineCoding.bitrate);
		pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
		pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
		pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
		pbuf[4] = LineCoding.format;
		pbuf[5] = LineCoding.paritytype;
		pbuf[6] = LineCoding.datatype;
		puts("Get Line coding");
		break;

	case CDC_SET_CONTROL_LINE_STATE:

		break;

	case CDC_SEND_BREAK:

		break;

	default:
		break;
	}

	return (USBD_OK);
	/* USER CODE END 5 */
}


/**
  * @brief  CDC_Receive_FS
  *         Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         until exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS (uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
	//uint8_t length = MIN(Len[0],APP_TX_DATA_SIZE);

	/* USER CODE BEGIN 6 */
	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
	USBD_CDC_ReceivePacket(&hUsbDeviceFS);
	uint16_t m = 256-rxidx;
	if (*Len < m)
		m = *Len;
	memcpy(Rx_Buffer + rxidx, Buf, m);
	rxidx += m;
	//CDC_Transmit_FS("*", 1);
	//CDC_Transmit_FS(Buf, *Len);
	return (USBD_OK);
	/* USER CODE END 6 */
}

void CDC_Process() {
	if (rxidx) {
		char Buf[256];
		uint16_t Len=rxidx;
		memcpy(Buf, Rx_Buffer, Len);
		rxidx=0;
		for (int i=0; i< Len; ++i)
			consoleChar(Buf[i]);
	}
}

/**
  * @brief  CDC_Transmit_FS
  *         Data send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be send
  * @param  Len: Number of data to be send (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
	uint8_t result = USBD_OK;
	/* USER CODE BEGIN 7 */
	USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
	if (hcdc->TxState != 0) {
		return USBD_BUSY;
	}
	USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
	result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
	/* USER CODE END 7 */
	return result;
}

#endif


#if USE_MSC
static void ReadRootDir(uint8_t* buf);
/*******************************************************************************
* Function Name  : STORAGE_Init_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Init_FS (uint8_t lun)
{
  /* USER CODE BEGIN 2 */
	puts("Storage Init");
  return (USBD_OK);
  /* USER CODE END 2 */
}

/*******************************************************************************
* Function Name  : STORAGE_GetCapacity_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
  *block_num  = STORAGE_BLK_NBR;
  *block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
  /* USER CODE END 3 */
}

/*******************************************************************************
* Function Name  : STORAGE_IsReady_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsReady_FS (uint8_t lun)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/*******************************************************************************
* Function Name  : STORAGE_IsWriteProtected_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/*******************************************************************************
* Function Name  : STORAGE_Read_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Read_FS (uint8_t lun,
                        uint8_t *buf,
                        uint32_t blk_addr,
                        uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */
	if (blk_len !=1 ) {
		printf("Bad size! %d\n", blk_len);
		return USBD_FAIL;
	}
	if (blk_addr == 3) {
		if (W25qxx_IsBusy())
			return USBD_BUSY;
		ReadRootDir(buf);
	}
	else if (blk_addr < 0x200) {
		if (W25qxx_IsBusy())
			return USBD_BUSY;
		W25qxx_ReadBytes(buf, blk_addr* STORAGE_BLK_SIZ, STORAGE_BLK_SIZ);
	}
	else
		memset(buf, 0, blk_len*STORAGE_BLK_SIZ);
	return (USBD_OK);
  /* USER CODE END 6 */
}

/*******************************************************************************
* Function Name  : STORAGE_Write_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Write_FS (uint8_t lun,
                         uint8_t *buf,
                         uint32_t blk_addr,
                         uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */
	if (g_writable ) {
			//printf("Writing block 0x%04x\n", blk_addr* STORAGE_BLK_SIZ);
			uint32_t addr = blk_addr * STORAGE_BLK_SIZ / w25qxx.PageSize;
			W25qxx_WritePage(buf, addr, 0, w25qxx.PageSize);
			W25qxx_WritePage(buf+w25qxx.PageSize, addr + 1, 0, w25qxx.PageSize);
		}
	return (USBD_OK);
  /* USER CODE END 7 */
}

/*******************************************************************************
* Function Name  : STORAGE_GetMaxLun_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetMaxLun_FS (void)
{
  /* USER CODE BEGIN 8 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */
}

static void ReadRootDir(uint8_t* buf) {
	memset(buf, 0, STORAGE_BLK_SIZ);
	memcpy(buf, fake_root, 32);
	int i=1, j=1;
	struct file_entry tf;
	while (true) {
		W25qxx_ReadBytes(&tf, 0x600 + (j++*sizeof(tf)), sizeof(tf));
		if ((tf.filename[0] == 0) || (j>= (STORAGE_BLK_SIZ/sizeof(struct file_entry))))
			return;

		if (checkFlag(FLAG_CAGE) && (!checkFlag(FLAG_VIRUS))) {
			tf.filename[0] = 0xe5;
		}
		else {
			if ((strncmp(tf.filename, "PH4N70M EML", 11)==0) && (!checkFlag(FLAG_EMAIL)))
				continue;
			else if ((strncmp(tf.filename, "PUZZLE  TXT", 11)==0) && (!checkFlag(FLAG_PUZZLE)))
				continue;
			else if ((strncmp(tf.filename, "RTTY    OGG", 11)==0) && (!(checkFlag(FLAG_BATT_IN) || checkFlag(FLAG_ANTENNA))))
				continue;
			else if ((strncmp(tf.filename, "SAMPLE  CAP", 11)==0) && (!checkFlag(FLAG_PCAP)))
				continue;
			if ((strncmp(tf.filename, "BLCKMAILRTF", 11)==0) && (!checkFlag(FLAG_BOX_OPEN)))
				continue;
			if ((strncmp(tf.filename, "CHESS   JPG", 11)==0) && (!checkFlag(FLAG_STEGO)))
				continue;
		}
		if (strncmp(tf.filename, "RECOVER PDF", 11)==0)
			tf.filename[0] = 0xe5;
		memcpy(&buf[sizeof(struct file_entry)*(i++)], &tf, sizeof(struct file_entry));
	}
	/*memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[0], sizeof(struct file_entry));
	if (checkFlag(FLAG_EMAIL))
		memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[1], sizeof(struct file_entry));
	if (checkFlag(FLAG_PUZZLE))
		memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[2], sizeof(struct file_entry));
	if (checkFlag(FLAG_BATT_IN) && checkFlag(FLAG_ANTENNA))
		memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[3], sizeof(struct file_entry));
	if (checkFlag(FLAG_PCAP))
		memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[4], sizeof(struct file_entry));
	if (checkFlag(FLAG_BOX_OPEN))
		memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[5], sizeof(struct file_entry));
	if (checkFlag(FLAG_STEGO))
		memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[6], sizeof(struct file_entry));
	memcpy(&buf[sizeof(struct file_entry)*(i++)], &fake_files[7], sizeof(struct file_entry));*/
}

void setWritable(uint8_t enabled) {
	g_writable=enabled;
}

#endif

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

