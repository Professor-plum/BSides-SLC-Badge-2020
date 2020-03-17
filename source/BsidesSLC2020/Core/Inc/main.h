/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define WAKETIME	100
#define USB_POWER	1
#define USART_OPEN	2
#define DEBOUNCE	2

#define VREFINT_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF80078))

#define COMMAND_MAX_LENGTH 16

extern SPI_HandleTypeDef hspi1;
extern DAC_HandleTypeDef hdac;
extern RTC_HandleTypeDef hrtc;
extern uint8_t stay_awake;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

enum UsartState {
	IDLE,
	WIPE_CONFIRM,
	WIPE_PENDING,
	RESET_CONFIRM,
	SYSTEM_TEST,
	PROGRAM,
};

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BLUE_Pin GPIO_PIN_0
#define BLUE_GPIO_Port GPIOH
#define RED_Pin GPIO_PIN_1
#define RED_GPIO_Port GPIOH
#define RED_LED_Pin GPIO_PIN_1
#define RED_LED_GPIO_Port GPIOA
#define GRN_LED_Pin GPIO_PIN_2
#define GRN_LED_GPIO_Port GPIOA
#define BLU_LED_Pin GPIO_PIN_3
#define BLU_LED_GPIO_Port GPIOA
#define EPD_PWR_Pin GPIO_PIN_0
#define EPD_PWR_GPIO_Port GPIOB
#define EPD_BUSY_Pin GPIO_PIN_1
#define EPD_BUSY_GPIO_Port GPIOB
#define EPD_RST_Pin GPIO_PIN_2
#define EPD_RST_GPIO_Port GPIOB
#define EPD_DC_Pin GPIO_PIN_10
#define EPD_DC_GPIO_Port GPIOB
#define EPD_CS_Pin GPIO_PIN_11
#define EPD_CS_GPIO_Port GPIOB
#define FLASH_CS_Pin GPIO_PIN_12
#define FLASH_CS_GPIO_Port GPIOB
#define VBUS_Pin GPIO_PIN_8
#define VBUS_GPIO_Port GPIOA
#define VBUS_EXTI_IRQn EXTI4_15_IRQn
#define MEMS_INT_Pin GPIO_PIN_15
#define MEMS_INT_GPIO_Port GPIOA
#define MEMS_INT_EXTI_IRQn EXTI4_15_IRQn
#define BTN_DN_Pin GPIO_PIN_3
#define BTN_DN_GPIO_Port GPIOB
#define BTN_DN_EXTI_IRQn EXTI2_3_IRQn
#define BTN_Pin GPIO_PIN_4
#define BTN_GPIO_Port GPIOB
#define BTN_EXTI_IRQn EXTI4_15_IRQn
#define BTN_UP_Pin GPIO_PIN_5
#define BTN_UP_GPIO_Port GPIOB
#define BTN_UP_EXTI_IRQn EXTI4_15_IRQn
#define NFC_PWR_Pin GPIO_PIN_8
#define NFC_PWR_GPIO_Port GPIOB
#define NFC_INT_Pin GPIO_PIN_9
#define NFC_INT_GPIO_Port GPIOB
#define NFC_INT_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
