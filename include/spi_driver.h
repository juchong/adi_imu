/**
  * @file		  spi_driver.h
  * @date		  10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Generic SPI driver to be implemented for the target platform.
 **/

#ifndef __SPI_DRIVER_H_
#define __SPI_DRIVER_H_

#include "adi_imu.h"
#include "adi_imu_conf.h"

/** 
 * @brief Gets the active IMU page if the IMU supports it.
 * 
 * @param txBuf The BYTE array to be transmitted.
 * 
 * @param rxBuf The BYTE array that should receive the data.
 * 
 * @param xferLen The total length of the array to be transmitted/received in BYTES.
 * 
 * @param wordLen The number of BYTES that should be transmitted per CS assertion/deassertion. 
 * 
 * @param stallTime The number of microseconds to wait between each transmitted word.
 * 
 * @return A status code indicating the success of the subroutine.
 * 
 * This function provides an interface between the SPI hardware and the IMU library. This function must be implemented
 * exactly as described for the library to work properly!
 **/
adi_imu_Status spi_Transfer(uint8_t *txBuf, uint8_t *rxBuf, uint16_t xferLen, uint16_t wordLen, uint16_t stallTime);

/* Generic microsecond delay function */
void delay_US(uint32_t microseconds);

/* Generic millisecond delay function */
void delay_MS(uint32_t milliseconds);

#endif