/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *    motion_sensor_custom.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This Module is for motion sensor driver.
 *
 * Author:
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef _MOTION_SENSOR_CUSTOM_H
#define _MOTION_SENSOR_CUSTOM_H

#ifdef __CUST_NEW__

extern const char gpio_ms_i2c_clk_pin;
extern const char gpio_ms_i2c_data_pin;
#define MS_SCL	  gpio_ms_i2c_clk_pin
#define MS_SDA	  gpio_ms_i2c_data_pin

#else	//__CUST_NEW__
#define MS_SCL	55
#define	MS_SDA	56
#endif	//__CUST_NEW__

/******************************************************
	Motion sensor controller register macro define
*******************************************************/
#define         C_BMA2XX_Zero_U8X                       (unsigned char)0
#define         C_BMA2XX_One_U8X                        (unsigned char)1
#define         C_BMA2XX_Two_U8X                        (unsigned char)2
#define         C_BMA2XX_Three_U8X                      (unsigned char)3
#define         C_BMA2XX_Four_U8X                       (unsigned char)4
#define         C_BMA2XX_Five_U8X                       (unsigned char)5
#define         C_BMA2XX_Six_U8X                        (unsigned char)6
#define         C_BMA2XX_Seven_U8X                      (unsigned char)7
#define         C_BMA2XX_Eight_U8X                      (unsigned char)8
#define         C_BMA2XX_Nine_U8X                       (unsigned char)9
#define         C_BMA2XX_Twelve_U8X                     (unsigned char)12
#define         C_BMA2XX_Fifteen_U8X                    (unsigned char)15
#define         C_BMA2XX_Sixteen_U8X                    (unsigned char)16
#define         C_BMA2XX_ThirtyTwo_U8X                  (unsigned char)32

/*
 *       SMB380 API error codes
 */
#define E_SMB_NULL_PTR          (char)-127
#define E_COMM_RES              (char)-1
#define E_OUT_OF_RANGE          (char)-2
#define E_EEPROM_BUSY           (char)-3
#define BMA2XX_RETURN_FUNCTION_TYPE        int   /**< This refers BMA2XX return type as char */

#define MS_CLK_PIN_GPIO_MODE		GPIO_ModeSetup(MS_SCL,0)
#define	MS_DATA_PIN_GPIO_MODE		GPIO_ModeSetup(MS_SDA,0)
#define MS_I2C_CLK_OUTPUT			GPIO_InitIO(OUTPUT,MS_SCL)
#define MS_I2C_DATA_OUTPUT			GPIO_InitIO(OUTPUT,MS_SDA)
#define MS_I2C_DATA_INPUT		   	GPIO_InitIO(INPUT,MS_SDA)
#define MS_I2C_CLK_HIGH				GPIO_WriteIO(1,MS_SCL)
#define MS_I2C_CLK_LOW				GPIO_WriteIO(0,MS_SCL)
#define MS_I2C_DATA_HIGH			GPIO_WriteIO(1,MS_SDA)
#define MS_I2C_DATA_LOW				GPIO_WriteIO(0,MS_SDA)
#define MS_I2C_GET_BIT				GPIO_ReadIO(MS_SDA)

/******************************************************
	I2C header (slaver address|W/R)   macro define
*******************************************************/
#define MS_SLAVEADDR_W	0x30	//0x0001 0110  BMA222E 223 250 250E 280 address all is 0x18
#define MS_SLAVEADDR_R	0x31	//0x0001 0111

#define BMA222E_CHIP_ID     0xF8 
#define BMA223_CHIP_ID      0xF8
#define BMA250E_CHIP_ID     0xF9
#define BMA280_CHIP_ID      0xFB

/*register definition*/
#define BMA2XX_EEP_OFFSET                       0x16
#define BMA2XX_IMAGE_BASE                       0x38
#define BMA2XX_IMAGE_LEN                        22

#define BMA2XX_CHIP_ID_REG                      0x00
#define BMA2XX_X_AXIS_LSB_REG                   0x02
#define BMA2XX_X_AXIS_MSB_REG                   0x03
#define BMA2XX_Y_AXIS_LSB_REG                   0x04
#define BMA2XX_Y_AXIS_MSB_REG                   0x05
#define BMA2XX_Z_AXIS_LSB_REG                   0x06
#define BMA2XX_Z_AXIS_MSB_REG                   0x07
#define BMA2XX_STATUS1_REG                      0x09
#define BMA2XX_STATUS2_REG                      0x0A
#define BMA2XX_STATUS_TAP_SLOPE_REG             0x0B
#define BMA2XX_STATUS_ORIENT_HIGH_REG           0x0C
#define BMA2XX_STATUS_FIFO_REG                  0x0E         
#define BMA2XX_RANGE_SEL_REG                    0x0F
#define BMA2XX_BW_SEL_REG                       0x10

#define BMA2XX_MODE_CTRL_REG                    0x11

#define BMA2XX_LOW_NOISE_CTRL_REG               0x12

#define BMA2XX_DATA_CTRL_REG                    0x13

#define BMA2XX_RESET_REG                        0x14
#define BMA2XX_INT_ENABLE1_REG                  0x16
#define BMA2XX_INT_ENABLE2_REG                  0x17
#define BMA2XX_INT_SLO_NO_MOT_REG               0x18     
#define BMA2XX_INT1_PAD_SEL_REG                 0x19
#define BMA2XX_INT_DATA_SEL_REG                 0x1A
#define BMA2XX_INT2_PAD_SEL_REG                 0x1B
#define BMA2XX_INT_SRC_REG                      0x1E
#define BMA2XX_INT_SET_REG                      0x20
#define BMA2XX_INT_CTRL_REG                     0x21
#define BMA2XX_LOW_DURN_REG                     0x22
#define BMA2XX_LOW_THRES_REG                    0x23
#define BMA2XX_LOW_HIGH_HYST_REG                0x24
#define BMA2XX_HIGH_DURN_REG                    0x25
#define BMA2XX_HIGH_THRES_REG                   0x26
#define BMA2XX_SLOPE_DURN_REG                   0x27
#define BMA2XX_SLOPE_THRES_REG                  0x28
#define BMA2XX_SLO_NO_MOT_THRES_REG             0x29
#define BMA2XX_TAP_PARAM_REG                    0x2A
#define BMA2XX_TAP_THRES_REG                    0x2B
#define BMA2XX_ORIENT_PARAM_REG                 0x2C
#define BMA2XX_THETA_BLOCK_REG                  0x2D
#define BMA2XX_THETA_FLAT_REG                   0x2E
#define BMA2XX_FLAT_HOLD_TIME_REG               0x2F

#define BMA2XX_FIFO_WML_TRIG                    0x30   
#define BMA2XX_SELF_TEST_REG                    0x32
#define BMA2XX_EEPROM_CTRL_REG                  0x33
#define BMA2XX_SERIAL_CTRL_REG                  0x34
#define BMA2XX_OFFSET_CTRL_REG                  0x36
#define BMA2XX_OFFSET_PARAMS_REG                0x37
#define BMA2XX_OFFSET_X_AXIS_REG                0x38
#define BMA2XX_OFFSET_Y_AXIS_REG                0x39
#define BMA2XX_OFFSET_Z_AXIS_REG                0x3A
#define BMA2XX_GP0_REG                          0x3B
#define BMA2XX_GP1_REG                          0x3C
#define BMA2XX_FIFO_MODE_REG                    0x3E
#define BMA2XX_FIFO_DATA_OUTPUT_REG             0x3F

#define E_BMA2XX_NULL_PTR                       (char)-127
#define BMA2XX_NULL                              0

/* register write and read delays */
#define BMA2XX_MDELAY_DATA_TYPE                 unsigned int
#define BMA2XX_EE_W_DELAY                       28       /* delay after EEP write is 28 msec */
/** bma2x2 acceleration data
        \brief Structure containing acceleration values for x,y and z-axis in signed short*/

/**************************************0x00******************************************/
#define BMA2XX_CHIP_ID__POS              0
#define BMA2XX_CHIP_ID__MSK              0xFF
#define BMA2XX_CHIP_ID__LEN              8
#define BMA2XX_CHIP_ID__REG              BMA2XX_CHIP_ID_REG

/**************************************0x02*****************************************/
/* DATA REGISTERS */
#define BMA2XX_NEW_DATA_X__POS           0
#define BMA2XX_NEW_DATA_X__LEN           1
#define BMA2XX_NEW_DATA_X__MSK           0x01
#define BMA2XX_NEW_DATA_X__REG           BMA2XX_X_AXIS_LSB_REG

#define BMA2XX_ACC_X14_LSB__POS          2
#define BMA2XX_ACC_X14_LSB__LEN          6
#define BMA2XX_ACC_X14_LSB__MSK          0xFC
#define BMA2XX_ACC_X14_LSB__REG          BMA2XX_X_AXIS_LSB_REG

#define BMA2XX_ACC_X12_LSB__POS          4
#define BMA2XX_ACC_X12_LSB__LEN          4
#define BMA2XX_ACC_X12_LSB__MSK          0xF0
#define BMA2XX_ACC_X12_LSB__REG          BMA2XX_X_AXIS_LSB_REG

#define BMA2XX_ACC_X10_LSB__POS          6
#define BMA2XX_ACC_X10_LSB__LEN          2
#define BMA2XX_ACC_X10_LSB__MSK          0xC0
#define BMA2XX_ACC_X10_LSB__REG          BMA2XX_X_AXIS_LSB_REG

#define BMA2XX_ACC_X8_LSB__POS           0
#define BMA2XX_ACC_X8_LSB__LEN           0
#define BMA2XX_ACC_X8_LSB__MSK           0x00
#define BMA2XX_ACC_X8_LSB__REG           BMA2XX_X_AXIS_LSB_REG

#define BMA2XX_ACC_X_MSB__POS            0
#define BMA2XX_ACC_X_MSB__LEN            8
#define BMA2XX_ACC_X_MSB__MSK            0xFF
#define BMA2XX_ACC_X_MSB__REG            BMA2XX_X_AXIS_MSB_REG

/************************************0x04*************************************************/
#define BMA2XX_NEW_DATA_Y__POS           0
#define BMA2XX_NEW_DATA_Y__LEN           1
#define BMA2XX_NEW_DATA_Y__MSK           0x01
#define BMA2XX_NEW_DATA_Y__REG           BMA2XX_Y_AXIS_LSB_REG

#define BMA2XX_ACC_Y14_LSB__POS          2
#define BMA2XX_ACC_Y14_LSB__LEN          6
#define BMA2XX_ACC_Y14_LSB__MSK          0xFC
#define BMA2XX_ACC_Y14_LSB__REG          BMA2XX_Y_AXIS_LSB_REG

#define BMA2XX_ACC_Y12_LSB__POS          4
#define BMA2XX_ACC_Y12_LSB__LEN          4
#define BMA2XX_ACC_Y12_LSB__MSK          0xF0
#define BMA2XX_ACC_Y12_LSB__REG          BMA2XX_Y_AXIS_LSB_REG

#define BMA2XX_ACC_Y10_LSB__POS          6
#define BMA2XX_ACC_Y10_LSB__LEN          2
#define BMA2XX_ACC_Y10_LSB__MSK          0xC0
#define BMA2XX_ACC_Y10_LSB__REG          BMA2XX_Y_AXIS_LSB_REG

#define BMA2XX_ACC_Y8_LSB__POS           0
#define BMA2XX_ACC_Y8_LSB__LEN           0
#define BMA2XX_ACC_Y8_LSB__MSK           0x00
#define BMA2XX_ACC_Y8_LSB__REG           BMA2XX_Y_AXIS_LSB_REG

#define BMA2XX_ACC_Y_MSB__POS            0
#define BMA2XX_ACC_Y_MSB__LEN            8
#define BMA2XX_ACC_Y_MSB__MSK            0xFF
#define BMA2XX_ACC_Y_MSB__REG            BMA2XX_Y_AXIS_MSB_REG

/*************************************0X06*****************************************/
#define BMA2XX_NEW_DATA_Z__POS           0
#define BMA2XX_NEW_DATA_Z__LEN           1
#define BMA2XX_NEW_DATA_Z__MSK           0x01
#define BMA2XX_NEW_DATA_Z__REG           BMA2XX_Z_AXIS_LSB_REG

#define BMA2XX_ACC_Z14_LSB__POS          2
#define BMA2XX_ACC_Z14_LSB__LEN          6
#define BMA2XX_ACC_Z14_LSB__MSK          0xFC
#define BMA2XX_ACC_Z14_LSB__REG          BMA2XX_Z_AXIS_LSB_REG

#define BMA2XX_ACC_Z12_LSB__POS          4
#define BMA2XX_ACC_Z12_LSB__LEN          4
#define BMA2XX_ACC_Z12_LSB__MSK          0xF0
#define BMA2XX_ACC_Z12_LSB__REG          BMA2XX_Z_AXIS_LSB_REG

#define BMA2XX_ACC_Z10_LSB__POS          6
#define BMA2XX_ACC_Z10_LSB__LEN          2
#define BMA2XX_ACC_Z10_LSB__MSK          0xC0
#define BMA2XX_ACC_Z10_LSB__REG          BMA2XX_Z_AXIS_LSB_REG

#define BMA2XX_ACC_Z8_LSB__POS           0
#define BMA2XX_ACC_Z8_LSB__LEN           0
#define BMA2XX_ACC_Z8_LSB__MSK           0x00
#define BMA2XX_ACC_Z8_LSB__REG           BMA2XX_Z_AXIS_LSB_REG

#define BMA2XX_ACC_Z_MSB__POS            0
#define BMA2XX_ACC_Z_MSB__LEN            8
#define BMA2XX_ACC_Z_MSB__MSK            0xFF
#define BMA2XX_ACC_Z_MSB__REG            BMA2XX_Z_AXIS_MSB_REG

/***************************************0X09****************************************/
/*  INTERRUPT STATUS BITS  */
#define BMA2XX_LOWG_INT_S__POS               0
#define BMA2XX_LOWG_INT_S__LEN               1
#define BMA2XX_LOWG_INT_S__MSK               0x01
#define BMA2XX_LOWG_INT_S__REG               BMA2XX_STATUS1_REG

#define BMA2XX_HIGHG_INT_S__POS              1
#define BMA2XX_HIGHG_INT_S__LEN              1
#define BMA2XX_HIGHG_INT_S__MSK              0x02
#define BMA2XX_HIGHG_INT_S__REG              BMA2XX_STATUS1_REG

#define BMA2XX_SLOPE_INT_S__POS              2
#define BMA2XX_SLOPE_INT_S__LEN              1
#define BMA2XX_SLOPE_INT_S__MSK              0x04
#define BMA2XX_SLOPE_INT_S__REG              BMA2XX_STATUS1_REG

//ADDED
#define BMA2XX_SLO_NO_MOT_INT_S__POS         3
#define BMA2XX_SLO_NO_MOT_INT_S__LEN         1
#define BMA2XX_SLO_NO_MOT_INT_S__MSK         0x08
#define BMA2XX_SLO_NO_MOT_INT_S__REG         BMA2XX_STATUS1_REG

#define BMA2XX_DOUBLE_TAP_INT_S__POS         4
#define BMA2XX_DOUBLE_TAP_INT_S__LEN         1
#define BMA2XX_DOUBLE_TAP_INT_S__MSK         0x10
#define BMA2XX_DOUBLE_TAP_INT_S__REG         BMA2XX_STATUS1_REG

#define BMA2XX_SINGLE_TAP_INT_S__POS         5
#define BMA2XX_SINGLE_TAP_INT_S__LEN         1
#define BMA2XX_SINGLE_TAP_INT_S__MSK         0x20
#define BMA2XX_SINGLE_TAP_INT_S__REG         BMA2XX_STATUS1_REG

#define BMA2XX_ORIENT_INT_S__POS             6
#define BMA2XX_ORIENT_INT_S__LEN             1
#define BMA2XX_ORIENT_INT_S__MSK             0x40
#define BMA2XX_ORIENT_INT_S__REG             BMA2XX_STATUS1_REG

#define BMA2XX_FLAT_INT_S__POS               7
#define BMA2XX_FLAT_INT_S__LEN               1
#define BMA2XX_FLAT_INT_S__MSK               0x80
#define BMA2XX_FLAT_INT_S__REG               BMA2XX_STATUS1_REG

/***********************************0X0A*************************************/
#define BMA2XX_FIFO_FULL_INT_S__POS          5
#define BMA2XX_FIFO_FULL_INT_S__LEN          1
#define BMA2XX_FIFO_FULL_INT_S__MSK          0x20
#define BMA2XX_FIFO_FULL_INT_S__REG          BMA2XX_STATUS2_REG

#define BMA2XX_FIFO_WM_INT_S__POS            6
#define BMA2XX_FIFO_WM_INT_S__LEN            1
#define BMA2XX_FIFO_WM_INT_S__MSK            0x40
#define BMA2XX_FIFO_WM_INT_S__REG            BMA2XX_STATUS2_REG

#define BMA2XX_DATA_INT_S__POS               7
#define BMA2XX_DATA_INT_S__LEN               1
#define BMA2XX_DATA_INT_S__MSK               0x80
#define BMA2XX_DATA_INT_S__REG               BMA2XX_STATUS2_REG

/***********************************0X0B*************************************/
#define BMA2XX_SLOPE_FIRST_X__POS        0
#define BMA2XX_SLOPE_FIRST_X__LEN        1
#define BMA2XX_SLOPE_FIRST_X__MSK        0x01
#define BMA2XX_SLOPE_FIRST_X__REG        BMA2XX_STATUS_TAP_SLOPE_REG

#define BMA2XX_SLOPE_FIRST_Y__POS        1
#define BMA2XX_SLOPE_FIRST_Y__LEN        1
#define BMA2XX_SLOPE_FIRST_Y__MSK        0x02
#define BMA2XX_SLOPE_FIRST_Y__REG        BMA2XX_STATUS_TAP_SLOPE_REG

#define BMA2XX_SLOPE_FIRST_Z__POS        2
#define BMA2XX_SLOPE_FIRST_Z__LEN        1
#define BMA2XX_SLOPE_FIRST_Z__MSK        0x04
#define BMA2XX_SLOPE_FIRST_Z__REG        BMA2XX_STATUS_TAP_SLOPE_REG

#define BMA2XX_SLOPE_SIGN_S__POS         3
#define BMA2XX_SLOPE_SIGN_S__LEN         1
#define BMA2XX_SLOPE_SIGN_S__MSK         0x08
#define BMA2XX_SLOPE_SIGN_S__REG         BMA2XX_STATUS_TAP_SLOPE_REG

#define BMA2XX_TAP_FIRST_X__POS          4
#define BMA2XX_TAP_FIRST_X__LEN          1
#define BMA2XX_TAP_FIRST_X__MSK          0x10
#define BMA2XX_TAP_FIRST_X__REG          BMA2XX_STATUS_TAP_SLOPE_REG

#define BMA2XX_TAP_FIRST_Y__POS          5
#define BMA2XX_TAP_FIRST_Y__LEN          1
#define BMA2XX_TAP_FIRST_Y__MSK          0x20
#define BMA2XX_TAP_FIRST_Y__REG          BMA2XX_STATUS_TAP_SLOPE_REG

#define BMA2XX_TAP_FIRST_Z__POS          6
#define BMA2XX_TAP_FIRST_Z__LEN          1
#define BMA2XX_TAP_FIRST_Z__MSK          0x40
#define BMA2XX_TAP_FIRST_Z__REG          BMA2XX_STATUS_TAP_SLOPE_REG

#define BMA2XX_TAP_SIGN_S__POS           7
#define BMA2XX_TAP_SIGN_S__LEN           1
#define BMA2XX_TAP_SIGN_S__MSK           0x80
#define BMA2XX_TAP_SIGN_S__REG           BMA2XX_STATUS_TAP_SLOPE_REG


/***********************************0X0C**********************************/
#define BMA2XX_HIGHG_FIRST_X__POS        0
#define BMA2XX_HIGHG_FIRST_X__LEN        1
#define BMA2XX_HIGHG_FIRST_X__MSK        0x01
#define BMA2XX_HIGHG_FIRST_X__REG        BMA2XX_STATUS_ORIENT_HIGH_REG

#define BMA2XX_HIGHG_FIRST_Y__POS        1
#define BMA2XX_HIGHG_FIRST_Y__LEN        1
#define BMA2XX_HIGHG_FIRST_Y__MSK        0x02
#define BMA2XX_HIGHG_FIRST_Y__REG        BMA2XX_STATUS_ORIENT_HIGH_REG

#define BMA2XX_HIGHG_FIRST_Z__POS        2
#define BMA2XX_HIGHG_FIRST_Z__LEN        1
#define BMA2XX_HIGHG_FIRST_Z__MSK        0x04
#define BMA2XX_HIGHG_FIRST_Z__REG        BMA2XX_STATUS_ORIENT_HIGH_REG

#define BMA2XX_HIGHG_SIGN_S__POS         3
#define BMA2XX_HIGHG_SIGN_S__LEN         1
#define BMA2XX_HIGHG_SIGN_S__MSK         0x08
#define BMA2XX_HIGHG_SIGN_S__REG         BMA2XX_STATUS_ORIENT_HIGH_REG
#define BMA2XX_ORIENT_S__POS             4
#define BMA2XX_ORIENT_S__LEN             3
#define BMA2XX_ORIENT_S__MSK             0x70
#define BMA2XX_ORIENT_S__REG             BMA2XX_STATUS_ORIENT_HIGH_REG

#define BMA2XX_FLAT_S__POS               7
#define BMA2XX_FLAT_S__LEN               1
#define BMA2XX_FLAT_S__MSK               0x80
#define BMA2XX_FLAT_S__REG               BMA2XX_STATUS_ORIENT_HIGH_REG

/***********************************0X0E***********************************/
//FIFO_STATUS
#define BMA2XX_FIFO_FRAME_COUNTER_S__POS            0
#define BMA2XX_FIFO_FRAME_COUNTER_S__LEN            7
#define BMA2XX_FIFO_FRAME_COUNTER_S__MSK            0x7F
#define BMA2XX_FIFO_FRAME_COUNTER_S__REG            BMA2XX_STATUS_FIFO_REG

#define BMA2XX_FIFO_OVERRUN_S__POS                  7
#define BMA2XX_FIFO_OVERRUN_S__LEN                  1
#define BMA2XX_FIFO_OVERRUN_S__MSK                  0x80
#define BMA2XX_FIFO_OVERRUN_S__REG                  BMA2XX_STATUS_FIFO_REG

/***************************************0x0F**********************************************/
#define BMA2XX_RANGE_SEL__POS                       0
#define BMA2XX_RANGE_SEL__LEN                       4
#define BMA2XX_RANGE_SEL__MSK                       0x0F
#define BMA2XX_RANGE_SEL__REG                       BMA2XX_RANGE_SEL_REG

/**************************************0x10*********************************************/
#define BMA2XX_BANDWIDTH__POS                       0
#define BMA2XX_BANDWIDTH__LEN                       5
#define BMA2XX_BANDWIDTH__MSK                       0x1F
#define BMA2XX_BANDWIDTH__REG                       BMA2XX_BW_SEL_REG

/**************************************0x11**********************************************/
#define BMA2XX_SLEEP_DUR__POS                       1
#define BMA2XX_SLEEP_DUR__LEN                       4
#define BMA2XX_SLEEP_DUR__MSK                       0x1E
#define BMA2XX_SLEEP_DUR__REG                       BMA2XX_MODE_CTRL_REG

#define BMA2XX_MODE_CTRL__POS                       5
#define BMA2XX_MODE_CTRL__LEN                       3
#define BMA2XX_MODE_CTRL__MSK                       0xE0
#define BMA2XX_MODE_CTRL__REG                       BMA2XX_MODE_CTRL_REG

//ADDED
#define BMA2XX_EN_LOW_POWER__POS                    6
#define BMA2XX_EN_LOW_POWER__LEN                    1
#define BMA2XX_EN_LOW_POWER__MSK                    0x40
#define BMA2XX_EN_LOW_POWER__REG                    BMA2XX_MODE_CTRL_REG

/*********************************0x12***************************************************/
//ADDED
#define BMA2XX_SLEEP_TIMER__POS                     5
#define BMA2XX_SLEEP_TIMER__LEN                     1
#define BMA2XX_SLEEP_TIMER__MSK                     0x20
#define BMA2XX_SLEEP_TIMER__REG                     BMA2XX_LOW_NOISE_CTRL_REG

//ADDED
#define BMA2XX_LOW_POWER_MODE__POS                  6
#define BMA2XX_LOW_POWER_MODE__LEN                  1
#define BMA2XX_LOW_POWER_MODE__MSK                  0x40
#define BMA2XX_LOW_POWER_MODE__REG                  BMA2XX_LOW_NOISE_CTRL_REG


/**********************************0x13*************************************************/
/**     DISABLE MSB SHADOWING PROCEDURE          **/
#define BMA2XX_DIS_SHADOW_PROC__POS                 6
#define BMA2XX_DIS_SHADOW_PROC__LEN                 1
#define BMA2XX_DIS_SHADOW_PROC__MSK                 0x40
#define BMA2XX_DIS_SHADOW_PROC__REG                 BMA2XX_DATA_CTRL_REG

/**     FILTERED OR UNFILTERED ACCELERATION DATA  **/
#define BMA2XX_EN_DATA_HIGH_BW__POS                 7
#define BMA2XX_EN_DATA_HIGH_BW__LEN                 1
#define BMA2XX_EN_DATA_HIGH_BW__MSK                 0x80
#define BMA2XX_EN_DATA_HIGH_BW__REG                 BMA2XX_DATA_CTRL_REG

#define BMA2XX_EN_SOFT_RESET_VALUE                  0xB6

/************************************0x16*********************************************/
/**     INTERRUPT ENABLE REGISTER              **/
#define BMA2XX_EN_SLOPE_X_INT__POS                  0
#define BMA2XX_EN_SLOPE_X_INT__LEN                  1
#define BMA2XX_EN_SLOPE_X_INT__MSK                  0x01
#define BMA2XX_EN_SLOPE_X_INT__REG                  BMA2XX_INT_ENABLE1_REG

#define BMA2XX_EN_SLOPE_Y_INT__POS                  1
#define BMA2XX_EN_SLOPE_Y_INT__LEN                  1
#define BMA2XX_EN_SLOPE_Y_INT__MSK                  0x02
#define BMA2XX_EN_SLOPE_Y_INT__REG                  BMA2XX_INT_ENABLE1_REG

#define BMA2XX_EN_SLOPE_Z_INT__POS                  2
#define BMA2XX_EN_SLOPE_Z_INT__LEN                  1
#define BMA2XX_EN_SLOPE_Z_INT__MSK                  0x04
#define BMA2XX_EN_SLOPE_Z_INT__REG                  BMA2XX_INT_ENABLE1_REG

#define BMA2XX_EN_DOUBLE_TAP_INT__POS               4
#define BMA2XX_EN_DOUBLE_TAP_INT__LEN               1
#define BMA2XX_EN_DOUBLE_TAP_INT__MSK               0x10
#define BMA2XX_EN_DOUBLE_TAP_INT__REG               BMA2XX_INT_ENABLE1_REG

#define BMA2XX_EN_SINGLE_TAP_INT__POS               5
#define BMA2XX_EN_SINGLE_TAP_INT__LEN               1
#define BMA2XX_EN_SINGLE_TAP_INT__MSK               0x20
#define BMA2XX_EN_SINGLE_TAP_INT__REG               BMA2XX_INT_ENABLE1_REG

#define BMA2XX_EN_ORIENT_INT__POS                   6
#define BMA2XX_EN_ORIENT_INT__LEN                   1
#define BMA2XX_EN_ORIENT_INT__MSK                   0x40
#define BMA2XX_EN_ORIENT_INT__REG                   BMA2XX_INT_ENABLE1_REG

#define BMA2XX_EN_FLAT_INT__POS                     7
#define BMA2XX_EN_FLAT_INT__LEN                     1
#define BMA2XX_EN_FLAT_INT__MSK                     0x80
#define BMA2XX_EN_FLAT_INT__REG                     BMA2XX_INT_ENABLE1_REG

/***************************************0x17*******************************************/
/**     INTERRUPT ENABLE REGISTER              **/
#define BMA2XX_EN_HIGHG_X_INT__POS                  0
#define BMA2XX_EN_HIGHG_X_INT__LEN                  1
#define BMA2XX_EN_HIGHG_X_INT__MSK                  0x01
#define BMA2XX_EN_HIGHG_X_INT__REG                  BMA2XX_INT_ENABLE2_REG

#define BMA2XX_EN_HIGHG_Y_INT__POS                  1
#define BMA2XX_EN_HIGHG_Y_INT__LEN                  1
#define BMA2XX_EN_HIGHG_Y_INT__MSK                  0x02
#define BMA2XX_EN_HIGHG_Y_INT__REG                  BMA2XX_INT_ENABLE2_REG

#define BMA2XX_EN_HIGHG_Z_INT__POS                  2
#define BMA2XX_EN_HIGHG_Z_INT__LEN                  1
#define BMA2XX_EN_HIGHG_Z_INT__MSK                  0x04
#define BMA2XX_EN_HIGHG_Z_INT__REG                  BMA2XX_INT_ENABLE2_REG

#define BMA2XX_EN_LOWG_INT__POS                     3
#define BMA2XX_EN_LOWG_INT__LEN                     1
#define BMA2XX_EN_LOWG_INT__MSK                     0x08
#define BMA2XX_EN_LOWG_INT__REG                     BMA2XX_INT_ENABLE2_REG

#define BMA2XX_EN_NEW_DATA_INT__POS                 4
#define BMA2XX_EN_NEW_DATA_INT__LEN                 1
#define BMA2XX_EN_NEW_DATA_INT__MSK                 0x10
#define BMA2XX_EN_NEW_DATA_INT__REG                 BMA2XX_INT_ENABLE2_REG

//ADDED
#define BMA2XX_INT_FFULL_EN_INT__POS                5
#define BMA2XX_INT_FFULL_EN_INT__LEN                1
#define BMA2XX_INT_FFULL_EN_INT__MSK                0x20
#define BMA2XX_INT_FFULL_EN_INT__REG                BMA2XX_INT_ENABLE2_REG

#define BMA2XX_INT_FWM_EN_INT__POS                  6
#define BMA2XX_INT_FWM_EN_INT__LEN                  1
#define BMA2XX_INT_FWM_EN_INT__MSK                  0x40
#define BMA2XX_INT_FWM_EN_INT__REG                  BMA2XX_INT_ENABLE2_REG


/**********************************0x18*********************************************/
//INT SLO NO MOT
#define BMA2XX_INT_SLO_NO_MOT_EN_X_INT__POS        0
#define BMA2XX_INT_SLO_NO_MOT_EN_X_INT__LEN        1
#define BMA2XX_INT_SLO_NO_MOT_EN_X_INT__MSK        0x01
#define BMA2XX_INT_SLO_NO_MOT_EN_X_INT__REG        BMA2XX_INT_SLO_NO_MOT_REG

#define BMA2XX_INT_SLO_NO_MOT_EN_Y_INT__POS        1
#define BMA2XX_INT_SLO_NO_MOT_EN_Y_INT__LEN        1
#define BMA2XX_INT_SLO_NO_MOT_EN_Y_INT__MSK        0x02
#define BMA2XX_INT_SLO_NO_MOT_EN_Y_INT__REG        BMA2XX_INT_SLO_NO_MOT_REG

#define BMA2XX_INT_SLO_NO_MOT_EN_Z_INT__POS        2
#define BMA2XX_INT_SLO_NO_MOT_EN_Z_INT__LEN        1
#define BMA2XX_INT_SLO_NO_MOT_EN_Z_INT__MSK        0x04
#define BMA2XX_INT_SLO_NO_MOT_EN_Z_INT__REG        BMA2XX_INT_SLO_NO_MOT_REG

#define BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT__POS      3
#define BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT__LEN      1
#define BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT__MSK      0x08
#define BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT__REG      BMA2XX_INT_SLO_NO_MOT_REG

/************************************0x19*******************************************/
#define BMA2XX_EN_INT1_PAD_LOWG__POS               0
#define BMA2XX_EN_INT1_PAD_LOWG__LEN               1
#define BMA2XX_EN_INT1_PAD_LOWG__MSK       		   0x01
#define BMA2XX_EN_INT1_PAD_LOWG__REG        	   BMA2XX_INT1_PAD_SEL_REG

#define BMA2XX_EN_INT1_PAD_HIGHG__POS              1
#define BMA2XX_EN_INT1_PAD_HIGHG__LEN              1
#define BMA2XX_EN_INT1_PAD_HIGHG__MSK              0x02
#define BMA2XX_EN_INT1_PAD_HIGHG__REG              BMA2XX_INT1_PAD_SEL_REG

#define BMA2XX_EN_INT1_PAD_SLOPE__POS              2
#define BMA2XX_EN_INT1_PAD_SLOPE__LEN              1
#define BMA2XX_EN_INT1_PAD_SLOPE__MSK              0x04
#define BMA2XX_EN_INT1_PAD_SLOPE__REG              BMA2XX_INT1_PAD_SEL_REG

//ADDED
#define BMA2XX_EN_INT1_PAD_SLO_NO_MOT__POS         3
#define BMA2XX_EN_INT1_PAD_SLO_NO_MOT__LEN         1
#define BMA2XX_EN_INT1_PAD_SLO_NO_MOT__MSK         0x08
#define BMA2XX_EN_INT1_PAD_SLO_NO_MOT__REG         BMA2XX_INT1_PAD_SEL_REG

#define BMA2XX_EN_INT1_PAD_DB_TAP__POS             4
#define BMA2XX_EN_INT1_PAD_DB_TAP__LEN             1
#define BMA2XX_EN_INT1_PAD_DB_TAP__MSK             0x10
#define BMA2XX_EN_INT1_PAD_DB_TAP__REG             BMA2XX_INT1_PAD_SEL_REG

#define BMA2XX_EN_INT1_PAD_SNG_TAP__POS            5
#define BMA2XX_EN_INT1_PAD_SNG_TAP__LEN            1
#define BMA2XX_EN_INT1_PAD_SNG_TAP__MSK            0x20
#define BMA2XX_EN_INT1_PAD_SNG_TAP__REG            BMA2XX_INT1_PAD_SEL_REG

#define BMA2XX_EN_INT1_PAD_ORIENT__POS             6
#define BMA2XX_EN_INT1_PAD_ORIENT__LEN             1
#define BMA2XX_EN_INT1_PAD_ORIENT__MSK             0x40
#define BMA2XX_EN_INT1_PAD_ORIENT__REG             BMA2XX_INT1_PAD_SEL_REG

#define BMA2XX_EN_INT1_PAD_FLAT__POS               7
#define BMA2XX_EN_INT1_PAD_FLAT__LEN               1
#define BMA2XX_EN_INT1_PAD_FLAT__MSK               0x80
#define BMA2XX_EN_INT1_PAD_FLAT__REG               BMA2XX_INT1_PAD_SEL_REG

/***************************************0x1B***********************************************/
#define BMA2XX_EN_INT2_PAD_LOWG__POS               0
#define BMA2XX_EN_INT2_PAD_LOWG__LEN               1
#define BMA2XX_EN_INT2_PAD_LOWG__MSK               0x01
#define BMA2XX_EN_INT2_PAD_LOWG__REG               BMA2XX_INT2_PAD_SEL_REG

#define BMA2XX_EN_INT2_PAD_HIGHG__POS              1
#define BMA2XX_EN_INT2_PAD_HIGHG__LEN              1
#define BMA2XX_EN_INT2_PAD_HIGHG__MSK              0x02
#define BMA2XX_EN_INT2_PAD_HIGHG__REG              BMA2XX_INT2_PAD_SEL_REG

#define BMA2XX_EN_INT2_PAD_SLOPE__POS              2
#define BMA2XX_EN_INT2_PAD_SLOPE__LEN              1
#define BMA2XX_EN_INT2_PAD_SLOPE__MSK              0x04
#define BMA2XX_EN_INT2_PAD_SLOPE__REG              BMA2XX_INT2_PAD_SEL_REG

//ADDED
#define BMA2XX_EN_INT2_PAD_SLO_NO_MOT__POS         3
#define BMA2XX_EN_INT2_PAD_SLO_NO_MOT__LEN         1
#define BMA2XX_EN_INT2_PAD_SLO_NO_MOT__MSK         0x08
#define BMA2XX_EN_INT2_PAD_SLO_NO_MOT__REG         BMA2XX_INT2_PAD_SEL_REG

#define BMA2XX_EN_INT2_PAD_DB_TAP__POS             4
#define BMA2XX_EN_INT2_PAD_DB_TAP__LEN             1
#define BMA2XX_EN_INT2_PAD_DB_TAP__MSK             0x10
#define BMA2XX_EN_INT2_PAD_DB_TAP__REG             BMA2XX_INT2_PAD_SEL_REG

#define BMA2XX_EN_INT2_PAD_SNG_TAP__POS            5
#define BMA2XX_EN_INT2_PAD_SNG_TAP__LEN            1
#define BMA2XX_EN_INT2_PAD_SNG_TAP__MSK            0x20
#define BMA2XX_EN_INT2_PAD_SNG_TAP__REG            BMA2XX_INT2_PAD_SEL_REG

#define BMA2XX_EN_INT2_PAD_ORIENT__POS             6
#define BMA2XX_EN_INT2_PAD_ORIENT__LEN             1
#define BMA2XX_EN_INT2_PAD_ORIENT__MSK             0x40
#define BMA2XX_EN_INT2_PAD_ORIENT__REG             BMA2XX_INT2_PAD_SEL_REG

#define BMA2XX_EN_INT2_PAD_FLAT__POS               7
#define BMA2XX_EN_INT2_PAD_FLAT__LEN               1
#define BMA2XX_EN_INT2_PAD_FLAT__MSK               0x80
#define BMA2XX_EN_INT2_PAD_FLAT__REG               BMA2XX_INT2_PAD_SEL_REG

/*********************************0x1A**********************************************/
#define BMA2XX_EN_INT1_PAD_NEWDATA__POS            0
#define BMA2XX_EN_INT1_PAD_NEWDATA__LEN            1
#define BMA2XX_EN_INT1_PAD_NEWDATA__MSK            0x01
#define BMA2XX_EN_INT1_PAD_NEWDATA__REG            BMA2XX_INT_DATA_SEL_REG

//ADDED
#define BMA2XX_EN_INT1_PAD_FWM__POS                1
#define BMA2XX_EN_INT1_PAD_FWM__LEN                1
#define BMA2XX_EN_INT1_PAD_FWM__MSK                0x02
#define BMA2XX_EN_INT1_PAD_FWM__REG                BMA2XX_INT_DATA_SEL_REG

#define BMA2XX_EN_INT1_PAD_FFULL__POS              2
#define BMA2XX_EN_INT1_PAD_FFULL__LEN              1
#define BMA2XX_EN_INT1_PAD_FFULL__MSK              0x04
#define BMA2XX_EN_INT1_PAD_FFULL__REG              BMA2XX_INT_DATA_SEL_REG

#define BMA2XX_EN_INT2_PAD_FFULL__POS              5
#define BMA2XX_EN_INT2_PAD_FFULL__LEN              1
#define BMA2XX_EN_INT2_PAD_FFULL__MSK              0x20
#define BMA2XX_EN_INT2_PAD_FFULL__REG              BMA2XX_INT_DATA_SEL_REG

#define BMA2XX_EN_INT2_PAD_FWM__POS                6
#define BMA2XX_EN_INT2_PAD_FWM__LEN                1
#define BMA2XX_EN_INT2_PAD_FWM__MSK                0x40
#define BMA2XX_EN_INT2_PAD_FWM__REG                BMA2XX_INT_DATA_SEL_REG

#define BMA2XX_EN_INT2_PAD_NEWDATA__POS            7
#define BMA2XX_EN_INT2_PAD_NEWDATA__LEN            1
#define BMA2XX_EN_INT2_PAD_NEWDATA__MSK            0x80
#define BMA2XX_EN_INT2_PAD_NEWDATA__REG            BMA2XX_INT_DATA_SEL_REG

/*****************************0x1E********************************************************/
/*****          INTERRUPT SOURCE SELECTION                      *****/
#define BMA2XX_UNFILT_INT_SRC_LOWG__POS            0
#define BMA2XX_UNFILT_INT_SRC_LOWG__LEN            1
#define BMA2XX_UNFILT_INT_SRC_LOWG__MSK            0x01
#define BMA2XX_UNFILT_INT_SRC_LOWG__REG            BMA2XX_INT_SRC_REG

#define BMA2XX_UNFILT_INT_SRC_HIGHG__POS           1
#define BMA2XX_UNFILT_INT_SRC_HIGHG__LEN           1
#define BMA2XX_UNFILT_INT_SRC_HIGHG__MSK           0x02
#define BMA2XX_UNFILT_INT_SRC_HIGHG__REG           BMA2XX_INT_SRC_REG

#define BMA2XX_UNFILT_INT_SRC_SLOPE__POS           2
#define BMA2XX_UNFILT_INT_SRC_SLOPE__LEN           1
#define BMA2XX_UNFILT_INT_SRC_SLOPE__MSK           0x04
#define BMA2XX_UNFILT_INT_SRC_SLOPE__REG           BMA2XX_INT_SRC_REG

//ADDED
#define BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT__POS      3
#define BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT__LEN      1
#define BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT__MSK      0x08
#define BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT__REG      BMA2XX_INT_SRC_REG

#define BMA2XX_UNFILT_INT_SRC_TAP__POS             4
#define BMA2XX_UNFILT_INT_SRC_TAP__LEN             1
#define BMA2XX_UNFILT_INT_SRC_TAP__MSK             0x10
#define BMA2XX_UNFILT_INT_SRC_TAP__REG             BMA2XX_INT_SRC_REG

#define BMA2XX_UNFILT_INT_SRC_DATA__POS            5
#define BMA2XX_UNFILT_INT_SRC_DATA__LEN            1
#define BMA2XX_UNFILT_INT_SRC_DATA__MSK            0x20
#define BMA2XX_UNFILT_INT_SRC_DATA__REG            BMA2XX_INT_SRC_REG

/*******************************0x20****************************************************/
/*****  INTERRUPT PAD ACTIVE LEVEL AND OUTPUT TYPE       *****/
#define BMA2XX_INT1_PAD_ACTIVE_LEVEL__POS          0
#define BMA2XX_INT1_PAD_ACTIVE_LEVEL__LEN          1
#define BMA2XX_INT1_PAD_ACTIVE_LEVEL__MSK          0x01
#define BMA2XX_INT1_PAD_ACTIVE_LEVEL__REG          BMA2XX_INT_SET_REG

#define BMA2XX_INT2_PAD_ACTIVE_LEVEL__POS          2
#define BMA2XX_INT2_PAD_ACTIVE_LEVEL__LEN          1
#define BMA2XX_INT2_PAD_ACTIVE_LEVEL__MSK          0x04
#define BMA2XX_INT2_PAD_ACTIVE_LEVEL__REG          BMA2XX_INT_SET_REG


/*****  OUTPUT TYPE IF SET TO 1 IS : OPEN DRIVE , IF NOT SET
        IT IS PUSH-PULL                                  *****/
#define BMA2XX_INT1_PAD_OUTPUT_TYPE__POS           1
#define BMA2XX_INT1_PAD_OUTPUT_TYPE__LEN           1
#define BMA2XX_INT1_PAD_OUTPUT_TYPE__MSK           0x02
#define BMA2XX_INT1_PAD_OUTPUT_TYPE__REG           BMA2XX_INT_SET_REG

#define BMA2XX_INT2_PAD_OUTPUT_TYPE__POS           3
#define BMA2XX_INT2_PAD_OUTPUT_TYPE__LEN           1
#define BMA2XX_INT2_PAD_OUTPUT_TYPE__MSK           0x08
#define BMA2XX_INT2_PAD_OUTPUT_TYPE__REG           BMA2XX_INT_SET_REG

/*********************************0x21**************************************************/
/*****               INTERRUPT MODE SELECTION              ******/
#define BMA2XX_LATCH_INT__POS                      0
#define BMA2XX_LATCH_INT__LEN                      4
#define BMA2XX_LATCH_INT__MSK                      0x0F
#define BMA2XX_LATCH_INT__REG                      BMA2XX_INT_CTRL_REG

/*****               LATCHED INTERRUPT RESET               ******/
#define BMA2XX_RESET_INT__POS                      7
#define BMA2XX_RESET_INT__LEN                      1
#define BMA2XX_RESET_INT__MSK                      0x80
#define BMA2XX_RESET_INT__REG                      BMA2XX_INT_CTRL_REG

/*******************************0x24**************************************************/
/*****               LOW-G HYSTERESIS                       ******/
#define BMA2XX_LOWG_HYST__POS                      0
#define BMA2XX_LOWG_HYST__LEN                      2
#define BMA2XX_LOWG_HYST__MSK                      0x03
#define BMA2XX_LOWG_HYST__REG                      BMA2XX_LOW_HIGH_HYST_REG

/*****               LOW-G INTERRUPT MODE                   ******/
/*****       IF 1 -- SUM MODE , 0 -- SINGLE MODE            ******/
#define BMA2XX_LOWG_INT_MODE__POS                  2
#define BMA2XX_LOWG_INT_MODE__LEN                  1
#define BMA2XX_LOWG_INT_MODE__MSK                  0x04
#define BMA2XX_LOWG_INT_MODE__REG                  BMA2XX_LOW_HIGH_HYST_REG

/*****               HIGH-G HYSTERESIS                       ******/
#define BMA2XX_HIGHG_HYST__POS                     6
#define BMA2XX_HIGHG_HYST__LEN                     2
#define BMA2XX_HIGHG_HYST__MSK                     0xC0
#define BMA2XX_HIGHG_HYST__REG                     BMA2XX_LOW_HIGH_HYST_REG

/*******************************0x27**************************************************/
/*****               SLOPE DURATION                        ******/
#define BMA2XX_SLOPE_DUR__POS                      0
#define BMA2XX_SLOPE_DUR__LEN                      2
#define BMA2XX_SLOPE_DUR__MSK                      0x03
#define BMA2XX_SLOPE_DUR__REG                      BMA2XX_SLOPE_DURN_REG

//SLO_NO_MOT_DUR ADDED
#define BMA2XX_SLO_NO_MOT_DUR__POS                 2
#define BMA2XX_SLO_NO_MOT_DUR__LEN                 6
#define BMA2XX_SLO_NO_MOT_DUR__MSK                 0xFC
#define BMA2XX_SLO_NO_MOT_DUR__REG                 BMA2XX_SLOPE_DURN_REG

/*******************************0x2A**************************************************/
/*****               TAP DURATION                        ******/
#define BMA2XX_TAP_DUR__POS                    0
#define BMA2XX_TAP_DUR__LEN                    3
#define BMA2XX_TAP_DUR__MSK                    0x07
#define BMA2XX_TAP_DUR__REG                    BMA2XX_TAP_PARAM_REG

/*****               TAP SHOCK DURATION                 ******/
#define BMA2XX_TAP_SHOCK_DURN__POS             6
#define BMA2XX_TAP_SHOCK_DURN__LEN             1
#define BMA2XX_TAP_SHOCK_DURN__MSK             0x40
#define BMA2XX_TAP_SHOCK_DURN__REG             BMA2XX_TAP_PARAM_REG

/*****               ADV TAP INT                        ******/
#define BMA2XX_ADV_TAP_INT__POS                5
#define BMA2XX_ADV_TAP_INT__LEN                1
#define BMA2XX_ADV_TAP_INT__MSK                0x20
#define BMA2XX_ADV_TAP_INT__REG                BMA2XX_TAP_PARAM_REG

/*****               TAP QUIET DURATION                 ******/
#define BMA2XX_TAP_QUIET_DURN__POS             7
#define BMA2XX_TAP_QUIET_DURN__LEN             1
#define BMA2XX_TAP_QUIET_DURN__MSK             0x80
#define BMA2XX_TAP_QUIET_DURN__REG             BMA2XX_TAP_PARAM_REG

/*******************************0x2B**************************************************/
/*****               TAP THRESHOLD                       ******/
#define BMA2XX_TAP_THRES__POS                  0
#define BMA2XX_TAP_THRES__LEN                  5
#define BMA2XX_TAP_THRES__MSK                  0x1F
#define BMA2XX_TAP_THRES__REG                  BMA2XX_TAP_THRES_REG

/*****               TAP SAMPLES                         ******/
#define BMA2XX_TAP_SAMPLES__POS                6
#define BMA2XX_TAP_SAMPLES__LEN                2
#define BMA2XX_TAP_SAMPLES__MSK                0xC0
#define BMA2XX_TAP_SAMPLES__REG                BMA2XX_TAP_THRES_REG

/*******************************0x2C**************************************************/
/*****       ORIENTATION MODE                        ******/
#define BMA2XX_ORIENT_MODE__POS                  0
#define BMA2XX_ORIENT_MODE__LEN                  2
#define BMA2XX_ORIENT_MODE__MSK                  0x03
#define BMA2XX_ORIENT_MODE__REG                  BMA2XX_ORIENT_PARAM_REG

/*****       ORIENTATION BLOCKING                    ******/
#define BMA2XX_ORIENT_BLOCK__POS                 2
#define BMA2XX_ORIENT_BLOCK__LEN                 2
#define BMA2XX_ORIENT_BLOCK__MSK                 0x0C
#define BMA2XX_ORIENT_BLOCK__REG                 BMA2XX_ORIENT_PARAM_REG

/*****       ORIENTATION HYSTERESIS                  ******/
#define BMA2XX_ORIENT_HYST__POS                  4
#define BMA2XX_ORIENT_HYST__LEN                  3
#define BMA2XX_ORIENT_HYST__MSK                  0x70
#define BMA2XX_ORIENT_HYST__REG                  BMA2XX_ORIENT_PARAM_REG
/*****       ORIENTATION AXIS SELECTION              ******/
/***** IF SET TO 1 -- X AND Z ARE SWAPPED , Y IS INVERTED */

/*******************************0x2D**************************************************/
//ADDED
#define BMA2XX_ORIENT_UD_EN__POS                  6
#define BMA2XX_ORIENT_UD_EN__LEN                  1
#define BMA2XX_ORIENT_UD_EN__MSK                  0x40
#define BMA2XX_ORIENT_UD_EN__REG                  BMA2XX_THETA_BLOCK_REG

/*****       THETA BLOCKING                    ******/
#define BMA2XX_THETA_BLOCK__POS                   0
#define BMA2XX_THETA_BLOCK__LEN                   6
#define BMA2XX_THETA_BLOCK__MSK                   0x3F
#define BMA2XX_THETA_BLOCK__REG                   BMA2XX_THETA_BLOCK_REG

/*******************************0x2E**************************************************/
/*****       THETA FLAT                        ******/
#define BMA2XX_THETA_FLAT__POS                    0
#define BMA2XX_THETA_FLAT__LEN                    6
#define BMA2XX_THETA_FLAT__MSK                    0x3F
#define BMA2XX_THETA_FLAT__REG                    BMA2XX_THETA_FLAT_REG

/*******************************0x2F**************************************************/
/*****      FLAT HOLD TIME                     ******/
#define BMA2XX_FLAT_HOLD_TIME__POS                4
#define BMA2XX_FLAT_HOLD_TIME__LEN                2
#define BMA2XX_FLAT_HOLD_TIME__MSK                0x30
#define BMA2XX_FLAT_HOLD_TIME__REG                BMA2XX_FLAT_HOLD_TIME_REG

/*****      FLAT HYS                           ******/
#define BMA2XX_FLAT_HYS__POS                      0
#define BMA2XX_FLAT_HYS__LEN                      3
#define BMA2XX_FLAT_HYS__MSK                      0x07
#define BMA2XX_FLAT_HYS__REG                      BMA2XX_FLAT_HOLD_TIME_REG

/*******************************0x30**************************************************/
/*****      FIFO WATER MARK LEVEL TRIGGER RETAIN                        ******/
//ADDED
#define BMA2XX_FIFO_WML_TRIG_RETAIN__POS          0
#define BMA2XX_FIFO_WML_TRIG_RETAIN__LEN          6
#define BMA2XX_FIFO_WML_TRIG_RETAIN__MSK          0x3F
#define BMA2XX_FIFO_WML_TRIG_RETAIN__REG          BMA2XX_FIFO_WML_TRIG

/*******************************0x32**************************************************/
/*****      ACTIVATE SELF TEST                 ******/
#define BMA2XX_EN_SELF_TEST__POS                  0
#define BMA2XX_EN_SELF_TEST__LEN                  2
#define BMA2XX_EN_SELF_TEST__MSK                  0x03
#define BMA2XX_EN_SELF_TEST__REG                  BMA2XX_SELF_TEST_REG

/*****     SELF TEST -- NEGATIVE               ******/
#define BMA2XX_NEG_SELF_TEST__POS                 2
#define BMA2XX_NEG_SELF_TEST__LEN                 1
#define BMA2XX_NEG_SELF_TEST__MSK                 0x04
#define BMA2XX_NEG_SELF_TEST__REG                 BMA2XX_SELF_TEST_REG

/*******************************0x33**************************************************/
/*****     EEPROM CONTROL                      ******/
/* SETTING THIS BIT  UNLOCK'S WRITING SETTING REGISTERS TO EEPROM */
#define BMA2XX_UNLOCK_EE_PROG_MODE__POS           0
#define BMA2XX_UNLOCK_EE_PROG_MODE__LEN           1
#define BMA2XX_UNLOCK_EE_PROG_MODE__MSK           0x01
#define BMA2XX_UNLOCK_EE_PROG_MODE__REG           BMA2XX_EEPROM_CTRL_REG


/* SETTING THIS BIT STARTS WRITING SETTING REGISTERS TO EEPROM */
#define BMA2XX_START_EE_PROG_TRIG__POS            1
#define BMA2XX_START_EE_PROG_TRIG__LEN            1
#define BMA2XX_START_EE_PROG_TRIG__MSK            0x02
#define BMA2XX_START_EE_PROG_TRIG__REG            BMA2XX_EEPROM_CTRL_REG


/* STATUS OF WRITING TO EEPROM */
#define BMA2XX_EE_PROG_READY__POS                 2
#define BMA2XX_EE_PROG_READY__LEN                 1
#define BMA2XX_EE_PROG_READY__MSK                 0x04
#define BMA2XX_EE_PROG_READY__REG                 BMA2XX_EEPROM_CTRL_REG


/* UPDATE IMAGE REGISTERS WRITING TO EEPROM */
#define BMA2XX_UPDATE_IMAGE__POS                  3
#define BMA2XX_UPDATE_IMAGE__LEN                  1
#define BMA2XX_UPDATE_IMAGE__MSK                  0x08
#define BMA2XX_UPDATE_IMAGE__REG                  BMA2XX_EEPROM_CTRL_REG

/* ADDED */
#define BMA2XX_EE_REMAIN__POS                     4
#define BMA2XX_EE_REMAIN__LEN                     4
#define BMA2XX_EE_REMAIN__MSK                     0xF0
#define BMA2XX_EE_REMAIN__REG                     BMA2XX_EEPROM_CTRL_REG

/*******************************0x34**************************************************/
/* SPI INTERFACE MODE SELECTION */
#define BMA2XX_EN_SPI_MODE_3__POS                 0
#define BMA2XX_EN_SPI_MODE_3__LEN                 1
#define BMA2XX_EN_SPI_MODE_3__MSK                 0x01
#define BMA2XX_EN_SPI_MODE_3__REG                 BMA2XX_SERIAL_CTRL_REG

/* I2C WATCHDOG PERIOD SELECTION */
#define BMA2XX_I2C_WATCHDOG_PERIOD__POS           1
#define BMA2XX_I2C_WATCHDOG_PERIOD__LEN           1
#define BMA2XX_I2C_WATCHDOG_PERIOD__MSK           0x02
#define BMA2XX_I2C_WATCHDOG_PERIOD__REG           BMA2XX_SERIAL_CTRL_REG

/* I2C WATCHDOG ENABLE */
#define BMA2XX_EN_I2C_WATCHDOG__POS               2
#define BMA2XX_EN_I2C_WATCHDOG__LEN               1
#define BMA2XX_EN_I2C_WATCHDOG__MSK               0x04
#define BMA2XX_EN_I2C_WATCHDOG__REG               BMA2XX_SERIAL_CTRL_REG

/*******************************0x36**************************************************/
/**    OFFSET  COMPENSATION     **/
/**    SLOW COMPENSATION FOR X,Y,Z AXIS      **/
#define BMA2XX_EN_SLOW_COMP_X__POS              0
#define BMA2XX_EN_SLOW_COMP_X__LEN              1
#define BMA2XX_EN_SLOW_COMP_X__MSK              0x01
#define BMA2XX_EN_SLOW_COMP_X__REG              BMA2XX_OFFSET_CTRL_REG

#define BMA2XX_EN_SLOW_COMP_Y__POS              1
#define BMA2XX_EN_SLOW_COMP_Y__LEN              1
#define BMA2XX_EN_SLOW_COMP_Y__MSK              0x02
#define BMA2XX_EN_SLOW_COMP_Y__REG              BMA2XX_OFFSET_CTRL_REG

#define BMA2XX_EN_SLOW_COMP_Z__POS              2
#define BMA2XX_EN_SLOW_COMP_Z__LEN              1
#define BMA2XX_EN_SLOW_COMP_Z__MSK              0x04
#define BMA2XX_EN_SLOW_COMP_Z__REG              BMA2XX_OFFSET_CTRL_REG

/**    FAST COMPENSATION READY FLAG          **/
//ADDED
#define BMA2XX_FAST_CAL_RDY_S__POS              4
#define BMA2XX_FAST_CAL_RDY_S__LEN              1
#define BMA2XX_FAST_CAL_RDY_S__MSK              0x10
#define BMA2XX_FAST_CAL_RDY_S__REG              BMA2XX_OFFSET_CTRL_REG

/**    FAST COMPENSATION FOR X,Y,Z AXIS      **/
#define BMA2XX_CAL_TRIGGER__POS                 5
#define BMA2XX_CAL_TRIGGER__LEN                 2
#define BMA2XX_CAL_TRIGGER__MSK                 0x60
#define BMA2XX_CAL_TRIGGER__REG                 BMA2XX_OFFSET_CTRL_REG

/**    RESET OFFSET REGISTERS                **/
#define BMA2XX_RESET_OFFSET_REGS__POS           7
#define BMA2XX_RESET_OFFSET_REGS__LEN           1
#define BMA2XX_RESET_OFFSET_REGS__MSK           0x80
#define BMA2XX_RESET_OFFSET_REGS__REG           BMA2XX_OFFSET_CTRL_REG


/*******************************0x37**************************************************/
/**     SLOW COMPENSATION  CUTOFF               **/
#define BMA2XX_COMP_CUTOFF__POS                 0
#define BMA2XX_COMP_CUTOFF__LEN                 1
#define BMA2XX_COMP_CUTOFF__MSK                 0x01
#define BMA2XX_COMP_CUTOFF__REG                 BMA2XX_OFFSET_PARAMS_REG

/**     COMPENSATION TARGET                  **/
#define BMA2XX_COMP_TARGET_OFFSET_X__POS        1
#define BMA2XX_COMP_TARGET_OFFSET_X__LEN        2
#define BMA2XX_COMP_TARGET_OFFSET_X__MSK        0x06
#define BMA2XX_COMP_TARGET_OFFSET_X__REG        BMA2XX_OFFSET_PARAMS_REG

#define BMA2XX_COMP_TARGET_OFFSET_Y__POS        3
#define BMA2XX_COMP_TARGET_OFFSET_Y__LEN        2
#define BMA2XX_COMP_TARGET_OFFSET_Y__MSK        0x18
#define BMA2XX_COMP_TARGET_OFFSET_Y__REG        BMA2XX_OFFSET_PARAMS_REG

#define BMA2XX_COMP_TARGET_OFFSET_Z__POS        5
#define BMA2XX_COMP_TARGET_OFFSET_Z__LEN        2
#define BMA2XX_COMP_TARGET_OFFSET_Z__MSK        0x60
#define BMA2XX_COMP_TARGET_OFFSET_Z__REG        BMA2XX_OFFSET_PARAMS_REG

/*******************************0x3E**************************************************/
/**     FIFO DATA SELECT              **/
//ADDED
#define BMA2XX_FIFO_DATA_SELECT__POS            0
#define BMA2XX_FIFO_DATA_SELECT__LEN            2
#define BMA2XX_FIFO_DATA_SELECT__MSK            0x03
#define BMA2XX_FIFO_DATA_SELECT__REG            BMA2XX_FIFO_MODE_REG

//FIFO MODE
#define BMA2XX_FIFO_MODE__POS                   6
#define BMA2XX_FIFO_MODE__LEN                   2
#define BMA2XX_FIFO_MODE__MSK                   0xC0
#define BMA2XX_FIFO_MODE__REG                   BMA2XX_FIFO_MODE_REG

/*********************************************************************************************/
/*********************************************************************************************/
#define BMA2XX_GET_BITSLICE(regvar, bitname)\
                        (regvar & bitname##__MSK) >> bitname##__POS


#define BMA2XX_SET_BITSLICE(regvar, bitname, val)\
                  (regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK)


/** \endcond */
/* CONSTANTS */
#define BMA2XX_STATUS1                             0               /**< It refers BMA2XX STATUS_INT1 */
#define BMA2XX_STATUS2                             1               /**< It refers BMA2XX STATUS_INT2 */
#define BMA2XX_STATUS3                             2               /**< It refers BMA2XX STATUS_INT_TAP */
#define BMA2XX_STATUS4                             3               /**< It refers BMA2XX STATUS_INT_ORIENT */
#define BMA2XX_STATUS5                             4               /**< It refers BMA2XX STATUS_INT_FIFO */

#define E_BMA2XX_OUT_OF_RANGE                      (char)-2

/*set shadow dis*/
#define MSB_IS_LOCKED    0
#define MSB_NO_Locking   1

/*set high bandwidth*/
#define Filtered_Low_Bandwidth      0
#define Unfiltered_HIGH_Bandwidth   1

/* range and bandwidth */
#define BMA2XX_RANGE_2G                 3 /**< sets range to +/- 2G mode \see BMA2XX_set_range() */
#define BMA2XX_RANGE_4G                 5 /**< sets range to +/- 4G mode \see BMA2XX_set_range() */
#define BMA2XX_RANGE_8G                 8 /**< sets range to +/- 8G mode \see BMA2XX_set_range() */
#define BMA2XX_RANGE_16G                12/**< sets range to +/- 16G mode \see BMA2XX_set_range() */

#define BMA2XX_BW_7_81HZ        0x08       /**< sets bandwidth to LowPass 7.81  HZ \see BMA2XX_set_bandwidth() */
#define BMA2XX_BW_15_63HZ       0x09       /**< sets bandwidth to LowPass 15.63 HZ \see BMA2XX_set_bandwidth() */
#define BMA2XX_BW_31_25HZ       0x0A       /**< sets bandwidth to LowPass 31.25 HZ \see BMA2XX_set_bandwidth() */
#define BMA2XX_BW_62_50HZ       0x0B       /**< sets bandwidth to LowPass 62.50 HZ \see BMA2XX_set_bandwidth() */
#define BMA2XX_BW_125HZ         0x0C       /**< sets bandwidth to LowPass 125HZ \see BMA2XX_set_bandwidth() */
#define BMA2XX_BW_250HZ         0x0D       /**< sets bandwidth to LowPass 250HZ \see BMA2XX_set_bandwidth() */
#define BMA2XX_BW_500HZ         0x0E       /**< sets bandwidth to LowPass 500HZ \see BMA2XX_set_bandwidth() */
#define BMA2XX_BW_1000HZ        0x0F       /**< sets bandwidth to LowPass 1000HZ \see BMA2XX_set_bandwidth() */

/*        SLEEP DURATION              */
#define BMA2XX_SLEEP_DUR_0_5MS        0x05       /* sets sleep duration to 0.5 ms  */
#define BMA2XX_SLEEP_DUR_1MS          0x06       /* sets sleep duration to 1 ms */
#define BMA2XX_SLEEP_DUR_2MS          0x07       /* sets sleep duration to 2 ms */
#define BMA2XX_SLEEP_DUR_4MS          0x08       /* sets sleep duration to 4 ms */
#define BMA2XX_SLEEP_DUR_6MS          0x09       /* sets sleep duration to 6 ms*/
#define BMA2XX_SLEEP_DUR_10MS         0x0A       /* sets sleep duration to 10 ms */
#define BMA2XX_SLEEP_DUR_25MS         0x0B       /* sets sleep duration to 25 ms */
#define BMA2XX_SLEEP_DUR_50MS         0x0C       /* sets sleep duration to 50 ms */
#define BMA2XX_SLEEP_DUR_100MS        0x0D       /* sets sleep duration to 100 ms */
#define BMA2XX_SLEEP_DUR_500MS        0x0E       /* sets sleep duration to 500 ms */
#define BMA2XX_SLEEP_DUR_1S           0x0F       /* sets sleep duration to 1 s */

/*        LATCH DURATION              */
#define BMA2XX_LATCH_DUR_NON_LATCH    0x00       /* sets LATCH duration to NON LATCH  */
#define BMA2XX_LATCH_DUR_250MS        0x01       /* sets LATCH duration to 250 ms */
#define BMA2XX_LATCH_DUR_500MS        0x02       /* sets LATCH duration to 500 ms */
#define BMA2XX_LATCH_DUR_1S           0x03       /* sets LATCH duration to 1 s */
#define BMA2XX_LATCH_DUR_2S           0x04       /* sets LATCH duration to 2 s*/
#define BMA2XX_LATCH_DUR_4S           0x05       /* sets LATCH duration to 4 s */
#define BMA2XX_LATCH_DUR_8S           0x06       /* sets LATCH duration to 8 s */
#define BMA2XX_LATCH_DUR_LATCH        0x07       /* sets LATCH duration to LATCH */
#define BMA2XX_LATCH_DUR_NON_LATCH1   0x08       /* sets LATCH duration to NON LATCH1 */
#define BMA2XX_LATCH_DUR_250US        0x09       /* sets LATCH duration to 250 Us */
#define BMA2XX_LATCH_DUR_500US        0x0A       /* sets LATCH duration to 500 Us */
#define BMA2XX_LATCH_DUR_1MS          0x0B       /* sets LATCH duration to 1 Ms */
#define BMA2XX_LATCH_DUR_12_5MS       0x0C       /* sets LATCH duration to 12.5 Ms */
#define BMA2XX_LATCH_DUR_25MS         0x0D       /* sets LATCH duration to 25 Ms */
#define BMA2XX_LATCH_DUR_50MS         0x0E       /* sets LATCH duration to 50 Ms */
#define BMA2XX_LATCH_DUR_LATCH1       0x0F       /* sets LATCH duration to LATCH*/

#define MS_ACC_REG		              0x02

/* mode settings */
#define BMA2XX_MODE_NORMAL            0
#define BMA2XX_MODE_LOWPOWER1         1
#define BMA2XX_MODE_SUSPEND           2
#define BMA2XX_MODE_DEEP_SUSPEND      3
#define BMA2XX_MODE_LOWPOWER2         4
#define BMA2XX_MODE_STANDBY           5

/* BMA2XX AXIS      */
#define BMA2XX_X_AXIS                 0          /**< It refers BMA2XX X-axis */
#define BMA2XX_Y_AXIS                 1         /**< It refers BMA2XX Y-axis */
#define BMA2XX_Z_AXIS                 2         /**< It refers BMA2XX Z-axis */

/*  INTERRUPT TYPES    */
#define BMA2XX_Low_G_Interrupt        0
#define BMA2XX_High_G_X_Interrupt     1
#define BMA2XX_High_G_Y_Interrupt     2
#define BMA2XX_High_G_Z_Interrupt     3
#define BMA2XX_DATA_EN                4
#define BMA2XX_Slope_X_Interrupt      5
#define BMA2XX_Slope_Y_Interrupt      6
#define BMA2XX_Slope_Z_Interrupt      7
#define BMA2XX_Single_Tap_Interrupt   8
#define BMA2XX_Double_Tap_Interrupt   9
#define BMA2XX_Orient_Interrupt       10
#define BMA2XX_Flat_Interrupt         11
#define BMA2XX_FFULL_INTERRUPT        12
#define BMA2XX_FWM_INTERRUPT          13

/*  INTERRUPTS PADS  */
#define BMA2XX_INT1_LOWG              0
#define BMA2XX_INT2_LOWG              1
#define BMA2XX_INT1_HIGHG             0
#define BMA2XX_INT2_HIGHG             1
#define BMA2XX_INT1_SLOPE             0
#define BMA2XX_INT2_SLOPE             1
#define BMA2XX_INT1_SLO_NO_MOT        0
#define BMA2XX_INT2_SLO_NO_MOT        1
#define BMA2XX_INT1_DTAP              0
#define BMA2XX_INT2_DTAP              1
#define BMA2XX_INT1_STAP              0
#define BMA2XX_INT2_STAP              1
#define BMA2XX_INT1_ORIENT            0
#define BMA2XX_INT2_ORIENT            1
#define BMA2XX_INT1_FLAT              0
#define BMA2XX_INT2_FLAT              1
#define BMA2XX_INT1_NDATA             0
#define BMA2XX_INT2_NDATA             1
#define BMA2XX_INT1_FWM               0
#define BMA2XX_INT2_FWM               1
#define BMA2XX_INT1_FFULL             0
#define BMA2XX_INT2_FFULL             1

/*       SOURCE REGISTER        */
#define BMA2XX_SRC_LOWG         0
#define BMA2XX_SRC_HIGHG        1
#define BMA2XX_SRC_SLOPE        2
#define BMA2XX_SRC_SLO_NO_MOT   3
#define BMA2XX_SRC_TAP          4
#define BMA2XX_SRC_DATA         5

#define BMA2XX_INT1_OUTPUT      0
#define BMA2XX_INT2_OUTPUT      1
#define BMA2XX_INT1_LEVEL       0
#define BMA2XX_INT2_LEVEL       1

/*    DURATION         */
#define BMA2XX_LOW_DURATION             0
#define BMA2XX_HIGH_DURATION            1
#define BMA2XX_SLOPE_DURATION           2
#define BMA2XX_SLO_NO_MOT_DURATION      3

/*      THRESHOLD        */
#define BMA2XX_LOW_THRESHOLD            0
#define BMA2XX_HIGH_THRESHOLD           1
#define BMA2XX_SLOPE_THRESHOLD          2
#define BMA2XX_SLO_NO_MOT_THRESHOLD     3

#define BMA2XX_LOWG_HYST                0
#define BMA2XX_HIGHG_HYST               1

#define BMA2XX_ORIENT_THETA             0
#define BMA2XX_FLAT_THETA               1

#define BMA2XX_I2C_SELECT               0
#define BMA2XX_I2C_EN                   1

/*    COMPENSATION           */
#define BMA2XX_SLOW_COMP_X              0
#define BMA2XX_SLOW_COMP_Y              1
#define BMA2XX_SLOW_COMP_Z              2

/*       OFFSET TRIGGER          */
#define BMA2XX_CUT_OFF                  0
#define BMA2XX_OFFSET_TRIGGER_X         1
#define BMA2XX_OFFSET_TRIGGER_Y         2
#define BMA2XX_OFFSET_TRIGGER_Z         3

/*       GP REGISTERS           */
#define BMA2XX_GP0                      0
#define BMA2XX_GP1                      1

/*    SLO NO MOT REGISTER          */
#define BMA2XX_SLO_NO_MOT_EN_X          0
#define BMA2XX_SLO_NO_MOT_EN_Y          1
#define BMA2XX_SLO_NO_MOT_EN_Z          2
#define BMA2XX_SLO_NO_MOT_EN_SEL        3

/* wake up */
#define BMA2XX_WAKE_UP_DUR_20MS         0
#define BMA2XX_WAKE_UP_DUR_80MS         1
#define BMA2XX_WAKE_UP_DUR_320MS        2
#define BMA2XX_WAKE_UP_DUR_2560MS       3

/* LG/HG thresholds are in LSB and depend on RANGE setting */
/* no range check on threshold calculation */
#define BMA2XX_SELF_TEST0_ON            1
#define BMA2XX_SELF_TEST1_ON            2

#define BMA2XX_EE_W_OFF                 0
#define BMA2XX_EE_W_ON                  1

/* Resolution Settings */
#define BMA2XX_RESOLUTION_10_BIT        1
#define BMA2XX_RESOLUTION_8_BIT         2
#define BMA2XX_RESOLUTION_14_BIT        3

/** Macro to convert floating point low-g-thresholds in G to 8-bit register values.<br>
  * Example: BMA2XX_LOW_TH_IN_G( 0.3, 2.0) generates the register value for 0.3G threshold in 2G mode.
  * \brief convert g-values to 8-bit value
 */
#define BMA2XX_LOW_TH_IN_G( gthres, range)                       ((256 * gthres ) / range)

/** Macro to convert floating point high-g-thresholds in G to 8-bit register values.<br>
  * Example: BMA2XX_HIGH_TH_IN_G( 1.4, 2.0) generates the register value for 1.4G threshold in 2G mode.
  * \brief convert g-values to 8-bit value
 */
#define BMA2XX_HIGH_TH_IN_G(gthres, range)                       ((256 * gthres ) / range)

/** Macro to convert floating point low-g-hysteresis in G to 8-bit register values.<br>
  * Example: BMA2XX_LOW_HY_IN_G( 0.2, 2.0) generates the register value for 0.2G threshold in 2G mode.
  * \brief convert g-values to 8-bit value
 */
#define BMA2XX_LOW_HY_IN_G( ghyst, range )                        ((32 * ghyst) / range)

/** Macro to convert floating point high-g-hysteresis in G to 8-bit register values.<br>
  * Example: BMA2XX_HIGH_HY_IN_G( 0.2, 2.0) generates the register value for 0.2G threshold in 2G mode.
  * \brief convert g-values to 8-bit value
 */
#define BMA2XX_HIGH_HY_IN_G( ghyst, range )                       ((32 * ghyst) / range)

/** Macro to convert floating point G-thresholds to 8-bit register values<br>
  * Example: BMA2XX_SLOPE_TH_IN_G( 1.2, 2.0) generates the register value for 1.2G threshold in 2G mode.
  * \brief convert g-values to 8-bit value
 */
#define BMA2XX_SLOPE_TH_IN_G( gthres, range)                      ((128 * gthres ) / range)

/*****************************************************
	ADC value configure of 0g 1g and -1g in X,Y,Z axis
******************************************************/				
#define ACC_0G_X      (2048) 			
#define ACC_1G_X      (2048+(15<<6))
#define ACC_MINUS1G_X (2048-(15<<6))
#define ACC_0G_Y      (2048)
#define ACC_1G_Y      (2048+(15<<6))
#define ACC_MINUS1G_Y (2048-(15<<6))
#define ACC_0G_Z      (2048)
#define ACC_1G_Z      (2048+(15<<6))
#define ACC_MINUS1G_Z (2048-(15<<6))

typedef struct
{
   short x, /**< holds x-axis acceleration data sign extended. Range -512 to 511. */
         y, /**< holds y-axis acceleration data sign extended. Range -512 to 511. */
         z; /**< holds z-axis acceleration data sign extended. Range -512 to 511. */
} BMA2XXacc_t;
   
/******************************************************
	Global struct variable
*******************************************************/
typedef struct {
  kal_uint16  X;
  kal_uint16  Y;
  kal_uint16  Z;  
} SPARAMETERS;



kal_uint8 BMA2XX_ReadID(void);
void BMA2XX_init(void);
void BMA2XX_ReadXYZ(kal_uint16 *X, kal_uint16 *Y, kal_uint16 *Z);
kal_uint8  Motion_detection_deal(void);
extern MotionSensor_customize_function_struct *ms_GetFunc(void);




#endif	//_MOTION_SENSOR_CUSTOM_H

