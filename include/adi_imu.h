/**
  * @file	    adi_imu.h
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

/* Boolean typedef */
typedef enum {
    FALSE = 0,
    TRUE = 1
} adi_imu_Boolean;

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

/* IMU range register */
typedef enum {
    RANGE_125DPS = 0,
    RANGE_500DPS = 4,
    RANGE_2000DPS = 12
} adi_imu_RangeReg;

/* IMU device information struct */
typedef struct {
    uint16_t prodId;
    uint16_t fwRev;
    uint16_t fwDayMonth;
    uint16_t fwYear;
    uint16_t serialNumber;
    uint16_t decRate;
#if SUPPORTS_PAGES
    uint16_t activePageId;
#endif
#if SUPPORTS_RANGE_REG
    adi_imu_RangeReg range;
#endif
} adi_imu_DeviceInfo;

/** Enum of library errors */
typedef enum {
    ADI_IMU_SUCCESS = 0,                    /* (0) Success/No Error */
    ADI_IMU_SPIRW_FAILED,                   /* (1) SPI read/write failed */
    ADI_IMU_PRODID_VERIFY_FAILED,           /* (2) Product ID read from the device does not match the compiled library */
    ADI_IMU_INVALID_DATA_RATE,              /* (3) An invalid data rate was requested */
    ADI_IMU_BURST_NOT_SUPPORTED,            /* (4) The selected IMU configuration does not support burst data capture */
    ADI_IMU_CHECK_SPI_COMS_FAILED,          /* (5) The SPI communication verification routine failed to read back valid data */
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
#if ENABLE_MAGNETOMETER
    float xm;
    float ym;
    float zm;
#endif
#if ENABLE_BAROMETER
    float baro;
#endif
    } adi_imu_ScaledData;
#endif

/* Target 16-bit scale factors */
#if ENABLE_SCALED_DATA
typedef struct {
    float gyro16Scale;
    float accel16Scale;
    float tempScale;
    #if ENABLE_MAGNETOMETER
        float magScale;
    #endif
    #if ENABLE_BAROMETER
        float baroScale;
    #endif
} adi_imu_16Bit_ScaleFactors;
#endif

/* Target 32-bit scale factors */
#if ENABLE_SCALED_DATA
    #if SUPPORTS_32BIT_REGS
        #if ENABLE_32BIT_DATA
        typedef struct {
            float gyro32Scale;
            float accel32Scale;
            float tempScale;
            #if ENABLE_MAGNETOMETER
                float magScale;
            #endif
            #if ENABLE_BAROMETER
                float baroScale;
            #endif
        } adi_imu_32Bit_ScaleFactors;
        #endif
    #endif
#endif

/* Unscaled data struct */
typedef struct {
    int32_t xg;
    int32_t yg;
    int32_t zg;
    int32_t xa;
    int32_t ya;
    int32_t za;
    int32_t temperature;
#if ENABLE_MAGNETOMETER
    int32_t xm;
    int32_t ym;
    int32_t zm;
#endif
#if ENABLE_BAROMETER
    int32_t baro;
#endif
} adi_imu_UnscaledData;

/* Initialization routine */
adi_imu_Status adi_imu_Init();

/* Write to IMU register */
adi_imu_Status adi_imu_WriteReg(uint16_t pageIDRegAddr, uint16_t val);

/* Read several IMU registers at once */
adi_imu_Status adi_imu_ReadRegArray(const uint16_t *regList, uint16_t *outData, uint16_t numRegs, uint16_t timesToRead);

/* Read a single IMU register */
adi_imu_Status adi_imu_ReadReg(uint16_t pageIDRegAddr, uint16_t *val);

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

#if SUPPORTS_PAGES
    /* Set the active register page */
    adi_imu_Status adi_imu_SetActivePage(uint16_t page);

    /* Get the active register page */
    adi_imu_Status adi_imu_GetActivePage(uint16_t *active_page);
#endif

/* Trigger a read of the inertial data and populate the unscaled data struct */
adi_imu_Status adi_imu_GetSensorData(adi_imu_UnscaledData *data_struct);

#if ENABLE_SCALED_DATA
    /* Trigger a read of the inertial data and populate the scaled data struct */
    adi_imu_Status adi_imu_GetScaledSensorData(adi_imu_ScaledData *data_struct);
#endif

#ifdef __cplusplus
}
#endif
#endif
