/*
 * modbus_crc.h
 *
 *  Created on: Oct 7, 2024
 *      Author: Daniel Pistorino
 */

#ifndef APPLICATION_USER_CORE_INC_NPK_H_
#define APPLICATION_USER_CORE_INC_NPK_H_

/* Private variables */
UART_HandleTypeDef huart1;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

/* data RX interrupt handler */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef, uint16_t);

/* NPK functions */
void npk_data_request()
 */
#endif /* APPLICATION_USER_CORE_INC_NPK_H_ */
