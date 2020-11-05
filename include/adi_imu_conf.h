/**
  * @file		  adi_imu_conf.h
  * @date		  10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Global adi_imu driver configuration settings.
 **/

#ifndef __ADI_IMU_CONF_H_
#define __ADI_IMU_CONF_H_

#include "adi_imu.h"

/**
 * IMU family selection. Uncomment the target IMU family to build in support. 
 * Only one device should be selected at a time!
 **/
//#include "adis16448.h"      /* ADIS16448 */
//#include "adis1646x.h"      /* ADIS16465, ADIS16467 */
#include "adis1647x.h"        /* ADIS16470, ADIS16475, ADIS16477 */
//#include "adis1649x.h"      /* ADIS16495, ADIS16497 */
//#include "adis1650x.h"      /* ADIS16500, ADIS16505, ADIS16507 */


/**
 * Enable reading IMU data in 32-bit format if the IMU supports it.
 **/
#if SUPPORTS_32BIT_REGS
  #define ENABLE_32BIT_DATA               1
#endif


/**
 * Enable compiling floating-point operations. 
 * Adds support for applying scale factors to IMU data.
 **/
#define ENABLE_SCALED_DATA                1


/**
 * Enable using burst read mode to access sensor data?
 * Does not affect discreet register reads/writes.
 **/
#if SUPPORTS_BURST
  #define ENABLE_BURST_MODE               1
#endif


/**
 * Enable 32-bit reads to access sensor data (if the sensor supports it)?
 **/
#if SUPPORTS_32BIT_BURST
  #define ENABLE_32_BIT_BURST_MODE        1
#endif


/**
 * Perform an IMU communications check after executing any subroutine.
 **/
#define CHECK_COMS_AFTER_COMMAND          1


/**
 * Set the tx and rx buffer size. Used for managing SPI transactions.
 **/
#define SPI_BUFF_SIZE                     64


#endif
