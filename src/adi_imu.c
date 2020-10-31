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

static volatile uint16_t activePage = 0;
static volatile adi_imu_Boolean pageValid = FALSE;

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

adi_imu_Status adi_imu_WriteReg(uint16_t pageIDRegAddr, uint16_t val)
{

#if SUPPORTS_PAGES
    /**
     * If the page stored in memory is not valid, get the active page and
     * update the activePage variable.
     **/
    if (pageValid == FALSE)
    {
        status = adi_imu_GetActivePage(activePage);
        if (status == ADI_IMU_SUCCESS)
        {
            pageValid = TRUE;
        }
        else
        {
            return status;
        }
    }

    /**
     * If the active page stored in memory is not the page requested in the
     * register map, change the page.
     **/
    if (activePage != ((pageIDRegAddr >> 8) & 0xFF))
    {
        status = adi_imu_SetActivePage(((pageIDRegAddr >> 8) & 0xFF));
        if (status != ADI_IMU_SUCCESS)
        {
            return status;
        }
    }

    /** 
     * Invalidate the stored page if writing a value other than the active 
     * page to the PageID register.
     **/
    if ((pageIDRegAddr & 0xFF) == PAGE_ID_REG)
    {
        if ((val & 0xFF) != activePage)
        {
            pageValid = FALSE;
        }
    }
#endif
    /* Prepare transmit buffer */
    txBuf[0] = ((0x80 | (pageIDRegAddr & 0xFF)) << 8);
    txBuf[1] = (val & 0xFF);
    txBuf[2] = ((0x80 | ((pageIDRegAddr & 0xFF) + 1)) << 8);
    txBuf[3] = ((val >> 8) & 0xFF);

    status = spi_Transfer(txBuf, rxBuf, 4, 16, STALL_TIME_US);

    return status;
}

adi_imu_Status adi_imu_ReadReg(uint16_t pageIDRegAddr, uint16_t *val)
{
    
#if SUPPORTS_PAGES
    /**
     * If the page stored in memory is not valid, get the active page and
     * update the activePage variable.
     **/
    if (pageValid == FALSE)
    {
        status = adi_imu_GetActivePage(activePage);
        if (status == ADI_IMU_SUCCESS)
        {
            pageValid = TRUE;
        }
        else
        {
            return status;
        }
    }


    /**
     * If the active page stored in memory is not the page requested in the
     * register map, change the page.
     **/
    if (activePage != ((pageIDRegAddr >> 8) & 0xFF))
    {
        status = adi_imu_SetActivePage(((pageIDRegAddr >> 8) & 0xFF));
        if (status != ADI_IMU_SUCCESS)
        {
            return status;
        }
    }
#endif

    /* Prepare the tx buffer */
    txBuf[0] = ((pageIDRegAddr & 0xFF) << 8);
    txBuf[1] = 0x00;
    txBuf[2] = 0x00;
    txBuf[3] = 0x00;

    /* Transmit buffer data */
    status = spi_Transfer(txBuf, rxBuf, 4, 16, STALL_TIME_US);

    /* Combine bytes into word response */
    *val = ((rxBuf[2] << 8) | rxBuf[3]);

    return status;

}


/** 
 * This function assumes that all registers are being read from the same 
 * page as the first register in the list!
 **/
adi_imu_Status adi_imu_ReadRegArray(const uint16_t *regList, uint16_t *outData, uint16_t numRegs)
{
#if SUPPORTS_PAGES
    /**
     * If the page stored in memory is not valid, get the active page and
     * update the activePage variable.
     **/
    if (pageValid == FALSE)
    {
        status = adi_imu_GetActivePage(activePage);
        if (status == ADI_IMU_SUCCESS)
        {
            pageValid = TRUE;
        }
        else
        {
            return status;
        }
    }


    /**
     * If the active page stored in memory is not the page requested in the
     * register map, change the page.
     **/
    if (activePage != ((regList[0] >> 8) & 0xFF))
    {
        status = adi_imu_SetActivePage(((regList[0] >> 8) & 0xFF));
        if (status != ADI_IMU_SUCCESS)
        {
            return status;
        }
    }
#endif

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

}

adi_imu_Status adi_imu_FlashUpdate()
{

}

adi_imu_Status adi_imu_SoftwareReset()
{

}

adi_imu_Status adi_imu_SetDataRate(uint16_t dataRate)
{

}

adi_imu_Status adi_imu_CheckComs()
{

}

adi_imu_Status adi_imu_GetDeviceInfo(adi_imu_DeviceInfo *data_info)
{

}

adi_imu_Status adi_imu_SetActivePage(uint16_t page)
{

}

adi_imu_Status adi_imu_GetActivePage(uint16_t *active_page)
{

}

adi_imu_Status adi_imu_GetScaledSensorData(adi_imu_ScaledData *data_struct)
{

}

adi_imu_Status adi_imu_GetSensorData(adi_imu_UnscaledData *data_struct)
{

}
