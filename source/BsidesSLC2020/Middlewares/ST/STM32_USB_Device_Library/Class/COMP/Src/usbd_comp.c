/**
  ******************************************************************************
  * @file    usbd_comp.c
  * @author  MCD Application Team
  * @version V2.4.0t
  * @date    28-February-2015
  * @brief   This file provides the high layer firmware functions to manage the 
  *          following functionalities of the USB Composite:
  *           
  *           
  *                                  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "USBD_COMP.h"
#include "USBD_CDC.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_CDC 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_CDC_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_CDC_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_CDC_Private_Macros
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_CDC_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_COMP_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx);
static uint8_t  USBD_COMP_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx);
static uint8_t  USBD_COMP_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req);
static uint8_t  USBD_COMP_DataIn (USBD_HandleTypeDef *pdev, 
                                 uint8_t epnum);
static uint8_t  USBD_COMP_DataOut (USBD_HandleTypeDef *pdev, 
                                 uint8_t epnum);
static uint8_t  USBD_COMP_EP0_RxReady (USBD_HandleTypeDef *pdev);
static uint8_t  *USBD_COMP_GetFSCfgDesc (uint16_t *length);

uint8_t  *USBD_COMP_GetDeviceQualifierDescriptor (uint16_t *length);

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_COMP_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */ 

/** @defgroup USBD_CDC_Private_Variables
  * @{
  */ 


/* CDC interface class callbacks structure */
USBD_ClassTypeDef  USBD_COMP = 
{
  USBD_COMP_Init,
  USBD_COMP_DeInit,
  USBD_COMP_Setup,
  NULL,                 /* EP0_TxSent, */
  USBD_COMP_EP0_RxReady,
  USBD_COMP_DataIn,
  USBD_COMP_DataOut,
  NULL,
  NULL,
  NULL,     
  NULL,  
  USBD_COMP_GetFSCfgDesc,    
  NULL, 
  USBD_COMP_GetDeviceQualifierDescriptor,
};


/* USB COMP device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_COMP_CfgFSDesc[USB_COMP_CONFIG_DESC_SIZ] __ALIGN_END =
{
/*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  USB_COMP_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
  0x00,
  BNUMINTERFACES,   /* bNumInterfaces: 2 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */
#if USE_CDC	   
  /*---------------------------------------------------------------------------*/
	0x08,		/* BYTE  bLength      0x08 */
	0x0B,		/* BYTE  bDescriptorType    0x0B */
	CDC_INTERFACE1,		/* BYTE  bFirstInterface    0x00 */
	0x02,		/* BYTE  bInterfaceCount    0x02 */
	0x02,		/* BYTE  bFunctionClass    0x0E */
	0x02,		/* BYTE  bFunctionSubClass   0x03 */
	0x01,		/* BYTE  bFunctionProtocol   0x00 */
	0x00,		/* BYTE  iFunction      0x04 */
  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  CDC_INTERFACE1,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x01,   /* bNumEndpoints: One endpoints used */
  0x02,   /* bInterfaceClass: Communication Interface Class */
  0x02,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: */
  
  /*Header Functional Descriptor*/
  0x05,   /* bLength: Endpoint Descriptor size */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x00,   /* bDescriptorSubtype: Header Func Desc */
  0x10,   /* bcdCDC: spec release number */
  0x01,
  
  /*Call Management Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x01,   /* bDescriptorSubtype: Call Management Func Desc */
  0x00,   /* bmCapabilities: D0+D1 */
  0x01,   /* bDataInterface: 1 */
  
  /*ACM Functional Descriptor*/
  0x04,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,   /* bmCapabilities */
  
  /*Union Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x06,   /* bDescriptorSubtype: Union func desc */
  0x00,   /* bMasterInterface: Communication class interface */
  0x01,   /* bSlaveInterface0: Data Class Interface */
  
  /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  CDC_EPCMD_ADDR,                     /* bEndpointAddress */
  0x03,                           /* bmAttributes: Interrupt */
  LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
  HIBYTE(CDC_CMD_PACKET_SIZE),
  0x10,                           /* bInterval: */ 
  /*---------------------------------------------------------------------------*/
 
  /*Data class interface descriptor*/
  0x09,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
  CDC_INTERFACE2,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints: Two endpoints used */
  0x0A,   /* bInterfaceClass: CDC */
  0x00,   /* bInterfaceSubClass: */
  0x00,   /* bInterfaceProtocol: */
  0x00,   /* iInterface: */
  
  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  CDC_EPOUT_ADDR,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00,                              /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  CDC_EPIN_ADDR,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00 ,                              /* bInterval: ignore for Bulk transfer */
	
#endif
#if USE_HID	
	/************** Descriptor of Joystick Mouse interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  HID_INTERFACE,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x01,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Mouse endpoint ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
  
  HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_EPIN_SIZE, /*wMaxPacketSize: 4 Byte max */
  0x00,
  HID_FS_BINTERVAL,          /*bInterval: Polling Interval (10 ms)*/
#endif
#if USE_MSC
/********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  MSC_INTERFACE,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0x08,   /* bInterfaceClass: MSC Class */
  0x06,   /* bInterfaceSubClass : SCSI transparent*/
  0x50,   /* nInterfaceProtocol */
  0x00,          /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  MSC_EPIN_ADDR,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_FS_PACKET),
  HIBYTE(MSC_MAX_FS_PACKET),
  0x00,   /*Polling interval in milliseconds */
  
  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  MSC_EPOUT_ADDR,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_FS_PACKET),
  HIBYTE(MSC_MAX_FS_PACKET),
  0x00     /*Polling interval in milliseconds*/
#endif
} ;

#if USE_HID
	__ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE]  __ALIGN_END =
{
  0x05,   0x01,
  0x09,   0x02,
  0xA1,   0x01,
  0x09,   0x01,
  
  0xA1,   0x00,
  0x05,   0x09,
  0x19,   0x01,
  0x29,   0x03,
  
  0x15,   0x00,
  0x25,   0x01,
  0x95,   0x03,
  0x75,   0x01,
  
  0x81,   0x02,
  0x95,   0x01,
  0x75,   0x05,
  0x81,   0x01,
  
  0x05,   0x01,
  0x09,   0x30,
  0x09,   0x31,
  0x09,   0x38,
  
  0x15,   0x81,
  0x25,   0x7F,
  0x75,   0x08,
  0x95,   0x03,
  
  0x81,   0x06,
  0xC0,   0x09,
  0x3c,   0x05,
  0xff,   0x09,
  
  0x01,   0x15,
  0x00,   0x25,
  0x01,   0x75,
  0x01,   0x95,
  
  0x02,   0xb1,
  0x22,   0x75,
  0x06,   0x95,
  0x01,   0xb1,
  
  0x01,   0xc0
};

__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ]  __ALIGN_END  =
{
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};

#endif

/**
  * @}
  */ 

/** @defgroup USBD_CDC_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_COMP_Init
  *         Initialize the Composite interfaces
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_COMP_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx)
{
  uint8_t ret = 0;

#if USE_CDC  
	USBD_COMP_HandleTypeDef   *hcomp;
  if(pdev->dev_speed == USBD_SPEED_HIGH  ) 
  {  
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   CDC_EPIN_ADDR,
                   USBD_EP_TYPE_BULK,
                   CDC_DATA_HS_IN_PACKET_SIZE);
    
    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   CDC_EPOUT_ADDR,
                   USBD_EP_TYPE_BULK,
                   CDC_DATA_HS_OUT_PACKET_SIZE);
    
  }
  else
  {
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   CDC_EPIN_ADDR,
                   USBD_EP_TYPE_BULK,
                   CDC_DATA_FS_IN_PACKET_SIZE);
    
    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   CDC_EPOUT_ADDR,
                   USBD_EP_TYPE_BULK,
                   CDC_DATA_FS_OUT_PACKET_SIZE);
  }
  /* Open Command IN EP */
  USBD_LL_OpenEP(pdev,
                 CDC_EPCMD_ADDR,
                 USBD_EP_TYPE_INTR,
                 CDC_CMD_PACKET_SIZE);
#endif
#if USE_HID
	  USBD_LL_OpenEP(pdev,
                 HID_EPIN_ADDR,
                 USBD_EP_TYPE_INTR,
                 HID_EPIN_SIZE); 
#endif  
#if USE_MSC
	if(pdev->dev_speed == USBD_SPEED_HIGH  ) 
  {
    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   MSC_EPOUT_ADDR,
                   USBD_EP_TYPE_BULK,
                   MSC_MAX_HS_PACKET);
    
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   MSC_EPIN_ADDR,
                   USBD_EP_TYPE_BULK,
                   MSC_MAX_HS_PACKET);  
  }
  else
  {
    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   MSC_EPOUT_ADDR,
                   USBD_EP_TYPE_BULK,
                   MSC_MAX_FS_PACKET);
    
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   MSC_EPIN_ADDR,
                   USBD_EP_TYPE_BULK,
                   MSC_MAX_FS_PACKET);  
  }
#endif	

  pdev->pClassData = USBD_malloc(sizeof (USBD_COMP_HandleTypeDef));
  
  if(pdev->pClassData == NULL)
  {
    ret = 1; 
  }
  else
  {
#if USE_CDC
    hcomp = (USBD_COMP_HandleTypeDef*) pdev->pClassData;
    
    /* Init  physical Interface components */
    ((USBD_COMP_ItfTypeDef *)pdev->pUserData)->Init();
    
    /* Init Xfer states */
    hcomp->TxState =0;
    hcomp->RxState =0;
       
    if(pdev->dev_speed == USBD_SPEED_HIGH  ) 
    {      
      /* Prepare Out endpoint to receive next packet */
      USBD_LL_PrepareReceive(pdev,
                             CDC_EPOUT_ADDR,
                             hcomp->RxBuffer,
                             CDC_DATA_HS_OUT_PACKET_SIZE);
    }
    else
    {
      /* Prepare Out endpoint to receive next packet */
      USBD_LL_PrepareReceive(pdev,
                             CDC_EPOUT_ADDR,
                             hcomp->RxBuffer,
                             CDC_DATA_FS_OUT_PACKET_SIZE);
    }
#endif   
#if USE_HID
		((USBD_COMP_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;
#endif   
#if USE_MSC
	  MSC_BOT_Init(pdev); 
    ret = 0;
#endif	
		
  }
  return ret;
}

/**
  * @brief  USBD_COMP_DeInit
  *         DeInitialize the Composite layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_COMP_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx)
{
  uint8_t ret = 0;
#if USE_CDC  
  /* Open EP IN */
  USBD_LL_CloseEP(pdev,
              CDC_EPIN_ADDR);
  
  /* Open EP OUT */
  USBD_LL_CloseEP(pdev,
              CDC_EPOUT_ADDR);
  
  /* Open Command IN EP */
  USBD_LL_CloseEP(pdev,
              CDC_EPCMD_ADDR);
#endif 
#if USE_HID
  USBD_LL_CloseEP(pdev,
                  HID_EPIN_ADDR);
#endif 
#if USE_MSC
	USBD_LL_CloseEP(pdev,
                  MSC_EPOUT_ADDR);
  
  /* Open EP IN */
  USBD_LL_CloseEP(pdev,
                  MSC_EPIN_ADDR);
  
  
    /* De-Init the BOT layer */
  MSC_BOT_DeInit(pdev);	
#endif   
  
  /* DeInit  physical Interface components */
  if(pdev->pClassData != NULL)
  {
#if USE_CDC 
    ((USBD_COMP_ItfTypeDef *)pdev->pUserData)->DeInit();
#endif 
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }

  return ret;
}

/**
  * @brief  USBD_COMP_Setup
  *         Handle the specific requests belonging to interfaces in COMP
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_COMP_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req)
{
  USBD_COMP_HandleTypeDef   *hcomp = (USBD_COMP_HandleTypeDef*) pdev->pClassData;
#if USE_CDC 
  static uint8_t ifalt = 0;
#endif
#if USE_HID
	 uint16_t len = 0;
	uint8_t  *pbuf = NULL;
#endif
  

	if(req->wIndex==CDC_INTERFACE1 || req->wIndex==CDC_INTERFACE2){	//interface 0 - CDC
#if USE_CDC 
		switch (req->bmRequest & USB_REQ_TYPE_MASK)
		{
		case USB_REQ_TYPE_CLASS :
			if (req->wLength)
			{
				if (req->bmRequest & 0x80)
				{
					((USBD_COMP_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest,(uint8_t *)hcomp->data,req->wLength);
						USBD_CtlSendData (pdev, (uint8_t *)hcomp->data,req->wLength);
				}
				else
				{
					hcomp->CmdOpCode = req->bRequest;
					hcomp->CmdLength = req->wLength;
					
					USBD_CtlPrepareRx (pdev, (uint8_t *)hcomp->data, req->wLength);
				}
				
			}
			else
			{
				((USBD_COMP_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest, (uint8_t*)req, 0);
			}
			break;

		case USB_REQ_TYPE_STANDARD:
			switch (req->bRequest)
			{      
			case USB_REQ_GET_INTERFACE :
				USBD_CtlSendData (pdev, &ifalt, 1);
				break;
				
			case USB_REQ_SET_INTERFACE :
				break;
			}
	 
		default: 
			break;
		}
#endif
	}
	else if(req->wIndex==HID_INTERFACE){	//interface 1 - HID
#if USE_HID
	  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    switch (req->bRequest)
    {
      
      
    case HID_REQ_SET_PROTOCOL:
      hcomp->Protocol = (uint8_t)(req->wValue);
      break;
      
    case HID_REQ_GET_PROTOCOL:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&hcomp->Protocol,
                        1);    
      break;
      
    case HID_REQ_SET_IDLE:
      hcomp->IdleState = (uint8_t)(req->wValue >> 8);
      break;
      
    case HID_REQ_GET_IDLE:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&hcomp->IdleState,
                        1);        
      break;      
      
    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL; 
    }
    break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
      if( req->wValue >> 8 == HID_REPORT_DESC)
      {
        len = MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
        pbuf = HID_MOUSE_ReportDesc;
      }
      else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
      {
        pbuf = USBD_HID_Desc;   
        len = MIN(USB_HID_DESC_SIZ , req->wLength);
      }
      
      USBD_CtlSendData (pdev, 
                        pbuf,
                        len);
      
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&hcomp->AltSetting,
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      hcomp->AltSetting = (uint8_t)(req->wValue);
      break;
    }
  }
#endif		
	}	else if(req->wIndex==MSC_INTERFACE){	//interface 1 - HID
#if USE_MSC
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {

  /* Class request */
  case USB_REQ_TYPE_CLASS :
    switch (req->bRequest)
    {
    case BOT_GET_MAX_LUN :

      if((req->wValue  == 0) && 
         (req->wLength == 1) &&
         ((req->bmRequest & 0x80) == 0x80))
      {
        hcomp->max_lun = ((USBD_COMP_ItfTypeDef *)pdev->pUserData)->GetMaxLun();
        USBD_CtlSendData (pdev, (uint8_t *)&hcomp->max_lun, 1);
      }
      else
      {
         USBD_CtlError(pdev , req);
         return USBD_FAIL; 
      }
      break;
      
    case BOT_RESET :
      if((req->wValue  == 0) && 
         (req->wLength == 0) &&
        ((req->bmRequest & 0x80) != 0x80))
      {      
         MSC_BOT_Reset(pdev);
      }
      else
      {
         USBD_CtlError(pdev , req);
         return USBD_FAIL; 
      }
      break;

    default:
       USBD_CtlError(pdev , req);
       return USBD_FAIL; 
    }
    break;
  /* Interface & Endpoint request */
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&hcomp->interface,
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      hcomp->interface = (uint8_t)(req->wValue);
      break;
    
    case USB_REQ_CLEAR_FEATURE:  
      
      /* Flush the FIFO and Clear the stall status */    
      USBD_LL_FlushEP(pdev, (uint8_t)req->wIndex);
      
      /* Reactivate the EP */      
      USBD_LL_CloseEP (pdev , (uint8_t)req->wIndex);
      if((((uint8_t)req->wIndex) & 0x80) == 0x80)
      {
        if(pdev->dev_speed == USBD_SPEED_HIGH  ) 
        {
          /* Open EP IN */
          USBD_LL_OpenEP(pdev,
                         MSC_EPIN_ADDR,
                         USBD_EP_TYPE_BULK,
                         MSC_MAX_HS_PACKET);  
        }
        else
        {   
          /* Open EP IN */
          USBD_LL_OpenEP(pdev,
                         MSC_EPIN_ADDR,
                         USBD_EP_TYPE_BULK,
                         MSC_MAX_FS_PACKET);  
        }
      }
      else
      {
        if(pdev->dev_speed == USBD_SPEED_HIGH  ) 
        {
          /* Open EP IN */
          USBD_LL_OpenEP(pdev,
                         MSC_EPOUT_ADDR,
                         USBD_EP_TYPE_BULK,
                         MSC_MAX_HS_PACKET);  
        }
        else
        {   
          /* Open EP IN */
          USBD_LL_OpenEP(pdev,
                         MSC_EPOUT_ADDR,
                         USBD_EP_TYPE_BULK,
                         MSC_MAX_FS_PACKET);  
        }
      }
      
      /* Handle BOT error */
      MSC_BOT_CplClrFeature(pdev, (uint8_t)req->wIndex);
      break;
      
    }  
    break;
   
  default:
    break;
  }
#endif
	}
  return USBD_OK;
}

/**
  * @brief  USBD_COMP_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  USBD_COMP_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
#if USE_CDC 
  USBD_COMP_HandleTypeDef   *hcomp = (USBD_COMP_HandleTypeDef*) pdev->pClassData;
#endif
	if(epnum==(HID_EPIN_ADDR&0x0F)){
#if USE_HID 
	((USBD_COMP_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;
#endif
	}else if(epnum==(CDC_EPIN_ADDR&0x0F) || epnum==(CDC_EPCMD_ADDR&0x0F)){
#if USE_CDC 
  if(pdev->pClassData != NULL)
  {
    
    hcomp->TxState = 0;

    return USBD_OK;
  }
  else
  {
    return USBD_FAIL;
  }
#endif
	}else if(epnum==(MSC_EPIN_ADDR&0x0F)){
#if USE_MSC
		MSC_BOT_DataIn(pdev , epnum);
#endif
	}
	return USBD_OK;
}

/**
  * @brief  USBD_COMP_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  USBD_COMP_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{      
  
	if(epnum==(CDC_EPOUT_ADDR&0x0F)){
#if USE_CDC 
		USBD_COMP_HandleTypeDef   *hcomp = (USBD_COMP_HandleTypeDef*) pdev->pClassData;
		/* Get the received data length */
		hcomp->RxLength = USBD_LL_GetRxDataSize (pdev, epnum);
		
		/* USB data will be immediately processed, this allow next USB traffic being 
		NAKed till the end of the application Xfer */
		if(pdev->pClassData != NULL)
		{
			((USBD_COMP_ItfTypeDef *)pdev->pUserData)->Receive(hcomp->RxBuffer, &hcomp->RxLength);
			 USBD_LL_PrepareReceive(pdev,
														 CDC_EPOUT_ADDR,
														 hcomp->RxBuffer,
														 CDC_DATA_FS_OUT_PACKET_SIZE);
			return USBD_OK;
		}
		else
		{
			return USBD_FAIL;
		}
#endif
	}else if(epnum==(MSC_EPOUT_ADDR & 0x0F)){
#if USE_MSC
		MSC_BOT_DataOut(pdev , epnum);
#endif
	}
	return USBD_OK;
}



/**
  * @brief  USBD_COMP_EP0_RxReady
  *         EP0 recieved data handler
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_COMP_EP0_RxReady (USBD_HandleTypeDef *pdev)
{ 
#if USE_CDC 
  USBD_COMP_HandleTypeDef   *hcomp = (USBD_COMP_HandleTypeDef*) pdev->pClassData;	
  if((pdev->pUserData != NULL) && (hcomp->CmdOpCode != 0xFF))
  {
    ((USBD_COMP_ItfTypeDef *)pdev->pUserData)->Control(hcomp->CmdOpCode,
                                                      (uint8_t *)hcomp->data,
                                                      hcomp->CmdLength);
      hcomp->CmdOpCode = 0xFF; 
      
  }
#endif
  return USBD_OK;
}

/**
  * @brief  USBD_CDC_GetFSCfgDesc 
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_COMP_GetFSCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_COMP_CfgFSDesc);
  return USBD_COMP_CfgFSDesc;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_COMP_GetDeviceQualifierDescriptor (uint16_t *length)
{
  *length = sizeof (USBD_COMP_DeviceQualifierDesc);
  return USBD_COMP_DeviceQualifierDesc;
}

/**
* @brief  USBD_CDC_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: CD  Interface callback
  * @retval status
  */
uint8_t  USBD_COMP_RegisterInterface  (USBD_HandleTypeDef   *pdev, 
                                      USBD_COMP_ItfTypeDef *fops)
{
  uint8_t  ret = USBD_FAIL;
  
  if(fops != NULL)
  {
    pdev->pUserData= fops;
    ret = USBD_OK;    
  }
  
  return ret;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
