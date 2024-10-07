/*
 * modbus_crc.h
 *
 *  Created on: Oct 7, 2024
 *      Author: Daniel Pistorino
 */

#ifndef APPLICATION_USER_CORE_INC_MODBUS_H_
#define APPLICATION_USER_CORE_INC_MODBUS_H_


/* Private variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
void modbus_tx_data();

/* data RX interrupt handler */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef, uint16_t);



#endif /* APPLICATION_USER_CORE_INC_MODBUS_H_ */
