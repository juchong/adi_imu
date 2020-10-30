/**
  * @file	    adi_imu.c
  * @date		10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Global adi_imu driver implementation.
 **/

#include "adi_imu.h"
#include "adi_imu_conf.h"
#include "spi_driver.h"

//static uint8_t tx_buff[SPI_BUFF_SIZE]
//static uint8_t rx_buff[SPI_BUFF_SIZE]

extern adi_imu_Status spi_Transfer(uint16_t *txBuf, uint16_t *rxBuf, uint16_t xferLen, uint16_t wordLen, uint32_t stall);
extern void delay_US(uint32_t microseconds);
extern void delay_MS(uint32_t milliseconds);