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

/* IMU polarity typedef */
typedef enum {
    NEGATIVE = 0,
    POSITIVE = 1
} adi_imu_Polarity;

/* Signal edge typedef */
typedef enum {
    FALLING_EDGE = 0,
    RISING_EDGE = 1
} adi_imu_EdgeType;

/* Data-ready GPIO typedef */
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

/* IMU device information struct */
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
    ADI_IMU_SUCCESS = 0,                    /* (0) Success/No Error */
    ADI_IMU_SPIRW_FAILED,                   /* (1) SPI read/write failed */
    ADI_IMU_PRODID_VERIFY_FAILED,           /* (2) Product ID read from the device does not match the compiled library */
    ADI_IMU_INVALID_DATA_RATE,              /* (3) An invalid data rate was requested */
} adi_imu_Status;

/* Scaled data struct */
#if ENABLE_SCALED_DATA
typedef struct {
    float xg;
    float yg;
    float zg;
    float xa;
    float ya;
    float za;
    float temperature;
#if MAGNETOMETER_EN
    float xm;
    float ym;
    float zm;
#endif
#if BAROMETER_EN
    float baro;
#endif
    } adi_imu_ScaledData;
#endif

/* Unscaled data struct */
typedef struct {
    int32_t xg;
    int32_t yg;
    int32_t zg;
    int32_t xa;
    int32_t ya;
    int32_t za;
    int16_t temperature;
#if MAGNETOMETER_EN
    int16_t xm;
    int16_t ym;
    int16_t zm;
#endif
#if BAROMETER_EN
    int16_t baro;
#endif
} adi_imu_UnscaledData;

extern int32_t spi_Transfer(uint16_t *txBuf, uint16_t *rxBuf, uint16_t xferLen, uint16_t wordLen, uint32_t stall);
extern void delay_US(uint32_t microseconds);
extern void delay_MS(uint32_t milliseconds);

/* Initialization routine */
adi_imu_Status adi_imu_Init();

/* Write to IMU register */
adi_imu_Status adi_imu_WriteReg(uint16_t pageIDRegAddr, uint16_t val);

/* Read several IMU registers at once */
adi_imu_Status adi_imu_ReadRegArray(uint16_t *txBuf, uint16_t *rxBuf, uint16_t xferLen, uint16_t wordLen);

/* Read a single IMU register */
uint16_t adi_imu_ReadReg(uint16_t pageIDRegAddr);

/* Execute a flash update */
adi_imu_Status adi_imu_FlashUpdate();

/* Execute a software reset */
adi_imu_Status adi_imu_SoftwareReset();

/* Configure the sensor data rate */
adi_imu_Status adi_imu_SetDataRate(uint16_t dataRate);

/* Check SPI communication */
adi_imu_Status adi_imu_CheckComs();

/* Update device info/state */
adi_imu_Status adi_imu_GetDeviceInfo(adi_imu_DeviceInfo *data_info);

/* Set the active register page */
adi_imu_Status adi_imu_SetActivePage(uint16_t page);

/* Get the active register page */
int32_t adi_imu_GetActivePage();

/* Trigger a read of the inertial data and populate the scaled data struct */
adi_imu_Status adi_imu_GetScaledSensorData(adi_imu_ScaledData *data_struct);

/* Trigger a read of the inertial data and populate the unscaled data struct */
adi_imu_Status adi_imu_GetUnscaledSensorData(adi_imu_UnscaledData *data_struct);

#ifdef __cplusplus
}
#endif
#endif
