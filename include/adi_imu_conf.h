/**
  * @file		  adi_imu_conf.h
  * @date		  10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Global adi_imu driver configuration settings.
 **/

/* IMU family selection. Uncomment the target IMU family to build in support. Only one device should be selected at a time! */
//#include "adis1646x.h" /* ADIS16465, ADIS16467 */
//#include "adis1647x.h" /* ADIS16470, ADIS16475, ADIS16477 */
//#include "adis1649x.h" /* ADIS16495, ADIS16497 */
//#include "adis1650x.h" /* ADIS16500, ADIS16505, ADIS16507 */

/* Compile support for data scaling? */
#define SCALED_DATA_EN  1

/* Use burst read mode to access sensor data? */
#define BURST_MODE_EN   1


