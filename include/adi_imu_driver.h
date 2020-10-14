////////////////////////////////////////////////////////////////////////////////////
//  October 2020
//  Author: Juan Jose Chong <juan@highcurrent.io>
////////////////////////////////////////////////////////////////////////////////////
//  adi_imu_driver.h
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

#ifndef __ADI_IMU_DRIVER_H_
#define __ADI_IMU_DRIVER_H_

#define IMU_GET_16BITS(buf, idx)        ( ((buf[idx] << 8) & 0xFF00) | (buf[1+idx] & 0xFF) )
#define IMU_GET_32BITS(buf, idx)        ( (uint32_t)((buf[2+idx] << 24) & 0xFF000000) | (uint32_t)((buf[3+idx] << 16) & 0xFF0000) | (uint32_t)((buf[idx] << 8) & 0xFF00) | (uint32_t)(buf[1+idx] & 0xFF) )



#endif