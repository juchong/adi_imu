/**
  * @file		adi_imu.h
  * @date		10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Global adi_imu driver defines.
 **/

#ifndef __ADI_IMU_H_
#define __ADI_IMU_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_LANGUAGE_C) || (defined(__GNUC__) && !defined(__ASSEMBLER__))
#include <stdint.h>
#endif /* _LANGUAGE_C */

#include "adi_imu_conf.h"
#include "spi_driver.h"

extern int spi_Transfer(uint16_t *txBuf, uint16_t *rxBuf, uint16_t xferLen, uint32_t stall);
extern void delay_US(uint32_t microseconds);
extern void delay_MS(uint32_t milliseconds);

/* Initialization routine */
int adi_imu_Init();

/* Write to IMU register */
int adi_imu_WriteReg(uint16_t pageIDRegAddr, uint16_t val);

/* Read several IMU registers at once */
int adi_imu_ReadRegArray(uint16_t *txBuf, uint16_t *rxBuf, uint16_t xferLen);

/* Read a single IMU register */
uint16_t adi_imu_ReadReg(uint16_t pageIDRegAddr);

/* Execute a flash update */
int adi_imu_FlashUpdate();

/* Execute a software reset */
int adi_imu_SoftwareReset();

/* Configure the sensor data rate */
int adi_imu_SetDataRate(uint16_t dataRate);

/* Check SPI communication */
int adi_imu_CheckComs();

/* Update device info/state */
int adi_imu_GetDeviceInfo();

/* Set the active register page */
int adi_imu_SetActivePage(uint16_t page);

/* Get the active register page */
int adi_imu_GetActivePage();

/* Trigger a read of the inertial data */
int adi_imu_GetSensorData();

typedef enum {
    FALSE = 0,
    TRUE = 1
} adi_imu_Boolean;

typedef enum {
    NEGATIVE = 0,
    POSITIVE = 1
} adi_imu_Polarity;

typedef enum {
    FALLING_EDGE = 0,
    RISING_EDGE = 1
} adi_imu_EdgeType;

typedef enum {
    DIO1 = 0,
    DIO2 = 1,
    DIO3 = 2,
    DIO4 = 3
} adi_imu_DatRdyGPIO;

typedef enum {
    INTRNL = 0,
    SYNC = 1,
    PPS = 2
} adi_imu_ClkMode;

typedef struct {
    uint16_t prodId;
    uint16_t fwRev;
    uint16_t fwDay;
    uint16_t fwMonth;
    uint16_t fwYear;
    uint16_t serialNumber;
    uint16_t pageId;
    uint16_t decRate;
    uint16_t clkMode;
} adi_imu_DeviceInfo;

/** Enum of library errors */
typedef enum {
    adi_imu_InvalidDataRate_e = -4,
    adi_imu_IncorrectRange_e = -3,
    adi_imu_ProdIdVerifyFailed_e = -2,
    adi_imu_RwFailed_e = -1,
    adi_imu_Success_e = 0,
} adi_imu_Error;

typedef struct {
    int16_t xg;
    int16_t yg;
    int16_t zg;
    int16_t xa;
    int16_t ya;
    int16_t za;
    int16_t temperature;
} adi_imu_GyroAccel16BitData;

#ifdef SCALED_DATA_EN
    typedef struct {
        double xg;
        double yg;
        double zg;
        double xa;
        double ya;
        double za;
        double temperature;
    } adi_imu_GyroAccel16BitDataScaled;
#endif

#ifdef USE_32BIT
    typedef struct {
        int32_t xg;
        int32_t yg;
        int32_t zg;
        int32_t xa;
        int32_t ya;
        int32_t za;
        int16_t temperature;
    } adi_imu_GyroAccel32BitData;
    #ifdef SCALED_DATA_EN
    typedef struct {
        double xg;
        double yg;
        double zg;
        double xa;
        double ya;
        double za;
        double temperature;
    } adi_imu_GyroAccel32BitDataScaled;
    #endif
#endif

#ifdef MAGNETOMETER_EN
    typedef struct {
        int16_t xm;
        int16_t ym;
        int16_t zm;
    } adi_imu_MagData;
#endif

#ifdef BAROMETER_EN
    typedef struct {
        int16_t baro;
    } adi_imu_BaroData;
#endif

#ifdef __cplusplus
}
#endif
#endif
