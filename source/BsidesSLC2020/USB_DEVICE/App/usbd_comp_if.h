/**
  ******************************************************************************
  * @file    usbd_cdc_if_template.h
  * @author  MCD Application Team
  * @version V2.4.0
  * @date    28-February-2015
  * @brief   Header for usbd_cdc_if_template.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_COMP_IF_H
#define __USBD_COMP_IF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"
#include "usbd_comp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern USBD_COMP_ItfTypeDef  USBD_Interface_fops_FS;
extern USBD_CDC_ItfTypeDef  USBD_CDC_Template_fops;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
void setWritable(uint8_t enabled);
void CDC_Process();

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_IF_TEMPLATE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
