/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart_if.c
  * @author  MCD Application Team
  * @brief   Configuration of UART driver interface for hyperterminal communication
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usart_if.h"

/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/**
  * @brief DMA handle
  */
extern DMA_HandleTypeDef hdma_lpuart1_tx;

/**
  * @brief UART handle
  */
extern UART_HandleTypeDef hlpuart1;

/**
  * @brief buffer to receive 1 character
  */
uint8_t charRx;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief Trace driver callbacks handler
  */
const UTIL_ADV_TRACE_Driver_s UTIL_TraceDriver =
{
  vcom_Init,
  vcom_DeInit,
  vcom_ReceiveInit,
  vcom_Trace_DMA,
};

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
  * @brief  TX complete callback
  * @return none
  */
static void (*TxCpltCallback)(void *);
/**
  * @brief  RX complete callback
  * @param  rxChar ptr of chars buffer sent by user
  * @param  size buffer size
  * @param  error errorcode
  * @return none
  */
static void (*RxCpltCallback)(uint8_t *rxChar, uint16_t size, uint8_t error);

/* USER CODE BEGIN PV */
void SYS_SensorPower(bool state);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions --------------------------------------------------------*/

UTIL_ADV_TRACE_Status_t vcom_Init(void (*cb)(void *))
{
  /* USER CODE BEGIN vcom_Init_1 */

  /* USER CODE END vcom_Init_1 */
  TxCpltCallback = cb;
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_28);
  return UTIL_ADV_TRACE_OK;
  /* USER CODE BEGIN vcom_Init_2 */

  /* USER CODE END vcom_Init_2 */
}

UTIL_ADV_TRACE_Status_t vcom_DeInit(void)
{
  /* USER CODE BEGIN vcom_DeInit_1 */

  /* USER CODE END vcom_DeInit_1 */
  /* ##-1- Reset peripherals ################################################## */
  __HAL_RCC_LPUART1_FORCE_RESET();
  __HAL_RCC_LPUART1_RELEASE_RESET();

  /* ##-2- MspDeInit ################################################## */
  HAL_UART_MspDeInit(&hlpuart1);

  /* ##-3- Disable the NVIC for DMA ########################################### */
  /* USER CODE BEGIN 1 */
  HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);

  return UTIL_ADV_TRACE_OK;
  /* USER CODE END 1 */
  /* USER CODE BEGIN vcom_DeInit_2 */

  /* USER CODE END vcom_DeInit_2 */
}

void vcom_Trace(uint8_t *p_data, uint16_t size)
{
  /* USER CODE BEGIN vcom_Trace_1 */

  /* USER CODE END vcom_Trace_1 */
  HAL_UART_Transmit(&hlpuart1, p_data, size, 1000);
  /* USER CODE BEGIN vcom_Trace_2 */

  /* USER CODE END vcom_Trace_2 */
}

UTIL_ADV_TRACE_Status_t vcom_Trace_DMA(uint8_t *p_data, uint16_t size)
{
  /* USER CODE BEGIN vcom_Trace_DMA_1 */

  /* USER CODE END vcom_Trace_DMA_1 */
  HAL_UART_Transmit_DMA(&hlpuart1, p_data, size);
  return UTIL_ADV_TRACE_OK;
  /* USER CODE BEGIN vcom_Trace_DMA_2 */

  /* USER CODE END vcom_Trace_DMA_2 */
}

UTIL_ADV_TRACE_Status_t vcom_ReceiveInit(void (*RxCb)(uint8_t *rxChar, uint16_t size, uint8_t error))
{
  /* USER CODE BEGIN vcom_ReceiveInit_1 */

  /* USER CODE END vcom_ReceiveInit_1 */
  UART_WakeUpTypeDef WakeUpSelection;

  /*record call back*/
  RxCpltCallback = RxCb;

  /*Set wakeUp event on start bit*/
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;

  HAL_UARTEx_StopModeWakeUpSourceConfig(&hlpuart1, WakeUpSelection);

  /* Make sure that no UART transfer is on-going */
  while (__HAL_UART_GET_FLAG(&hlpuart1, USART_ISR_BUSY) == SET);

  /* Make sure that UART is ready to receive)   */
  while (__HAL_UART_GET_FLAG(&hlpuart1, USART_ISR_REACK) == RESET);

  /* Enable USART interrupt */
  __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_WUF);

  /*Enable wakeup from stop mode*/
  HAL_UARTEx_EnableStopMode(&hlpuart1);

  /*Start LPUART receive on IT*/
  HAL_UART_Receive_IT(&hlpuart1, &charRx, 1);

  return UTIL_ADV_TRACE_OK;
  /* USER CODE BEGIN vcom_ReceiveInit_2 */

  /* USER CODE END vcom_ReceiveInit_2 */
}

void vcom_Resume(void)
{
  /* USER CODE BEGIN vcom_Resume_1 */

  /* USER CODE END vcom_Resume_1 */
  /*to re-enable lost DMA settings*/
  if (HAL_DMA_Init(&hdma_lpuart1_tx) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN vcom_Resume_2 */

  /* USER CODE END vcom_Resume_2 */
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* USER CODE BEGIN HAL_UART_TxCpltCallback_1 */

  /* USER CODE END HAL_UART_TxCpltCallback_1 */
  /* buffer transmission complete*/
  if (huart->Instance == LPUART1)
  {
    TxCpltCallback(NULL);
  }
  /* USER CODE BEGIN HAL_UART_TxCpltCallback_2 */

  /* USER CODE END HAL_UART_TxCpltCallback_2 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* USER CODE BEGIN HAL_UART_RxCpltCallback_1 */

  /* USER CODE END HAL_UART_RxCpltCallback_1 */
  if (huart->Instance == LPUART1)
  {
    if ((NULL != RxCpltCallback) && (HAL_UART_ERROR_NONE == huart->ErrorCode))
    {
      RxCpltCallback(&charRx, 1, 0);
    }
    HAL_UART_Receive_IT(huart, &charRx, 1);
  }
  /* USER CODE BEGIN HAL_UART_RxCpltCallback_2 */

  /* USER CODE END HAL_UART_RxCpltCallback_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private Functions Definition -----------------------------------------------*/

/* USER CODE BEGIN PrFD */
/**
 * @brief Turns on or off the 12V power to the sensor.
 * @param state=true turns the power on.
 */
void SYS_SensorPower(bool state) {
	if(state) {
		HAL_GPIO_WritePin(GPIOC, 0, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOC, 0, GPIO_PIN_RESET);
	}
}

void charArrayToUint8(const char* cA, uint8_t* uA, size_t len) {
	const char *cPtr = cA;
	uint8_t *uPtr = uA;
	while (len--) {
		*uPtr++=(uint8_t)*cPtr++;
	}
}
/**
 * NPK Inquiry Codes and Data Masks
 */
uint8_t REQUEST_NPK[]={0x01, 0x03, 0x00, 0x1E, 0x00, 0x03, 0x34, 0x0D};
uint8_t REQUEST_N[]={0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xB5, 0xCC};
uint8_t REQUEST_P[]={0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0x4E, 0x0C};
uint8_t REQUEST_K[]={0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};
/**
 * @brief Gets the Nitrogen data from the NPK sensor
 * @return Returns the Nitrogen value in mg/kg.
 */
uint16_t SYS_GetN(void) {
	char in[8];

	// Request value from sensor
	HAL_UART_Transmit(&huart1, REQUEST_N, strlen(REQUEST_N), 10);
	HAL_Delay(1000);
	// Receive data from UART
	HAL_UART_Receive(&huart1, in, 8, 1000);
	char out[2]= {in[3], in[4]};
	return (uint16_t) strtoul(out, (char**)0, 16);//(16*in[6]+1*in[7]);//atoi(val); // converts string to uint and returns
}
/**
 * @brief Gets the Phosphorus data from the NPK sensor
 * @return Returns the Phosphorus value in mg/kg.
 */
uint16_t SYS_GetP(void) {
	char in[8];

	// Request value from sensor
	HAL_UART_Transmit(&huart1, REQUEST_P, strlen(REQUEST_P), 10);
	HAL_Delay(1000);
	// Receive data from UART
	HAL_UART_Receive(&huart1, (uint8_t *)in, 8, 1000);
	char out[2]= {in[3], in[4]};
	return (uint16_t) strtoul(out, (char**)0, 16);//(16*in[6]+1*in[7]);//atoi(val); // converts string to uint and returns
}
/**
 * @brief Gets all 3 NPK values from the NPK sensor
 * @return void
 * @param Pointers of type uint16_t*.
 */
void SYS_GetNPK(uint16_t* N, uint16_t* P, uint16_t* K) {
	const uint8_t CYCLE_LIMIT = 5;
	uint8_t in[11];
	uint8_t received = 0;

	*N=0;
	*P=0;
	*K=0;

	while (received < CYCLE_LIMIT) {
		// Request value from sensor
		HAL_UART_Transmit(&huart1, REQUEST_NPK, strlen(REQUEST_NPK), 1000);
		HAL_Delay(1000);
		// Receive data from UART
		HAL_UART_Receive(&huart1, in, 11, 1000);
		if (in[10]!=0x53) {
			received++;
		}
		else {
			received = CYCLE_LIMIT + 1;
			*N = (uint16_t)in[3] | ((uint16_t)in[4]<<8);
			*P = (uint16_t)in[5] | ((uint16_t)in[6]<<8);
			*K = (uint16_t)in[7] | ((uint16_t)in[8]<<8);
		}
		if (received == CYCLE_LIMIT) {
			*N = 1;
			*P = 2;
			*K = 3;
		}
	}
}

/**
 * @brief Gets the Potassium data from the NPK sensor
 * @return Returns the Potassium value in mg/kg.
 */
uint16_t SYS_GetK(void) {
	char in[8];

	// Request value from sensor
	HAL_UART_Transmit(&huart1, (uint8_t *)REQUEST_K, strlen(REQUEST_K), 10);
	HAL_Delay(1000);
	// Receive data from UART
	HAL_UART_Receive(&huart1, (uint8_t *)in, 8, 1000);
	char out[2]= {in[3], in[4]};
	return (uint16_t)(strtoul(out, (char**)0, 16));//(16*in[6]+1*in[7]);//atoi(val); // converts string to uint and returns
}
/* USER CODE END PrFD */
