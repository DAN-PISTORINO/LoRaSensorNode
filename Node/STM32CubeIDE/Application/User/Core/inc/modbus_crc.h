/*
 * modbus_crc.h
 *
 *  Created on: Oct 7, 2024
 *      Author: Daniel Pistorino
 *      Reference: https://github.com/hasaneee/stm32_modbus_master/blob/main/Core/Inc/modbus_crc.h
 */

#ifndef APPLICATION_USER_CORE_INC_MODBUS_CRC_H_
#define APPLICATION_USER_CORE_INC_MODBUS_CRC_H_

uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);

#endif /* APPLICATION_USER_CORE_INC_MODBUS_CRC_H_ */
