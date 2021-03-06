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
static uint16_t tempRegA, tempRegB;

/** 
 * @brief IMU initialization routine.
 * 
 * @return A status code indicating the success of the subroutine.
 * 
 * This function performs IMU initialization tasks.
 **/
adi_imu_Status adi_imu_Init() 
{
    status = adi_imu_CheckComs();

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
    /* Prepare the tx buffer */
    txBuf[2] = (0x80 | (pageIDRegAddr & 0xFF));
    txBuf[3] = (val & 0xFF);
    txBuf[4] = (0x80 | ((pageIDRegAddr & 0xFF) + 1));
    txBuf[5] = ((val >> 8) & 0xFF);
    /* Transmit tx buffer */
    status = spi_Transfer(txBuf, rxBuf, 6, 16, STALL_TIME_US);
#else
    /* Prepare the tx buffer */
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
    /* Prepare the tx buffer */
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
 * @param timesToRead The number of times to read the register list
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
adi_imu_Status adi_imu_ReadRegArray(const uint16_t *regList, uint16_t *outData, uint16_t numRegs, uint16_t timesToRead)
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


/** 
 * @brief Executes the IMU flash memory backup routine.
 * 
 * @return A status code indicating the success of the SPI transaction
 * 
 * This function writes the correct bit into the IMU command register to execute the flash memory backup routine.
 * This routine stores the settings located in the IMU's volatile memory into non-volatile memory to be recalled
 * next time the device is restarted.   
 **/
adi_imu_Status adi_imu_FlashUpdate()
{
    /* Clear status flag */
    status = ADI_IMU_SUCCESS;
    /* Set the flash update bit in the command register */
    status = adi_imu_WriteReg(COMMAND_REG, BITM_COMMAND_REG_FLASH_MEM_UPD);
    /* Wait for the execution time specified in the datasheet */
    delay_MS(FLASH_MEMORY_BACKUP_TIME_MS);

#if CHECK_COMS_AFTER_COMMAND
    adi_imu_CheckComs();
#endif

    return status;
}

/** 
 * @brief Executes the IMU software reset routine.
 * 
 * @return A status code indicating the success of the SPI transaction.
 * 
 * This function writes the correct bit into the IMU command register to trigger a software reset. 
 **/
adi_imu_Status adi_imu_SoftwareReset()
{
    /* Clear status flag */
    status = ADI_IMU_SUCCESS;
    /* Set the software reset bit in the command register */
    status = adi_imu_WriteReg(COMMAND_REG, BITM_COMMAND_REG_SOFTWARE_RST);
    /* Wait for the execution time specified in the datasheet */
    delay_MS(RESET_RECOVERY_TIME_MS);

#if CHECK_COMS_AFTER_COMMAND
    adi_imu_CheckComs();
#endif

    return status;
}

/** 
 * @brief Sets the IMU data output rate.
 * 
 * @return A status code indicating the success of the SPI transaction.
 * 
 * This function performs the necessary calculations and writes the value to the decimation register.
 **/
adi_imu_Status adi_imu_SetDataRate(uint16_t dataRate)
{
    status = ADI_IMU_SUCCESS;
    /* Write the decimation setting to the part if it supports arbitrary rates */
#if SUPPORTS_ARBITRARY_DEC_RATE
    status = adi_imu_WriteReg(DECIMATE_REG, ((MAX_DATA_RATE / dataRate) - 1));
#else
    //TODO: Figure out what to do  with 2^n
#endif
    return status;
}

/** 
 * @brief Checks whether SPI communication with the IMU is operational.
 * 
 * @return A status code indicating the success of the subroutine
 * 
 * This function reads the contents of the first scratch register, temporarily stores it in memory,
 * writes a known value to the scratch register, reads that value back, and if successful, re-writes the
 * original value into the scratch register. 
 **/
adi_imu_Status adi_imu_CheckComs()
{
    status = ADI_IMU_SUCCESS;
    status = adi_imu_ReadReg(SCRATCH_REG, &tempRegA);
    if (status != ADI_IMU_SUCCESS)
    {
        return status;
    }
    status = adi_imu_WriteReg(SCRATCH_REG, 0xA5A5);
    if (status != ADI_IMU_SUCCESS)
    {
        return status;
    }
    status = adi_imu_ReadReg(SCRATCH_REG, &tempRegB);
    if (status != ADI_IMU_SUCCESS)
    {
        return status;
    }
    if (tempRegB != 0xA5A5)
    {
        return ADI_IMU_CHECK_SPI_COMS_FAILED;
    }
    status = adi_imu_WriteReg(SCRATCH_REG, tempRegA);

    return status;
}

/** 
 * @brief Get the IMU gyroscope range.
 * 
 * @return A status code indicating the success of the subroutine.
 * 
 * This function reads the range register if the IMU supports it.
 **/
#if SUPPORTS_RANGE_REG
static adi_imu_Status adi_imu_GetSensorRange(adi_imu_RangeReg *range_data)
{
    status = ADI_IMU_SUCCESS;
    status = adi_imu_ReadReg(RANGE_REG, &tempRegA);
    tempRegA = tempRegA & 0x000C;
    *range_data = tempRegA;

    return status;
}
#endif

/** 
 * @brief Gets the IMU metadata from several IMU registers.
 * 
 * @return A status code indicating the success of the subroutine.
 * 
 * This function reads the contents of several IMU registers and compiles them in a single enum.
 **/
adi_imu_Status adi_imu_GetDeviceInfo(adi_imu_DeviceInfo *data_info)
{
    status = ADI_IMU_SUCCESS;
    status = adi_imu_ReadReg(PRODUCT_ID_REG, &data_info->prodId);
    status = adi_imu_ReadReg(FIRMWARE_REV_REG, &data_info->fwRev);
    status = adi_imu_ReadReg(FIRMWARE_DATE_MONTH_REG, &data_info->fwDayMonth);
    status = adi_imu_ReadReg(FIRMWARE_YEAR_REG, &data_info->fwYear);
    status = adi_imu_ReadReg(SERIAL_NUMBER_REG, &data_info->serialNumber);
    status = adi_imu_ReadReg(DECIMATE_REG, &data_info->decRate);
#if SUPPORTS_PAGES
    status = adi_imu_ReadReg(PAGE_ID_REG, &data_info->activePageId);
#endif
#if SUPPORTS_RANGE_REG
    status = adi_imu_GetSensorRange(&data_info->range);
#endif

    return status;
}

/** 
 * @brief Sets the active IMU page if the IMU supports it.
 * 
 * @return A status code indicating the success of the subroutine.
 * 
 * This function sets the active IMU page.
 **/
#if SUPPORTS_PAGES
adi_imu_Status adi_imu_SetActivePage(uint16_t page)
{
    status = ADI_IMU_SUCCESS;
    status = adi_imu_WriteReg(PAGE_ID_REG, page);
    return status;
}
#endif

/** 
 * @brief Gets the active IMU page if the IMU supports it.
 * 
 * @return A status code indicating the success of the subroutine.
 * 
 * This function gets the active IMU page.
 **/
#if SUPPORTS_PAGES
adi_imu_Status adi_imu_GetActivePage(uint16_t *active_page)
{
    status = ADI_IMU_SUCCESS;
    status = adi_imu_ReadReg(PAGE_ID_REG, active_page);
    return status;
}
#endif

#if ENABLE_SCALED_DATA
adi_imu_Status adi_imu_GetScaledSensorData(adi_imu_ScaledData *data_struct)
{
    status = ADI_IMU_SUCCESS;
    adi_imu_UnscaledData data;
    status = adi_imu_GetSensorData(&data);
    #if SUPPORTS_32BIT & ENABLE_32BIT_DATA
        // use 32-bit scale factors

    #else
        // use 16-bit scale factors

    #endif

    return status;
}
#endif

adi_imu_Status adi_imu_GetSensorData(adi_imu_UnscaledData *data_struct)
{
    status = ADI_IMU_SUCCESS;
#if ENABLE_BURST_MODE
    #if ENABLE_32_BIT_BURST_MODE & SUPPORTS_32BIT_BURST
        // TODO: Do I need to write the page ID here?
        /* Build the tx array */
        txBuf[0] = (BURST_TRIGGER_REG & 0xFF);
        txBuf[1] = 0x00;
        for (uint16_t i = 0; i < BURST_BYTE_LENGTH; i++)
        {
            txBuf[i + 2] = 0x00;
        }
        /* Transmit txBuf and store the response in rxBuf */
        status = spi_Transfer(txBuf, rxBuf, (BURST_BYTE_LENGTH + 2), (BURST_BYTE_LENGTH + 2), STALL_TIME_US);
        /* Push the rxBuf data into the data output struct */
        #if SUPPORTS_BURST_STATUS
            data_struct->status = (uint32_t) (IMU_GET_16BITS(rxBuf, STATUS_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        #if SUPPORTS_BURST_CNT
            data_struct->count = (uint32_t) (IMU_GET_16BITS(rxBuf, COUNT_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        data_struct->xg = (int32_t) (IMU_GET_32BITS(rxBuf, XG_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->yg = (int32_t) (IMU_GET_32BITS(rxBuf, YG_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->zg = (int32_t) (IMU_GET_32BITS(rxBuf, ZG_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->xa = (int32_t) (IMU_GET_32BITS(rxBuf, XA_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->ya = (int32_t) (IMU_GET_32BITS(rxBuf, YA_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->za = (int32_t) (IMU_GET_32BITS(rxBuf, ZA_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->temperature = (int32_t) (IMU_GET_16BITS(rxBuf, TEMP_OUT_INDEX + BURST_PAYLOAD_OFFSET));
        #if ENABLE_MAGNETOMETER
            data_struct->xm = (int32_t) (IMU_GET_16BITS(rxBuf, XM_INDEX + BURST_PAYLOAD_OFFSET));
            data_struct->ym = (int32_t) (IMU_GET_16BITS(rxBuf, YM_INDEX + BURST_PAYLOAD_OFFSET));
            data_struct->zm = (int32_t) (IMU_GET_16BITS(rxBuf, ZM_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        #if ENABLE_BAROMETER
            data_struct->baro = (int32_t) (IMU_GET_16BITS(rxBuf, BARO_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        #if SUPPORTS_BURST_CHECKSUM_CRC
            data_struct->chksm_crc = (uint32_t) (IMU_GET_32BITS(rxBuf, CHECKSUM_INDEX + BURST_PAYLOAD_OFFSET));
        #endif

    #else
        // TODO: Do I need to write the page ID here?
        /* Build the tx array */
        txBuf[0] = (BURST_TRIGGER_REG & 0xFF);
        txBuf[1] = 0x00;
        for (uint16_t i = 0; i < BURST_BYTE_LENGTH; i++)
        {
            txBuf[i + 2] = 0x00;
        }
        /* Transmit txBuf and store the response in rxBuf */
        status = spi_Transfer(txBuf, rxBuf, (BURST_BYTE_LENGTH + 2), (BURST_BYTE_LENGTH + 2), STALL_TIME_US);
        /* Push the rxBuf data into the data output struct */
        #if SUPPORTS_BURST_STATUS
            data_struct->status = (uint32_t) (IMU_GET_16BITS(rxBuf, STATUS_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        #if SUPPORTS_BURST_CNT
            data_struct->count = (uint32_t) (IMU_GET_16BITS(rxBuf, COUNT_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        data_struct->xg = (int32_t) (IMU_GET_16BITS(rxBuf, XG_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->yg = (int32_t) (IMU_GET_16BITS(rxBuf, YG_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->zg = (int32_t) (IMU_GET_16BITS(rxBuf, ZG_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->xa = (int32_t) (IMU_GET_16BITS(rxBuf, XA_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->ya = (int32_t) (IMU_GET_16BITS(rxBuf, YA_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->za = (int32_t) (IMU_GET_16BITS(rxBuf, ZA_INDEX + BURST_PAYLOAD_OFFSET));
        data_struct->temperature = (int16_t) (IMU_GET_16BITS(rxBuf, TEMP_OUT_INDEX + BURST_PAYLOAD_OFFSET));
        #if ENABLE_MAGNETOMETER
            data_struct->xm = (int32_t) (IMU_GET_16BITS(rxBuf, XM_INDEX + BURST_PAYLOAD_OFFSET));
            data_struct->ym = (int32_t) (IMU_GET_16BITS(rxBuf, YM_INDEX + BURST_PAYLOAD_OFFSET));
            data_struct->zm = (int32_t) (IMU_GET_16BITS(rxBuf, ZM_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        #if ENABLE_BAROMETER
            data_struct->baro = (int32_t) (IMU_GET_16BITS(rxBuf, BARO_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
        #if SUPPORTS_BURST_CHECKSUM_CRC
            data_struct->chksm_crc = (uint32_t) (IMU_GET_16BITS(rxBuf, CHECKSUM_INDEX + BURST_PAYLOAD_OFFSET));
        #endif
    #endif
#else
    #if ENABLE_32BIT_DATA & SUPPORTS_32BIT_REGS
        //32-bit regular reads
    #else
        // 16-bit regular reads
    #endif
#endif

    return status;
}
