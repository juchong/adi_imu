/**
  * @file	    adi_imu.c
  * @date		10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		Global adi_imu driver implementation.
 **/

#include "adi_imu.h"
#include "adi_imu_conf.h"
#include "spi_driver.h"

extern adi_imu_Status spi_Transfer(uint8_t *txBuf, uint8_t *rxBuf, uint16_t xferLen, uint16_t wordLen, uint16_t stallTime);
extern void delay_US(uint32_t microseconds);
extern void delay_MS(uint32_t milliseconds);

static uint8_t txBuf[SPI_BUFF_SIZE];
static uint8_t rxBuf[SPI_BUFF_SIZE];
static adi_imu_Status status = ADI_IMU_SUCCESS;


adi_imu_Status adi_imu_Init() 
{
    status = adi_imu_CheckComs();

#if SUPPORTS_PAGES
    if (status == ADI_IMU_SUCCESS)
    {
        status = adi_imu_GetActivePage(activePage);
        if (status == ADI_IMU_SUCCESS)
        {
            pageValid = TRUE;
        }
    }
#endif

    return status;
}

/** 
 * @brief Read a single register from the IMU
 * 
 * @param pageIDRegAddr The register location to be written to
 * 
 * @param val The data to be written to the sensor
 * 
 * @return A status code indicating the success of the SPI transaction
 * 
 * This function writes a 16-bit word (split into two 8-bit bytes) into the requested register
 * location and the adjacent location. If support for paged IMUs is compiled, a write 
 * to PAGE_ID is automatically appended to the transaction. 
 **/
adi_imu_Status adi_imu_WriteReg(uint16_t pageIDRegAddr, uint16_t val)
{

#if SUPPORTS_PAGES
    /* Append page write to message */
    txBuf[0] = (0x80 | PAGE_ID_REG & 0xFF);
    txBuf[1] = ((pageIDRegAddr >> 8) & 0xFF);
    txBuf[2] = (0x80 | (pageIDRegAddr & 0xFF));
    txBuf[3] = (val & 0xFF);
    txBuf[4] = (0x80 | ((pageIDRegAddr & 0xFF) + 1));
    txBuf[5] = ((val >> 8) & 0xFF);
    /* Transmit tx buffer */
    status = spi_Transfer(txBuf, rxBuf, 6, 16, STALL_TIME_US);
#else
    /* Prepare transmit buffer */
    txBuf[0] = (0x80 | (pageIDRegAddr & 0xFF));
    txBuf[1] = (val & 0xFF);
    txBuf[2] = (0x80 | ((pageIDRegAddr & 0xFF) + 1));
    txBuf[3] = ((val >> 8) & 0xFF);
    /* Transmit tx buffer */
    status = spi_Transfer(txBuf, rxBuf, 4, 16, STALL_TIME_US);
#endif

    return status;
}

/** 
 * @brief Read a single register from the IMU
 * 
 * @param pageIDRegAddr The register localtion to be read from
 * 
 * @param val A pointer to the data read back from the sensor
 * 
 * @return A status code indicating the success of the SPI transaction
 * 
 * This function reads a register location and places the resultant data in the location
 * provided. If support for paged IMUs is compiled, a write to PAGE_ID is automatically appended
 * to the transaction. 
 **/
adi_imu_Status adi_imu_ReadReg(uint16_t pageIDRegAddr, uint16_t *val)
{

#if SUPPORTS_PAGES
    /* Append page write to message */
    txBuf[0] = (0x80 | PAGE_ID_REG & 0xFF);
    txBuf[1] = ((pageIDRegAddr >> 8) & 0xFF);
    txBuf[2] = (pageIDRegAddr & 0xFF);
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x00;
    /* Transmit tx buffer */
    status = spi_Transfer(txBuf, rxBuf, 6, 16, STALL_TIME_US);
#else
    /* Prepare the tx buffer */
    txBuf[0] = (pageIDRegAddr & 0xFF);
    txBuf[1] = 0x00;
    txBuf[2] = 0x00;
    txBuf[3] = 0x00;
    /* Transmit tx buffer */
    status = spi_Transfer(txBuf, rxBuf, 4, 16, STALL_TIME_US);
#endif

    /* Combine bytes into word response */
    *val = ((rxBuf[2] << 8) | rxBuf[3]);

    return status;

}


/** 
 * @brief Read an array of registers in full-duplex mode.
 * 
 * @param regList A pointer to an array of registers to be read
 * 
 * @param outData A pointer to an array of data read back from the sensor corresponding to the requested register list
 * 
 * @param numRegs The number of registers contained in the array of registers
 * 
 * @return A status code indicating the success of the SPI transaction
 * 
 * This function reads an arbitrary array of registers and returns an array of data corresponding to the 
 * requested register list. The list is processed in the order the registers are stored within the array. 
 * If support for paged IMUs is compiled and the register list contains registers located on
 * different pages, writes to the PAGE_ID register are automatically inserted in the SPI transaction. 
 * A page write is always inserted at the beginning of the array read. Any extra SPI writes are automatically 
 * parsed out of the resultant data array.
 **/
adi_imu_Status adi_imu_ReadRegArray(const uint16_t *regList, uint16_t *outData, uint16_t numRegs)
{

#if SUPPORTS_PAGES
    uint8_t prevPageId = 0;
    uint16_t txBufCnt = 0;
    uint32_t regTracker = 0;

    /* Iterate through each register in the list and insert page writes as needed */
    for (uint16_t i = 0; i < numRegs; i++)
    {
        /* Insert page write at the beginning of the tx buffer */
        if (i == 0)
        {
            txBuf[0] = (0x80 | PAGE_ID_REG & 0xFF);
            txBuf[1] = ((regList[0] >> 8) & 0xFF);
            /* Store the page ID */
            prevPageId = txBuf[1];
            /* Increment the buffer size counter */
            txBufCnt = txBufCnt + 2;
            /* Set the nth bit in regTracker to indicate this word had a write appended */
            regTracker |= 1 << i;
        }
        else
        {
            if (((regList[i] >> 8) & 0xFF) != prevPageId)
            {
                txBuf[txBufCnt] = (0x80 | PAGE_ID_REG & 0xFF);
                txBuf[txBufCnt + 1] = ((regList[0] >> 8) & 0xFF);
                /* Store the page ID */
                prevPageId = txBuf[txBufCnt + 1];
                /* Increment the buffer size counter */
                txBufCnt = txBufCnt + 2;
                /* Set the nth bit in regTracker to indicate this word had a write appended */
                regTracker |= 1 << i;
            }
        }
        /* Insert data into the tx buffer */
        txBuf[txBufCnt] = (regList[i] & 0xFF);
        txBuf[txBufCnt + 1] = 0x00;
        /* Increment the buffer size counter */
        txBufCnt = txBufCnt + 2;
    }
    /* Append a dummy read request to the end of the array */
    txBuf[txBufCnt] = 0x00;
    txBuf[txBufCnt + 1] = 0x00;
    txBufCnt = txBufCnt + 2;
    /* Transmit the buffer */
    status = spi_Transfer(txBuf, rxBuf, txBufCnt, 16, STALL_TIME_US);
    /* Clear the local count variable for reuse and offset by two bytes */
    txBufCnt = 2;
    /* Parse and trim the rx buffer by stepping through the regTracker variable */
    for (uint16_t i = 0; i < numRegs; i++)
    {
        /* If the bit is set, skip the next two bytes */
        if (regTracker & (1<<i))
        {
            txBufCnt = txBufCnt + 2;
        }
        /* We know every extra transfer also included data, so pull that data into the output array */
        outData[i] = (rxBuf[txBufCnt] << 8) | rxBuf[txBufCnt + 1]);
        /* Increment txBufCnt */
        txBufCnt = txBufCnt + 2;
    }

    return status;

#else

    /* Copy the register list to the preallocated tx buffer */
    for (uint16_t i = 0; i < numRegs; i++)
    {
        txBuf[i * 2] = (regList[i] & 0xFF);
        txBuf[i * 2 + 1] = 0x00;
    }
    /* Append an extra, dummy word to the end of the list */
    txBuf[numRegs * 2 + 1] = 0x00;
    txBuf[numRegs * 2 + 2] = 0x00;

    /* Transmit the buffer */
    status = spi_Transfer(txBuf, rxBuf, (numRegs * 2 + 2), 16, STALL_TIME_US);

    /* Copy the rx buffer contents to the target buffer, skipping the first, garbage response */
    for (uint16_t i = 0; i < numRegs; i++)
    {
        outData[i] = (rxBuf[i * 2 + 2] << 8) | rxBuf[i * 2 + 3];
    }

    return status;

#endif

}

adi_imu_Status adi_imu_FlashUpdate()
{
    return status;
}

adi_imu_Status adi_imu_SoftwareReset()
{
    return status;
}

adi_imu_Status adi_imu_SetDataRate(uint16_t dataRate)
{
    return status;
}

adi_imu_Status adi_imu_CheckComs()
{
    return status;
}

adi_imu_Status adi_imu_GetDeviceInfo(adi_imu_DeviceInfo *data_info)
{
    return status;
}

adi_imu_Status adi_imu_SetActivePage(uint16_t page)
{
    return status;
}

adi_imu_Status adi_imu_GetActivePage(uint16_t *active_page)
{
    return status;
}

adi_imu_Status adi_imu_GetScaledSensorData(adi_imu_ScaledData *data_struct)
{
    return status;
}

adi_imu_Status adi_imu_GetSensorData(adi_imu_UnscaledData *data_struct)
{
    return status;
}
