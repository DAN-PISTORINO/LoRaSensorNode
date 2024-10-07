/*
 * npk.c
 *
 *  Created on: Oct 7, 2024
 *      Author: Daniel Pistorino
 *      Reference: https://embeddedthere.com/how-to-interface-stm32-with-rs485-modbus-sensors-with-hal-example-code/

 * EXAMPLE USAGE
 *************
int main(void)
{
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  HAL_Init();

  // Configure the system clock
  SystemClock_Config();

  // Initialize all configured peripherals
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();

  // Enable the receive interrupt
  HAL_UARTEx_ReceiveToIdle_IT(&huart1, uart1_rx_data, 32);

  //  Infinite loop
  //  USER CODE BEGIN WHILE
  while (1)
  {
    npk_data_request();
    // wait until NPK values are returned
	while (!npk_data_is_good) {
		// Better to do other things than just stall until
		// sensor data returns, but this is for example.

		// wait 100 ms
		HAL_Delay(100)
	}

	// do something with the NPK values.
  }
}
*/
/* Includes -*/
#include <npk.h>
#include <modbus_crc.h>
#include "main.h"
#include "stdint.h"


uint8_t uart1_tx_data[8]; 	// holds data request for slave device
uint8_t uart1_rx_data[23];	// stores received data from slave device 3*8bits=24bits
/*
 * @brief NPK data
 * @note Nitrogen in npk_data[0]
 * @note Phosphate in npk_data[1]
 * @note Potassium in npk_data[2]
 */
uint16_t npk_data[2];		// 16-bit data register (N:P:K)
bool npk_rx_is_good; 			// 8-bit crc check validation flag
/*
 * @brief Send data request to npk sensor on UART1.
 * @note Data received into npk_data.
 * @note Data request uses modbus transmission format.
 * @note GPIO pin 8 (PA8) controls the MAX485 uart-rs485 converter module.
 */
void npk_data_request(){
	uart1_tx_data[0] = 0x01; // slave address
	uart1_tx_data[1] = 0x03; // Function code for Read Input Registers (0x03)

	// this 2 byte indicate the first 16-bit register.
	// where we want to start reading
	//The Register address will be 00000000 00000000
	uart1_tx_data[2] = 0x00;
	uart1_tx_data[3] = 0x1E;

	// this 2 byte indicate how many 16-bit register we want to read.
	// we will read 3, 16-bit data register from_
	// 00000000 0000001E to 00000000 00000020
	// Registers hold N:P:K
	uart1_tx_data[4] = 0x00;
	uart1_tx_data[5] = 0x03;

	// CRC Check function
	uint16_t crc = crc16(uart1_tx_data, 6);
	uart1_tx_data[6] = crc & 0xFF; //CRC LOW
	uart1_tx_data[7] = (crc >> 8) & 0xFF; //CRC HIGH

	npk_rx_is_good = false;

	// set DE,RE pin LOW to set max485 as transmitter mode
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
	// sending the uart1_tx_data array
	HAL_UART_Transmit(&huart1, uart1_tx_data, 8, 1000);
	// set DE,RE pin HIGH to set max485 as receiver mode
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET);
}

/*
 * @brief The following function is called when the interrupt gets triggered
 * @note UART tx is 8 bits, but receiving 3 packets
 * @note (Packet format)	address:function:num-bytes:nitrogen:phosphorous:potassium:CRC_L:CRC_H
 * 		 num bytes in pkt:	   1   :   1    :   1     :    2   :     2     :     2   :  1  :  1
 * @note (NPK interpretation)	N:x20=32mg/kg	P: x25=37mg/kg	K: x30=48mg/kg
**/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	// implementing crc check for received data
	uint16_t crc_chk = crc16(uart1_rx_data, 9);
	npk_rx_is_good = ((uart1_rx_data[9] & 0x00FF) | (uart1_rx_data[10] << 8))==crc_chk;			// crc check-sum;

	// if the crc passes then save the values
	// otherwise request a retransmission.
	if (npk_rx_is_good==True) {
		// received data is 8-bit int
		//		left shift 8 or with nex 8-bit to form 16-bit data
		npk_data[0] = uart1_rx_data[3]<<8 | uart1_rx_data[4];	// Nitrogen
		npk_data[1] = uart1_rx_data[5]<<8 | uart1_rx_data[6];	// Phosphorous
		npk_data[2] = uart1_rx_data[7]<<8 | uart1_rx_data[8];	// Potassium
	}
	else {
		npk_data_request();
	}
}
