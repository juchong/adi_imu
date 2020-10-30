/**
  * @file		spi_driver.h
  * @date		10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Generic SPI driver to be implemented for the target platform.
 **/

#ifndef __SPI_DRIVER_H_
#define __SPI_DRIVER_H_

#include "adi_imu.h"

/* Generic SPI transfer function */
int32_t spi_Transfer(uint16_t *txBuf, uint16_t *rxBuf, uint16_t xferLen, uint16_t wordLen, uint32_t stall);

/* Generic microsecond delay function */
void delay_US(uint32_t microseconds);

/* Generic millisecond delay function */
void delay_MS(uint32_t milliseconds);

#endif