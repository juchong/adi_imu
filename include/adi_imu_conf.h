/**
  * @file		  adi_imu_conf.h
  * @date		  10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Global adi_imu driver configuration settings.
 **/


#ifndef __ADI_IMU_CONF_H_
#define __ADI_IMU_CONF_H_


/**
  * IMU family selection. Uncomment the target IMU family to build in support. 
  * Only one device should be selected at a time!
 **/
//#define ADIS1646X             1
#define ADIS1647X             1
//#define ADIS1649X             1
//#define ADIS1650X             1


/**
  * Enable compiling floating-point operations. 
  * Adds support for applying scale factors to IMU data.
 **/
#define ENABLE_SCALED_DATA    1


/**
  * Enable using burst read mode to access sensor data?
  * Does not affect discreet register reads/writes.
 **/
#define ENABLE_BURST_MODE     1


/**
  * Set the tx and rx buffer size. Used for managing SPI transactions.
 **/
#define SPI_BUFF_SIZE 64


#endif
