/*
 * modbus.c
 *
 *  Created on: Oct 7, 2024
 *      Author: Daniel Pistorino
 *      Reference: https://embeddedthere.com/how-to-interface-stm32-with-rs485-modbus-sensors-with-hal-example-code/
 */

/**
  * @brief  sample usage
  * @retval int
  */
//int main(void)
//{
//  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//  HAL_Init();
//
//  /* Configure the system clock */
//  SystemClock_Config();
//
//  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
//  MX_USART2_UART_Init();
//  MX_USART1_UART_Init();
//
//  // Enable the receive interrupt
//  HAL_UARTEx_ReceiveToIdle_IT(&huart1, uart1_rx_data, 32);
//
//  /* Infinite loop */
//  /* USER CODE BEGIN WHILE */
//  while (1)
//  {
//    modbus_tx_data();
//    HAL_Delay(1000);
//  }
//}

/* Includes -*/
#include <modbus.h>
#include <modbus_crc.h>
#include "main.h"
#include "stdint.h"


uint8_t uart1_tx_data[8]; 	// holds data request for slave device
uint8_t uart1_rx_data[15];	// stores received data from slave device
uint16_t recv_data[2];		// 16-bit register for modbus transmission

// format and send data according to modbus protocol
void modbus_tx_data(){
	uart1_tx_data[0] = 0x41; // slave address
	uart1_tx_data[1] = 0x03; // Function code for Read Input Registers (0x03)

	// this 2 byte indicate the first 16-bit register.
	// where we want to start reading
	//The Register address will be 00000000 00000000
	uart1_tx_data[2] = 0x00;
	uart1_tx_data[3] = 0x00;

	// this 2 byte indicate how many 16-bit register we want to read.
	// we will read 2, 16-bit data register from_
	// 00000000 00000000 to 00000000 00000001
	uart1_tx_data[4] = 0x00;
	uart1_tx_data[5] = 0x02;

	// CRC Check function
	uint16_t crc = crc16(uart1_tx_data, 6);
	uart1_tx_data[6] = crc & 0xFF; //CRC LOW
	uart1_tx_data[7] = (crc >> 8) & 0xFF; //CRC HIGH

	// set DE,RE pin LOW to set max485 as transmitter mode
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
	// sending the uart1_tx_data array
	HAL_UART_Transmit(&huart1, uart1_tx_data, 8, 1000);
	// set DE,RE pin HIGH to set max485 as receiver mode
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET);

}


// The following function is called when the interrupt gets triggered
// UART tx is 8 bits, but modbus is 18 bits
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	recv_data[0] = uart1_rx_data[3]<<8 | uart1_rx_data[4];
	recv_data[1] = uart1_rx_data[5]<<8 | uart1_rx_data[6];
}
