////////////////////////////////////////////////////////////////////////////////////
//  October 2020
//  Author: Juan Jose Chong <juan@highcurrent.io>
////////////////////////////////////////////////////////////////////////////////////
//  adcmxl3021_regmap.h
////////////////////////////////////////////////////////////////////////////////////
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
//  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
//  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////////

/* #define [15:8] = page id, [7:0] = reg addr */

#define     REG_PAGE_ID                 0x0000
#define     REG_TEMP_OUT                0x0004
#define     REG_SUPPLY_OUT              0x0006
#define     REG_FFT_AVG1                0x0008
#define     REG_FFT_AVG2                0x000A
#define     REG_BUF_PNTR                0x000C
#define     REG_REC_PNTR                0x000E
#define     REG_X_BUF                   0x0010
#define     REG_Y_BUF                   0x0012
#define     REG_Z_BUF_RSS_BUFF          0x0014
#define     REG_X_ANULL                 0x0016
#define     REG_Y_ANULL                 0x0018
#define     REG_Z_ANULL                 0x001A
#define     REG_REC_CTRL                0x001C
#define     REG_REC_PRD                 0x001E
#define     REG_ALM_F_LOW               0x0020
#define     REG_ALM_F_HIGH              0x0022
#define     REG_ALM_X_MAG1              0x0024
#define     REG_ALM_Y_MAG1              0x0026
#define     REG_ALM_Z_MAG1_ALM_RSS1     0x0028
#define     REG_ALM_X_MAG2              0x002A
#define     REG_ALM_Y_MAG2              0x002C
#define     REG_ALM_Z_MAG2_ALM_RSS2     0x002E
#define     REG_ALM_PNTR                0x0030
#define     REG_ALM_S_MAG               0x0032
#define     REG_ALM_CTRL                0x0034
#define     REG_DIO_CTRL                0x0036
#define     REG_FILT_CTRL               0x0038
#define     REG_AVG_CNT                 0x003A
#define     REG_DIAG_STAT               0x003C
#define     REG_GLOB_CMD                0x003E
#define     REG_ALM_X_STAT              0x0040
#define     REG_ALM_Y_STAT              0x0042
#define     REG_ALM_Z_STAT_ALM_RSS_STAT 0x0044
#define     REG_ALM_X_PEAK              0x0046
#define     REG_ALM_Y_PEAK              0x0048
#define     REG_ALM_Z_PEAK_ALM_RSS_PEAK 0x004A
#define     REG_TIME_STAMP_L            0x004C
#define     REG_TIME_STAMP_H            0x004E
#define     REG_REV_DAY                 0x0052
#define     REG_YEAR_MON                0x0054
#define     REG_PROD_ID                 0x0056
#define     REG_SERIAL_NUM              0x0058
#define     REG_USER_SCRATCH            0x005A
#define     REG_REC_FLASH_CNT           0x005C
#define     REG_MISC_CTRL               0x0064
#define     REG_REC_INFO1               0x0066
#define     REG_REC_INFO2               0x0068
#define     REG_REC_CNTR                0x006A
#define     REG_ALM_X_FREQ              0x006C
#define     REG_ALM_Y_FREQ              0x006E
#define     REG_ALM_Z_FREQ              0x0070
#define     REG_STAT_PNTR               0x0072
#define     REG_X_STATISTIC             0x0074
#define     REG_Y_STATISTIC             0x0076
#define     REG_Z_STATISTIC             0x0078
#define     REG_FUND_FREQ               0x007A
#define     REG_FLASH_CNT_L             0x007C
#define     REG_FLASH_CNT_U             0x007E

#define     FIR_COEF_A00                0x0102
#define     FIR_COEF_A01                0x0104
#define     FIR_COEF_A02                0x0106
#define     FIR_COEF_A03                0x0108
#define     FIR_COEF_A04                0x010A
#define     FIR_COEF_A05                0x010C
#define     FIR_COEF_A06                0x010E
#define     FIR_COEF_A07                0x0110
#define     FIR_COEF_A08                0x0112
#define     FIR_COEF_A09                0x0114
#define     FIR_COEF_A10                0x0116
#define     FIR_COEF_A11                0x0118
#define     FIR_COEF_A12                0x011A
#define     FIR_COEF_A13                0x011C
#define     FIR_COEF_A14                0x011E
#define     FIR_COEF_A15                0x0120
#define     FIR_COEF_A16                0x0122
#define     FIR_COEF_A17                0x0124
#define     FIR_COEF_A18                0x0126
#define     FIR_COEF_A19                0x0128
#define     FIR_COEF_A20                0x012A
#define     FIR_COEF_A21                0x012C
#define     FIR_COEF_A22                0x012E
#define     FIR_COEF_A23                0x0130
#define     FIR_COEF_A24                0x0132
#define     FIR_COEF_A25                0x0134
#define     FIR_COEF_A26                0x0136
#define     FIR_COEF_A27                0x0138
#define     FIR_COEF_A28                0x013A
#define     FIR_COEF_A29                0x013C
#define     FIR_COEF_A30                0x013E
#define     FIR_COEF_A31                0x0140

#define     FIR_COEF_B00                0x0202
#define     FIR_COEF_B01                0x0204
#define     FIR_COEF_B02                0x0206
#define     FIR_COEF_B03                0x0208
#define     FIR_COEF_B04                0x020A
#define     FIR_COEF_B05                0x020C
#define     FIR_COEF_B06                0x020E
#define     FIR_COEF_B07                0x0210
#define     FIR_COEF_B08                0x0212
#define     FIR_COEF_B09                0x0214
#define     FIR_COEF_B10                0x0216
#define     FIR_COEF_B11                0x0218
#define     FIR_COEF_B12                0x021A
#define     FIR_COEF_B13                0x021C
#define     FIR_COEF_B14                0x021E
#define     FIR_COEF_B15                0x0220
#define     FIR_COEF_B16                0x0222
#define     FIR_COEF_B17                0x0224
#define     FIR_COEF_B18                0x0226
#define     FIR_COEF_B19                0x0228
#define     FIR_COEF_B20                0x022A
#define     FIR_COEF_B21                0x022C
#define     FIR_COEF_B22                0x022E
#define     FIR_COEF_B23                0x0230
#define     FIR_COEF_B24                0x0232
#define     FIR_COEF_B25                0x0234
#define     FIR_COEF_B26                0x0236
#define     FIR_COEF_B27                0x0238
#define     FIR_COEF_B28                0x023A
#define     FIR_COEF_B29                0x023C
#define     FIR_COEF_B30                0x023E
#define     FIR_COEF_B31                0x0240

#define     FIR_COEF_C00                0x0302
#define     FIR_COEF_C01                0x0304
#define     FIR_COEF_C02                0x0306
#define     FIR_COEF_C03                0x0308
#define     FIR_COEF_C04                0x030A
#define     FIR_COEF_C05                0x030C
#define     FIR_COEF_C06                0x030E
#define     FIR_COEF_C07                0x0310
#define     FIR_COEF_C08                0x0312
#define     FIR_COEF_C09                0x0314
#define     FIR_COEF_C10                0x0316
#define     FIR_COEF_C11                0x0318
#define     FIR_COEF_C12                0x031A
#define     FIR_COEF_C13                0x031C
#define     FIR_COEF_C14                0x031E
#define     FIR_COEF_C15                0x0320
#define     FIR_COEF_C16                0x0322
#define     FIR_COEF_C17                0x0324
#define     FIR_COEF_C18                0x0326
#define     FIR_COEF_C19                0x0328
#define     FIR_COEF_C20                0x032A
#define     FIR_COEF_C21                0x032C
#define     FIR_COEF_C22                0x032E
#define     FIR_COEF_C23                0x0330
#define     FIR_COEF_C24                0x0332
#define     FIR_COEF_C25                0x0334
#define     FIR_COEF_C26                0x0336
#define     FIR_COEF_C27                0x0338
#define     FIR_COEF_C28                0x033A
#define     FIR_COEF_C29                0x033C
#define     FIR_COEF_C30                0x033E
#define     FIR_COEF_C31                0x0340

#define     FIR_COEF_D00                0x0402
#define     FIR_COEF_D01                0x0404
#define     FIR_COEF_D02                0x0406
#define     FIR_COEF_D03                0x0408
#define     FIR_COEF_D04                0x040A
#define     FIR_COEF_D05                0x040C
#define     FIR_COEF_D06                0x040E
#define     FIR_COEF_D07                0x0410
#define     FIR_COEF_D08                0x0412
#define     FIR_COEF_D09                0x0414
#define     FIR_COEF_D10                0x0416
#define     FIR_COEF_D11                0x0418
#define     FIR_COEF_D12                0x041A
#define     FIR_COEF_D13                0x041C
#define     FIR_COEF_D14                0x041E
#define     FIR_COEF_D15                0x0420
#define     FIR_COEF_D16                0x0422
#define     FIR_COEF_D17                0x0424
#define     FIR_COEF_D18                0x0426
#define     FIR_COEF_D19                0x0428
#define     FIR_COEF_D20                0x042A
#define     FIR_COEF_D21                0x042C
#define     FIR_COEF_D22                0x042E
#define     FIR_COEF_D23                0x0430
#define     FIR_COEF_D24                0x0432
#define     FIR_COEF_D25                0x0434
#define     FIR_COEF_D26                0x0436
#define     FIR_COEF_D27                0x0438
#define     FIR_COEF_D28                0x043A
#define     FIR_COEF_D29                0x043C
#define     FIR_COEF_D30                0x043E
#define     FIR_COEF_D31                0x0440

#define     FIR_COEF_E00                0x0502
#define     FIR_COEF_E01                0x0504
#define     FIR_COEF_E02                0x0506
#define     FIR_COEF_E03                0x0508
#define     FIR_COEF_E04                0x050A
#define     FIR_COEF_E05                0x050C
#define     FIR_COEF_E06                0x050E
#define     FIR_COEF_E07                0x0510
#define     FIR_COEF_E08                0x0512
#define     FIR_COEF_E09                0x0514
#define     FIR_COEF_E10                0x0516
#define     FIR_COEF_E11                0x0518
#define     FIR_COEF_E12                0x051A
#define     FIR_COEF_E13                0x051C
#define     FIR_COEF_E14                0x051E
#define     FIR_COEF_E15                0x0520
#define     FIR_COEF_E16                0x0522
#define     FIR_COEF_E17                0x0524
#define     FIR_COEF_E18                0x0526
#define     FIR_COEF_E19                0x0528
#define     FIR_COEF_E20                0x052A
#define     FIR_COEF_E21                0x052C
#define     FIR_COEF_E22                0x052E
#define     FIR_COEF_E23                0x0530
#define     FIR_COEF_E24                0x0532
#define     FIR_COEF_E25                0x0534
#define     FIR_COEF_E26                0x0536
#define     FIR_COEF_E27                0x0538
#define     FIR_COEF_E28                0x053A
#define     FIR_COEF_E29                0x053C
#define     FIR_COEF_E30                0x053E
#define     FIR_COEF_E31                0x0540

#define     FIR_COEF_F00                0x0602
#define     FIR_COEF_F01                0x0604
#define     FIR_COEF_F02                0x0606
#define     FIR_COEF_F03                0x0608
#define     FIR_COEF_F04                0x060A
#define     FIR_COEF_F05                0x060C
#define     FIR_COEF_F06                0x060E
#define     FIR_COEF_F07                0x0610
#define     FIR_COEF_F08                0x0612
#define     FIR_COEF_F09                0x0614
#define     FIR_COEF_F10                0x0616
#define     FIR_COEF_F11                0x0618
#define     FIR_COEF_F12                0x061A
#define     FIR_COEF_F13                0x061C
#define     FIR_COEF_F14                0x061E
#define     FIR_COEF_F15                0x0620
#define     FIR_COEF_F16                0x0622
#define     FIR_COEF_F17                0x0624
#define     FIR_COEF_F18                0x0626
#define     FIR_COEF_F19                0x0628
#define     FIR_COEF_F20                0x062A
#define     FIR_COEF_F21                0x062C
#define     FIR_COEF_F22                0x062E
#define     FIR_COEF_F23                0x0630
#define     FIR_COEF_F24                0x0632
#define     FIR_COEF_F25                0x0634
#define     FIR_COEF_F26                0x0636
#define     FIR_COEF_F27                0x0638
#define     FIR_COEF_F28                0x063A
#define     FIR_COEF_F29                0x063C
#define     FIR_COEF_F30                0x063E
#define     FIR_COEF_F31                0x0640