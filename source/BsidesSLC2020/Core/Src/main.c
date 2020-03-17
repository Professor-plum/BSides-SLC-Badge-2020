/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mems.h"
#include "epd.h"
#include "w25qxx.h"
#include "lib_NDEF_Text.h"
#include "nfc.h"
#include "game.h"
#include "eeprom.h"
#include "usbd_comp_if.h"
#include "flags.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/*const uint8_t ledWav[] = {
		0x2,0xa,0x15,0x25,0x39,0x4f,0x67,0x80,0x98,0xb0,0xc6,0xda,0xea,0xf5,0xfd,0xff,0xfd,0xf5,0xea,0xda,0xc6,0xb0,0x98,0x80,0x67,0x4f,0x39,0x25,0x15,0xa,0x2,0x0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		};*/
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

DAC_HandleTypeDef hdac;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim21;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint16_t idle=0;
uint32_t tick=0;
uint8_t	led_show=0;
uint8_t led_idx=0;
uint8_t stay_awake=0;
uint32_t debounce=0;
uint8_t last_event=EVENT_NONE;
uint8_t uartbyte;
enum UsartState ustate = IDLE;

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM21_Init(void);
static void MX_ADC_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
//static void ledColor(uint8_t r, uint8_t g, uint8_t b);
static void setLED(bool r, bool g, bool b);
static void SystemPower_Config(void);
static void Sleep(void);
static void Standby(void);
static void ProgramFlash(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM21_Init();
  MX_ADC_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  //ledColor(0, 0, 255);
  SystemPower_Config();
  if (!MEMS_Init()){
	  puts("Mems Init failed!");
	  Error_Handler();
  }
  if (!NFC_Init()) {
	  puts("NFC Init failed!");
	  Error_Handler();
  }

  if (!W25qxx_Init()) {
	  puts("Flash Init failed!");
	  Error_Handler();
  }

  /*hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
	  Error_Handler();
  }*/

  GameInit();
  MX_USB_DEVICE_Init();

  uint8_t resume = __HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET;
  if (resume) {

      /* Clear Standby flag */
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

      //setLED(false, true, false);

      //ADC Must be enabled to set the calibration value
      __HAL_ADC_ENABLE(&hadc);
      uint32_t adc_cal = HAL_RTCEx_BKUPRead(&hrtc, ADC_CAL_REG);
      HAL_ADCEx_Calibration_SetValue(&hadc, ADC_SINGLE_ENDED, adc_cal);

      if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_WUTF) == SET) {
		  puts("RTC Wakeup");
		  Standby();
		  //We don't return from this
	  }

	  puts("Event Wakeup");

      //We woke up from a key press so pass that on to the game
      last_event = EVENT_KEY_SELECT;
  }
  else{
	  puts("Init");
	  //TODO: run a systems check (Validate Flash data)

	  HAL_ADCEx_Calibration_Start(&hadc,ADC_SINGLE_ENDED);
	  uint32_t adc_cal = HAL_ADCEx_Calibration_GetValue(&hadc, ADC_SINGLE_ENDED);
	  HAL_RTCEx_BKUPWrite(&hrtc, ADC_CAL_REG, adc_cal);

	  DrawLogo(true);
  }

  uint16_t bat = readVoltage();
  printf("Battery is at %d.%dV\n", bat/1000, bat%1000);

  HAL_TIM_Base_Start_IT(&htim21);
  //HAL_TIM_Base_Start_IT(&htim2);

  //HAL_DMA_Start(&hdma_tim6_up, (uint32_t)&ledPins,  (uint32_t)&GPIOC->ODR, 16);
  // _HAL_TIM_ENABLE_DMA(&htim6, TIM_DMA_UPDATE);
  // __HAL_TIM_ENABLE(&htim6);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  if (HAL_GPIO_ReadPin(VBUS_GPIO_Port, VBUS_Pin) == GPIO_PIN_SET)
  	  stay_awake |= USB_POWER;
  while (1)
  {
	  if (last_event != EVENT_NONE) {
		  uint8_t evt = last_event;
		  if (evt == EVENT_NFC) {
			  while (last_event != EVENT_NONE) {
				  last_event= EVENT_NONE;
				  HAL_Delay(100); // just to let the NFC write actually finish
			  }
		  }
		  last_event= EVENT_NONE;
		  GameEvent(evt);
	  }

	  if (ustate == WIPE_PENDING) {
		  puts("Erasing flash memory");
		  W25qxx_EraseChip();
		  puts("Erase complete");
		  ustate = IDLE;
	  }
	  else if (ustate == SYSTEM_TEST) {
		  //TODO: system test
		  puts("OK - system test complete");
		  ustate = IDLE;
	  }
	  else if (ustate == PROGRAM) {
		  stay_awake=1;
		  ProgramFlash();
		  ustate = IDLE;
	  }
	  CDC_Process();
	  GameTick();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (stay_awake==0)
  	  	  Sleep();
	  else
		  HAL_Delay(10);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization 
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config 
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
	hrtc.Instance = RTC;

	//if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) == RESET) { //Only reset time if reset
	  if (HAL_RTCEx_BKUPRead(&hrtc, CLOCK_SET) != 0x12345678) {

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 10;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
  sDate.Month = RTC_MONTH_MARCH;
  sDate.Date = 20;
  sDate.Year = 20;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  HAL_RTCEx_BKUPWrite(&hrtc, CLOCK_SET, 0x12345678);
  }

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM21 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM21_Init(void)
{

  /* USER CODE BEGIN TIM21_Init 0 */

  /* USER CODE END TIM21_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM21_Init 1 */

  /* USER CODE END TIM21_Init 1 */
  htim21.Instance = TIM21;
  htim21.Init.Prescaler = 3200-1;
  htim21.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim21.Init.Period = 1000-1;
  htim21.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim21.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim21) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim21, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim21, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM21_Init 2 */

  /* USER CODE END TIM21_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* Receive one byte in interrupt mode */
  HAL_UART_Receive_IT(&huart1, &uartbyte, 1);

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RED_LED_Pin|GRN_LED_Pin|BLU_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EPD_PWR_Pin|NFC_PWR_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EPD_RST_Pin|EPD_DC_Pin|EPD_CS_Pin|FLASH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BLUE_Pin */
  GPIO_InitStruct.Pin = BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BLUE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RED_Pin */
  GPIO_InitStruct.Pin = RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(RED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RED_LED_Pin GRN_LED_Pin BLU_LED_Pin */
  GPIO_InitStruct.Pin = RED_LED_Pin|GRN_LED_Pin|BLU_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : EPD_PWR_Pin EPD_RST_Pin EPD_DC_Pin EPD_CS_Pin 
                           FLASH_CS_Pin NFC_PWR_Pin */
  GPIO_InitStruct.Pin = EPD_PWR_Pin|EPD_RST_Pin|EPD_DC_Pin|EPD_CS_Pin 
                          |FLASH_CS_Pin|NFC_PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : EPD_BUSY_Pin */
  GPIO_InitStruct.Pin = EPD_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EPD_BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_Pin */
  GPIO_InitStruct.Pin = VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT_Pin */
  GPIO_InitStruct.Pin = MEMS_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_DN_Pin BTN_Pin BTN_UP_Pin */
  GPIO_InitStruct.Pin = BTN_DN_Pin|BTN_Pin|BTN_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : NFC_INT_Pin */
  GPIO_InitStruct.Pin = NFC_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(NFC_INT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */
void setLED(bool r, bool g, bool b) {
	HAL_GPIO_WritePin(GPIOA, RED_LED_Pin, g?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GRN_LED_Pin, r?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, BLU_LED_Pin, b?GPIO_PIN_SET:GPIO_PIN_RESET);
}

void notifyLED(bool r, bool g, bool b) {
	led_show=3;
	setLED(r, g, b);
}
/*void ledColor(uint8_t r, uint8_t g, uint8_t b) {
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.Pulse = b;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
    sConfigOC.Pulse = r;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3);
    sConfigOC.Pulse = g;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}*/

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin) {
	uint8_t s;
	uint32_t now = tick;
	//printf("EXTI %x at %d\n", GPIO_Pin, now);
	switch (GPIO_Pin) {
	case GPIO_PIN_13:
		puts("Motion event?");
		break;
	case MEMS_INT_Pin:
		s = MEMS_Process();
		if (s==1){
			last_event = EVENT_DOUBLE_TAP;
			led_show=3;
			//setLED(true, false, true);
		}
		else if (s==2) {
			last_event = EVENT_HIT;
			led_show=3;
			//setLED(false, false, true);
		}
		else
			return; //don't count this a activity and don't keep system awake
	case VBUS_Pin:
		if (HAL_GPIO_ReadPin(VBUS_GPIO_Port, VBUS_Pin) == GPIO_PIN_SET) {
			stay_awake |= USB_POWER;
			puts("USB power on");
		}
		else {
			stay_awake &= ~USB_POWER;
			puts("%d USB power off");
		}
		break;
	case NFC_INT_Pin:
		puts("NFC event");
		last_event = EVENT_NFC;
		break;
	case BTN_DN_Pin:
		if ((now - debounce) > DEBOUNCE) {
			puts("button down");
			last_event = EVENT_KEY_DOWN;
			debounce = now;
		}
		break;
	case BTN_UP_Pin:
		if ((now - debounce) > DEBOUNCE) {
			puts("button up");
			last_event = EVENT_KEY_UP;
			debounce = now;
		}
		break;
	case BTN_Pin:
		if ((now - debounce) > DEBOUNCE) {
			puts("button enter");
			last_event = EVENT_KEY_SELECT;
			debounce = now;
		}
		break;
	case GPIO_PIN_1:
		if ((now - debounce) > DEBOUNCE) {
			puts("pad jumped");
			last_event = EVENT_WIRE_JUMP;
			debounce = now;
		}
		break;
	default:
		printf("%d Unknown EXTI event via pin %x\n", now, GPIO_Pin);
		break;
	}

	idle=0;
	HAL_PWR_DisableSleepOnExit();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim21.Instance){
		int s = tick++ %50;
		if (led_show)
			led_show--;
		else if (s < 7) {
			if (s % 2)
				setLED(false, false, false);
			else
				switch(GetLandmarks().group[s/2]) {
				case 0:
					setLED(true, false, false);
					break;
				case 1:
					setLED(true, true, true);
					break;
				case 2:
					setLED(false, true, false);
					break;
				case 3:
					setLED(false, true, true);
					break;
				case 4:
					setLED(false, false, true);
					break;
				}
		}
		else
			setLED(false, false, false);
		/*if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) {
			idle=0;
		}*/
		idle++;

		if ((idle>=WAKETIME)) {// && (ledWav[led_idx % 64] == 0))
			if (stay_awake == 0){
				printf("Idle for %ds, ", idle/5);
				Standby();
			}
			//else if (idle == WAKETIME)
			//	DrawLogo(true);
		}

		//if (HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_1) == GPIO_PIN_RESET)
		//	puts("jumped");
	}
}

/* This callback is called by the HAL_UART_IRQHandler when the given number of bytes are received */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
	  if (ustate==IDLE) {
		  switch(uartbyte) {
		  case 'w':
		  case 'W':
			  setWritable(1);
			  puts("Flash writing enabled");
			  break;
		  case 'l':
		  case 'L':
			  setWritable(0);
			  puts("Flash writing disabled");
			  break;
		  case 'r':
		  case 'R':
			  puts("Reboot initiated");
			  NVIC_SystemReset();
			  break;
		  case 'e':
		  case 'E':
			  puts("Are you sure you want to erase all flash memory!?");
			  ustate = WIPE_CONFIRM;
			  break;
		  case 'g':
		  case 'G':
			  puts("Are you sure you want reset game progress!?");
			  ustate = RESET_CONFIRM;
			  break;
		  case 'p':
		  case 'P':
			  puts("Preparing to program flash via terminal ");
			  ustate = PROGRAM;
			  return;
		  case 'f':
		  case 'F':
			  printf("Game flags: %s\n", GameFlags());
			  break;
		  default:
			  puts("Welcome to Plum's service terminal");
			  puts("\tR - Reboots badge");
			  puts("\tG - Resets game progress");
			  puts("\tT - Runs systems test");
			  puts("\tE - Erases all content on the NOR FLash");
			  puts("\tW - Enables writing to the NOR flash via USB(must be erased first)");
			  puts("\tL - Disables writing to the flash via USB");
			  puts("\tP - Program flash over terminal");
			  puts("\tF - Show game flags state");

		  }
	  }
	  else if (ustate==WIPE_CONFIRM) {
		  if ((uartbyte == 'y') || (uartbyte == 'Y')) {
			  ustate = WIPE_PENDING;
			  HAL_PWR_DisableSleepOnExit();
		  }
		  else {
			  puts("Erase canceled");
			  ustate = IDLE;
		  }
	  }
	  else if (ustate==RESET_CONFIRM) {
		  if ((uartbyte == 'y') || (uartbyte == 'Y')) {
			  puts("Game progress reset");
			  GameReset();
		  }
		  else
			  puts("Reset canceled");
		  ustate = IDLE;
	  }
	  else if (ustate==WIPE_PENDING)
		  puts("Please wait, NOR flash is being erased");
	  else if (ustate==SYSTEM_TEST)
		  puts("System test in progress");
	/* Receive one byte in interrupt mode */
	HAL_UART_Receive_IT(&huart1, &uartbyte, 1);
  }
}

static void ProgramFlash(void) {
	uint8_t buf[256];
	uint32_t expected;
	W25qxx_EraseChip();
	puts("Ready, enter Flash size:");

	HAL_UART_Receive(&huart1, &expected, sizeof(expected), HAL_MAX_DELAY);
	expected /= w25qxx.PageSize;
	printf("Writing %d pages\n", expected);

	for (uint32_t addr=0; addr<expected; ++addr) {
		HAL_UART_Receive(&huart1, buf, 256, HAL_MAX_DELAY);
		W25qxx_WritePage(buf, addr, 0, w25qxx.PageSize);
		puts(".");
	}

	puts("done");
	//restore normal behavior
	HAL_UART_Receive_IT(&huart1, &uartbyte, 1);
}

static void SystemPower_Config(void)
{
  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();

  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();
}

void Sleep() {
	puts("sleeping");

    /* Configure low-power mode */
    //SCB->SCR &= ~( SCB_SCR_SLEEPDEEP_Msk );  // low-power mode = sleep mode
    //SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;     // reenter low-power mode after ISR

    /* Ensure Flash memory stays on */
    //FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;
    //__WFI();  // enter low-power mode

	HAL_PWR_EnableSleepOnExit();
	HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();
    puts("awake");
}

void Standby() {
	puts("Going to standby");
	HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);

	//setLED(false, false, true);
	NFC_PowerDown();
	//ledColor(0,0,0);


	HAL_ResumeTick();

	GamePause();
	if (readVoltage() < 3100)
		EPD_DrawLowPower(80, 196);
	W25qxx_PowerDown();
	HAL_Delay(75);

	HAL_GPIO_WritePin(EPD_PWR_GPIO_Port, EPD_PWR_Pin, GPIO_PIN_RESET); //Turn off EPD
	//HAL_DeInit();

	MEMS_Clear(); //Clear any missed motion events

	HAL_SuspendTick();
	setLED(false, false, false);

	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	//TODO: set this to seconds left before next minute +1
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 59, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
	/* Clear all related wakeup flags*/
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	/* Enable WakeUp Pin PWR_WAKEUP_PIN1 connected to PA.0 */
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);  //Select Key press
	//HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2);  //Movement

	HAL_PWR_EnterSTANDBYMode();
}



uint16_t readVoltage() {

	//Wait until ADC is fully powered up, probably not necessary
	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VREFINTRDY));

	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 1000);

	uint32_t volt_count = HAL_ADC_GetValue(&hadc);

	uint16_t voltage = (3000 * (*VREFINT_CAL_ADDR))/volt_count;
	HAL_ADC_Stop(&hadc);

	return voltage;
}

int dayofweek(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    int r= ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
    if (r==0) r=7;
    return r;
}

bool SetTime(char* line) {
	unsigned int mon,day,year,hour,min,sec;
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	// "MM/DD/YY HH:MM:SS"
	int c = sscanf(line, "%2u/%2u/%2u %2u:%2u:%2u",&mon,&day,&year,&hour,&min,&sec);
	if (c!=6)
		return false;

	sTime.Hours = hour;
	sTime.Minutes = min;
	sTime.Seconds = sec;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	sDate.WeekDay = dayofweek(day, mon, year);
	sDate.Month = mon;
	sDate.Date = day;
	sDate.Year = year;

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	return true;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  HAL_ResumeTick();
  puts("Fatal Error, System halted");
  for (int i=0; i< 10; ++i) {
	  HAL_Delay(500);
	  setLED(true, false, false);
	  HAL_Delay(500);
	  setLED(false, false, false);
  }

  puts("Restarting...");
  NVIC_SystemReset();
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	printf("Assert Failed! %s line %d\r\n", file, line)
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
