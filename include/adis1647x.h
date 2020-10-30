/**
  * @file		  adis1647x.h
  * @date		  10/29/2020
  * @author		Juan Chong (juan.chong@analog.com)
  * @brief		ADIS1647x register map and configuration file.
 **/

#include "adi_imu.h"
#include "adi_imu_conf.h"

/* Device and family-specific definitions and configurations */
#define ENABLE_MAGNETOMETER                     0
#define ENABLE_BAROMETER                        0
#define SUPPORTS_32BIT                          1
#define SUPPORTS_BURST                          1
#define SUPPORTS_PAGED_ADDRESSES                0
#define MAX_DATA_RATE                           2000
#define SUPPORTS_PPS                            1
#define SUPPORTS_EXTERNAL_SYNC                  1
#define SUPPORTS_ARBITRARY_DEC_RATE             1
#define SUPPORTS_RANGE_REG                      1

/* Component-specific register locations */
#define SCRATCH_REG                             USER_SCR1
#define FIRMWARE_REV_REG                        FIRM_REV
#define FIRMWARE_DATE_MONTH_REG                 FIRM_DM
#define FIRMWARE_YEAR_REG                       FIRM_Y
#define PRODUCT_ID_REG                          PROD_ID
#define SERIAL_NUMBER_REG                       SERIAL_NUM
#define COMMAND_REG                             GLOB_CMD
#define MISC_CTRL_REG                           MSC_CTRL
#define DECIMATE_REG                            DEC_RATE
#if SUPPORTS_PAGED_ADDRESSES
  #define PAGE_ID_REG                           PAGE_ID
#endif
#if SUPPORTS_RANGE_REG
  #define RANGE_REG                             RANG_MDL
#endif

/* Component-specific scale factors */
#define GYRO_16BIT_SCALE_125                    (float) 160
#define GYRO_16BIT_SCALE_500                    (float) 40
#define GYRO_16BIT_SCALE_2000                   (float) 10
#define ACCEL_16BIT_SCALE_8G                    (float) 4000
#define ACCEL_16BIT_SCALE_40G                   (float) 800

#define GYRO_32BIT_SCALE_125                    (float) 10485760
#define GYRO_32BIT_SCALE_500                    (float) 2621440
#define GYRO_32BIT_SCALE_2000                   (float) 655360
#define ACCEL_32BIT_SCALE_8G                    (float) 262144000
#define ACCEL_32BIT_SCALE_40G                   (float) 52428800

/* Burst mode-specific definitions */
#if ENABLE_BURST_MODE
  #if SUPPORTS_BURST
    #define BURST_TRIGGER_REG                   GLOB_CMD
    #define BURST_BYTE_LENGTH                   20

    /* Burst message definition */
    #define DIAG_STAT_INDEX                     0
    #define XG_INDEX                            1
    #define YG_INDEX                            2
    #define ZG_INDEX                            3
    #define XA_INDEX                            4
    #define YA_INDEX                            5
    #define ZA_INDEX                            6
    #define TEMP_OUT_INDEX                      7
    #define DATA_CNTR_INDEX                     8
    #define CHECKSUM_INDEX                      9
  #endif
#endif

/* Misc. control register bit definitions */
#define BITP_MISC_CTRL_REG_LIN_G_COMP           7
#define BITP_MISC_CTRL_REG_POP_COMP             6
#define BITP_MISC_CTRL_REG_SYNC_FUNCTION        2
#define BITP_MISC_CTRL_REG_SYNC_POLARITY        1
#define BITP_MISC_CTRL_REG_DR_POLARITY          0
#define BITM_MISC_CTRL_REG_LIN_G_COMP           (1 << BITP_MISC_CTRL_REG_LIN_G_COMP)
#define BITM_MISC_CTRL_REG_POP_COMP             (1 << BITP_MISC_CTRL_REG_POP_COMP)
#define BITM_MISC_CTRL_REG_SYNC_FUNCTION        (3 << BITP_MISC_CTRL_REG_SYNC_FUNCTION)
#define BITM_MISC_CTRL_REG_SYNC_POLARITY        (1 << BITP_MISC_CTRL_REG_SYNC_POLARITY)
#define BITM_MISC_CTRL_REG_DR_POLARITY          (1 << BITP_MISC_CTRL_REG_DR_POLARITY)

/* Command register bit definitions */
#define BITP_COMMAND_REG_SOFTWARE_RST           7
#define BITP_COMMAND_REG_FLASH_MEM_TEST         4
#define BITP_COMMAND_REG_FLASH_MEM_UPD          3
#define BITP_COMMAND_REG_SELF_TEST              2
#define BITP_COMMAND_REG_CLR_USR_CALIB          1
#define BITP_COMMAND_REG_BIAS_CORR_UPD          0
#define BITM_COMMAND_REG_SOFTWARE_RST           (1 << BITP_COMMAND_REG_SOFTWARE_RST)
#define BITM_COMMAND_REG_FLASH_MEM_TEST         (1 << BITP_COMMAND_REG_FLASH_MEM_TEST)
#define BITM_COMMAND_REG_FLASH_MEM_UPD          (1 << BITP_COMMAND_REG_FLASH_MEM_UPD)
#define BITM_COMMAND_REG_SELF_TEST              (1 << BITP_COMMAND_REG_SELF_TEST)
#define BITM_COMMAND_REG_CLR_USR_CALIB          (1 << BITP_COMMAND_REG_CLR_USR_CALIB)
#define BITM_COMMAND_REG_BIAS_CORR_UPD          (1 << BITP_COMMAND_REG_BIAS_CORR_UPD)


/* [15:8] = page id, [7:0] = reg addr */
/* Example: TEMP_OUT 0x001C ->   0x   00  1C
                                      ^   ^
                                      |   |   
                                      |   This byte is the register address
                                      This byte is the page ID
*/
/* NOTE: Use 0x00 for the page ID if the IMU does not support paged addresses. */

/* User Register Memory Map from Table 6 */
#define DIAG_STAT   	0x0002  /* Diagnostic and operational status */
#define X_GYRO_LOW  	0x0004  /* X-axis gyroscope output, lower word */
#define X_GYRO_OUT  	0x0006  /* X-axis gyroscope output, upper word */
#define Y_GYRO_LOW  	0x0008  /* Y-axis gyroscope output, lower word */
#define Y_GYRO_OUT  	0x000A  /* Y-axis gyroscope output, upper word */
#define Z_GYRO_LOW  	0x000C  /* Z-axis gyroscope output, lower word */
#define Z_GYRO_OUT  	0x000E  /* Z-axis gyroscope output, upper word */
#define X_ACCL_LOW  	0x0010  /* X-axis accelerometer output, lower word */
#define X_ACCL_OUT  	0x0012  /* X-axis accelerometer output, upper word */
#define Y_ACCL_LOW  	0x0014  /* Y-axis accelerometer output, lower word */
#define Y_ACCL_OUT  	0x0016  /* Y-axis accelerometer output, upper word */
#define Z_ACCL_LOW  	0x0018  /* Z-axis accelerometer output, lower word */
#define Z_ACCL_OUT  	0x001A  /* Z-axis accelerometer output, upper word */
#define TEMP_OUT    	0x001C  /* Temperature output (internal, not calibrated) */
#define TIME_STAMP  	0x001E  /* PPS mode time stamp */
#define DATA_CNTR     0x0022  /* New data counter */
#define X_DELTANG_LOW	0x0024  /* X-axis delta angle output, lower word */
#define X_DELTANG_OUT	0x0026  /* X-axis delta angle output, upper word */
#define Y_DELTANG_LOW	0x0028  /* Y-axis delta angle output, lower word */
#define Y_DELTANG_OUT	0x002A  /* Y-axis delta angle output, upper word */
#define Z_DELTANG_LOW	0x002C  /* Z-axis delta angle output, lower word */
#define Z_DELTANG_OUT	0x002E  /* Z-axis delta angle output, upper word */
#define X_DELTVEL_LOW	0x0030  /* X-axis delta velocity output, lower word */
#define X_DELTVEL_OUT	0x0032  /* X-axis delta velocity output, upper word */
#define Y_DELTVEL_LOW	0x0034  /* Y-axis delta velocity output, lower word */
#define Y_DELTVEL_OUT	0x0036  /* Y-axis delta velocity output, upper word */
#define Z_DELTVEL_LOW	0x0038  /* Z-axis delta velocity output, lower word */
#define Z_DELTVEL_OUT	0x003A  /* Z-axis delta velocity output, upper word */
#define XG_BIAS_LOW		0x0040  /* X-axis gyroscope bias offset correction, lower word */
#define XG_BIAS_HIGH	0x0042  /* X-axis gyroscope bias offset correction, upper word */
#define YG_BIAS_LOW		0x0044  /* Y-axis gyroscope bias offset correction, lower word */
#define YG_BIAS_HIGH	0x0046  /* Y-axis gyroscope bias offset correction, upper word */
#define ZG_BIAS_LOW		0x0048  /* Z-axis gyroscope bias offset correction, lower word */
#define ZG_BIAS_HIGH	0x004A  /* Z-axis gyroscope bias offset correction, upper word */
#define XA_BIAS_LOW		0x004C  /* X-axis accelerometer bias offset correction, lower word */
#define XA_BIAS_HIGH	0x004E  /* X-axis accelerometer bias offset correction, upper word */
#define YA_BIAS_LOW		0x0050  /* Y-axis accelerometer bias offset correction, lower word */
#define YA_BIAS_HIGH	0x0052  /* Y-axis accelerometer bias offset correction, upper word */
#define ZA_BIAS_LOW		0x0054  /* Z-axis accelerometer bias offset correction, lower word */
#define ZA_BIAS_HIGH	0x0056  /* Z-axis accelerometer bias offset correction, upper word */
#define FILT_CTRL    	0x005C  /* Control, Bartlett window filter configuration */
#define RANG_MDL      0x005E  /* Measurement range/model-specific identifier */
#define MSC_CTRL    	0x0060  /* Miscellaneous control */
#define UP_SCALE    	0x0062  /* Clock scale factor, PPS mode */
#define DEC_RATE    	0x0064  /* Decimation rate control (output data rate) */
#define NULL_CFG    	0x0066  /* Auto-null configuration control */
#define GLOB_CMD    	0x0068  /* Global commands */
#define FIRM_REV    	0x006C  /* Firmware revision */
#define FIRM_DM    		0x006E  /* Firmware revision date, month and day */
#define FIRM_Y    		0x0070  /* Firmware revision date, year */
#define PROD_ID    		0x0072  /* Product identification  */
#define SERIAL_NUM    0x0074  /* Serial number (relative to assembly lot) */
#define USER_SCR1    	0x0076  /* User scratch register 1 */
#define USER_SCR2    	0x0078  /* User scratch register 2 */
#define USER_SCR3    	0x007A  /* User scratch register 3 */
#define FLSHCNT_LOW   0x007C  /* Flash update count, lower word */
#define FLSHCNT_HIGH  0x007E  /* Flash update count, upper word */
