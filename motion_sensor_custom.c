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
 *    motion_sensor_custom.c
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
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#if defined(MOTION_SENSOR_SUPPORT)

#include "kal_release.h"
#include "custom_config.h"
#include "gpio_sw.h"
#include "drv_comm.h"
#include "eint.h"
#include "motion_sensor.h"
#include "motion_sensor_custom.h"
#include "motion_sensor_hw_define.h"
#include "MMI_features.h"
#include "kal_trace.h"
#include "us_timer.h"
#include "dcl.h"
#include "intrCtrl.h"
#include "TimerEvents.h"

/******************************************
	customizaton data
*******************************************/
MotionSensor_custom_data_struct  ms_custom_data_def = 
{          
   	/*X axis*/
   	ACC_0G_X,   
   	ACC_1G_X,
   	ACC_MINUS1G_X,     
    /*Y axis*/
	ACC_0G_Y,   
   	ACC_1G_Y,
	ACC_MINUS1G_Y,     
	/*Z axis*/
	ACC_0G_Z,   
   	ACC_1G_Z,
	ACC_MINUS1G_Z    
};	

/*motion sensor pwr on flag*/
volatile kal_bool g_ms_is_power_on = KAL_TRUE;
extern const unsigned char  MOTION_SENSOR_EINT_NO;
/*motion sensor cord mapping define */
/******************************************
XY_MIRROR_MAPPING 	for x cord and y cord mirror mapping
XY_90_MAPPING		for x cord and y cord clockwise 90 mapping
XY_180_MAPPING		for x cord and y cord clockwise 180 mapping
XY_270_MAPPING		for x cord and y cord clockwise 270 mapping
******************************************/
//#define XY_MIRROR_MAPPING
//#define Y_MINUS_MAPPING

/******************************************
	Debug API for motion sensor
*******************************************/
#define DBG_MOTION_SENSOR

void test_dbg_print(char * fm, ...){}

#define dbg_print test_dbg_print

#ifdef DBG_MOTION_SENSOR
#define ms_dbg_print kal_prompt_trace
#define ms_dbg_print_ps dbg_print
#else
#define ms_dbg_print(...)
#define ms_dbg_print_ps(...)
#endif

/***************************************************/
/*			software I2C APIs for motion sensor    */
/***************************************************/

/******************************************
	us delay for software I2C
*******************************************/
void SW_i2c_udelay(kal_uint32 delay)
{
	kal_uint32 ust = 0; //ust_get_current_time
	kal_uint32 count = 0;
	kal_uint32 break_count = 0;
	
	ust = ust_get_current_time();
	do{
		if(ust_get_current_time() != ust)
			count++;
		else
			break_count++;
	}while((count < delay) && (break_count < 0xFFFFFF));
}

/******************************************
	software I2C start bit
*******************************************/
void SW_i2c_start(void)
{
	MS_CLK_PIN_GPIO_MODE;
	MS_I2C_CLK_OUTPUT;

	MS_DATA_PIN_GPIO_MODE;
	MS_I2C_DATA_OUTPUT;
	
	MS_I2C_DATA_HIGH;
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(40);		//20
	MS_I2C_DATA_LOW;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_LOW;
	SW_i2c_udelay(20);		//10
}

/******************************************
	software I2C stop bit
*******************************************/
void SW_i2c_stop(void)
{
	MS_I2C_CLK_OUTPUT;
	MS_I2C_DATA_OUTPUT;
	
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(20);		//10
	MS_I2C_DATA_HIGH;
}

/******************************************
	software I2C one clock
*******************************************/
void SW_i2c_one_clk(void)
{
	SW_i2c_udelay(10);		//5
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_LOW;
	SW_i2c_udelay(10);		//5
}

/******************************************
	software I2C read byte with ack
*******************************************/
kal_uint8 ms_ReadByteAck(void)
{
	kal_int8 i;
	kal_uint8 data;

	MS_I2C_DATA_INPUT; 
	data = 0; 
	
	for (i=7; i>=0; i--) 
	{
		if (MS_I2C_GET_BIT)
		{
			data |= (0x01<<i);
		}
		SW_i2c_one_clk();
	}			                                

	MS_I2C_DATA_OUTPUT;                    
	MS_I2C_DATA_LOW;                       
	SW_i2c_one_clk();                         

	return data;
}

/******************************************
	software I2C read byte without ack
*******************************************/
kal_uint8 ms_ReadByteNAck(void)
{
	kal_int8 i;
	kal_uint8 data;

	MS_I2C_DATA_INPUT; 
	data = 0; 
	
	for (i=7; i>=0; i--) 
	{
		if (MS_I2C_GET_BIT)
		{
			data |= (0x01<<i);
		}
		SW_i2c_one_clk();
	}			                                

	MS_I2C_DATA_OUTPUT;                                           
	MS_I2C_DATA_HIGH;
	SW_i2c_one_clk();                         
	
	return data;
}

/******************************************
	software I2C send byte
*******************************************/
void ms_SendByte(kal_uint8 sData) 
{
	kal_int8 i;
	
	for (i=7; i>=0; i--) 
	{            
		if ((sData>>i)&0x01) 
		{               
			MS_I2C_DATA_HIGH;	              
		} 
		else 
		{ 
			MS_I2C_DATA_LOW;                  
		}
		SW_i2c_one_clk();                        
	}		
}
/******************************************
	software I2C check ack bit
*******************************************/
kal_bool ms_ChkAck(void)
{
	MS_I2C_DATA_INPUT;
	SW_i2c_udelay(10);		//5
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(10);		//5

	if(MS_I2C_GET_BIT)		//Non-ack
	{
		SW_i2c_udelay(10);	//5
		MS_I2C_CLK_LOW;
		SW_i2c_udelay(10);	//5
		MS_I2C_DATA_OUTPUT;
		MS_I2C_DATA_LOW;
		
		return KAL_FALSE;
	}
	else					//Ack
	{
		SW_i2c_udelay(10);	//5
		MS_I2C_CLK_LOW;
		SW_i2c_udelay(10);	//5
		MS_I2C_DATA_OUTPUT;
		MS_I2C_DATA_LOW;

		return KAL_TRUE;
	}
}

/******************************************
	software I2C restart bit
*******************************************/
void ms_Restart(void)
{
	MS_I2C_CLK_OUTPUT;
	MS_I2C_DATA_OUTPUT;

	SW_i2c_udelay(40);
	MS_I2C_DATA_HIGH;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(40);
	MS_I2C_DATA_LOW;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_LOW;
	SW_i2c_udelay(20);		//10
}

/***************************************************/
/*			motion sensor read and write APIs 	   */
/***************************************************/
/******************************************
	BMA2XX ms delay function  
*******************************************/
void BMA2XX_DelayMS(kal_uint16 delay)
{
	kal_uint16 i=0;

	for(i=0; i<delay; i++)
	{
		SW_i2c_udelay(1000);
	}
}

/******************************************
	         BMA2XX read bytes
*******************************************/
kal_bool BMA2XX_ReadBytes(kal_uint8* Data, kal_uint8 RegAddr)
{
	SW_i2c_start();						//start bit
	ms_SendByte(MS_SLAVEADDR_W);		//slave address|write bit
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display ack check fail when send write id		
		ms_dbg_print(MOD_AUX, "ack fail when send wid in readbytes");
		ms_dbg_print_ps("ack fail when send wid in readbytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}
		
	ms_SendByte(RegAddr);				//send RegAddr
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display ack check fail when send RegAddr
		ms_dbg_print(MOD_AUX, "ack fail when send Reg in readbytes");
		ms_dbg_print_ps("ack fail when send Reg in readbytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	ms_Restart();						//restart bit

	ms_SendByte(MS_SLAVEADDR_R);		//slave address|read bit
	if(KAL_FALSE == ms_ChkAck())
	{
		//TO_DO: display ack check fail when send read id		
		ms_dbg_print(MOD_AUX, "ack fail when send rid in readbytes");
		ms_dbg_print_ps("ack fail when send rid in readbytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	*Data = ms_ReadByteNAck();

	SW_i2c_stop();						//stop bit

	//TO_DO: add debug code to display the data received

	return KAL_TRUE;
}

/******************************************
	BMA2XX contunious read mul bytes
*******************************************/
kal_bool BMA2XX_ReadBytes_Mul(kal_uint8* Data, kal_uint8 RegAddr, kal_uint16 Length)
{
	kal_uint8* Data_ptr;
	kal_uint16 i;

	Data_ptr = Data;
	
	SW_i2c_start();						//start bit
	ms_SendByte(MS_SLAVEADDR_W);		//slave address|write bit
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display ack check fail when send write id		
		ms_dbg_print(MOD_AUX, "ack fail when send wid in conread");
		ms_dbg_print_ps("ack fail when send wid in conread\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}
		
	ms_SendByte(RegAddr);				//send RegAddr
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display ack check fail when send RegAddr		
		ms_dbg_print(MOD_AUX, "ack fail when send Reg in conread");
		ms_dbg_print_ps("ack fail when send Reg in conread\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	ms_Restart();						//restart bit

	ms_SendByte(MS_SLAVEADDR_R);		//slave address|read bit
	if(KAL_FALSE == ms_ChkAck())
	{
		//TO_DO: display ack check fail when send read id		
		ms_dbg_print(MOD_AUX, "ack fail when send rid in conread");
		ms_dbg_print_ps("ack fail when send rid in conread\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	for(i= Length; i>1; i--)
	{
		*Data_ptr = ms_ReadByteAck();	//read byte with ack
		Data_ptr++;
	}
	
	*Data_ptr = ms_ReadByteNAck();		//read byte with non-ack to stop reading

	SW_i2c_stop();						//stop bit

	//TO_DO: add debug code to display the data received

	return KAL_TRUE;
}

/******************************************
	BMA2XX write bytes
*******************************************/
kal_bool BMA2XX_WriteBytes(kal_uint8 RegAddr, kal_uint8 Data)
{
	SW_i2c_start();						//start bit

	ms_SendByte(MS_SLAVEADDR_W);		//slave address|write bit
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display check ack fail when send write id
		ms_dbg_print(MOD_AUX, "ack fail when send wid in writebytes");		
		ms_dbg_print_ps("ack fail when send wid in writebytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	ms_SendByte(RegAddr);				//send RegAddr
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display check ack fail when send RegAddr
		ms_dbg_print(MOD_AUX, "ack fail when send Reg in wirtebytes");
		ms_dbg_print_ps("ack fail when send Reg in wirtebytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	ms_SendByte(Data);					//send parameter
	if(KAL_FALSE == ms_ChkAck())
	{
		//TO_DO: display check ack fail when send data
		ms_dbg_print(MOD_AUX, "ack fail when send para in writebytes");
		ms_dbg_print_ps("ack fail when send para in writebytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	SW_i2c_stop();						//stop bit

	return KAL_TRUE;
}

/******************************************
	BMA2XX contunious Write mul bytes
*******************************************/
kal_bool BMA2XX_WriteBytes_Mul(kal_uint8 RegAddr, kal_uint8 *Data, kal_uint16 Length)
{
	kal_uint16 i = 0;
	SW_i2c_start();						//start bit

	ms_SendByte(MS_SLAVEADDR_W);		//slave address|write bit
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display check ack fail when send write id
		ms_dbg_print(MOD_AUX, "ack fail when send wid in writebytes");		
		ms_dbg_print_ps("ack fail when send wid in writebytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	ms_SendByte(RegAddr);				//send RegAddr
	if(KAL_FALSE == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display check ack fail when send RegAddr
		ms_dbg_print(MOD_AUX, "ack fail when send Reg in wirtebytes");
		ms_dbg_print_ps("ack fail when send Reg in wirtebytes\n\r");
		SW_i2c_stop();
		return KAL_FALSE;
	}

	for(i=0; i<Length; i++)
	{
	    ms_SendByte(*Data++);					//send parameter
	    
		if(KAL_FALSE == ms_ChkAck())
		{
		 //TO_DO: display check ack fail when send data
		 ms_dbg_print(MOD_AUX, "ack fail when send para in writebytes");
		 ms_dbg_print_ps("ack fail when send para in writebytes\n\r");
		 SW_i2c_stop();
		 return KAL_FALSE;
		}
	}
	
	SW_i2c_stop();						//stop bit

	return KAL_TRUE;
}

						/*BMA2XX motion sensor driver APIs*/
/*******************************************************************************
 * Description: *//**\brief This API Reads tap slope status register byte
 *                          from location 0Bh
 *
 *					   è¯¥APIä»Žä½ç½®0Bhè¯»å–æŠ½å¤´æ–œçŽ‡çŠ¶æ€å¯„å­˜å™¨å­—èŠ‚
 *  \param unsigned char * status_tap : Address of status_tap register
 ******************************************************************************/
int BMA2XX_get_int_tap_status(unsigned char * status_tap)
{
	int comres = C_BMA2XX_Zero_U8X;

	comres = BMA2XX_ReadBytes_Mul(status_tap,BMA2XX_STATUS_TAP_SLOPE_REG, C_BMA2XX_One_U8X);
	
	return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API Reads orient status register byte
 *                          from location 0Ch
 *
 *					  è¯¥APIä»Žä½ç½®0Chè¯»å–å®šä½çŠ¶æ€å¯„å­˜å™¨å­—èŠ‚
 *  \param unsigned char *status_orient : Address of status_orient register
 ******************************************************************************/
int BMA2XX_get_int_orient_status(unsigned char *status_orient)
{
   int comres = C_BMA2XX_Zero_U8X;

   comres = BMA2XX_ReadBytes_Mul(status_orient, BMA2XX_STATUS_ORIENT_HIGH_REG,C_BMA2XX_One_U8X);
   
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API Reads interrupt status register byte
 *                          from location 09h
 *
 *					è¯¥APIä»Žä½ç½®09hè¯»å–ä¸­æ–­çŠ¶æ€å¯„å­˜å™¨å­—èŠ‚
 ******************************************************************************/
int BMA2XX_get_interrupt_status(unsigned char * status)
{
   int comres = C_BMA2XX_Zero_U8X;

   comres = BMA2XX_ReadBytes_Mul(status,BMA2XX_STATUS1_REG, C_BMA2XX_Four_U8X);

   return comres;
}

/**************************************************************************************
 * Description: *//**\brief This API is used to get the Ranges(g values) of the sensor
 *
 *					è¯¥APIç”¨äºŽèŽ·å–ä¼ æ„Ÿå™¨çš„èŒƒå›´ï¼ˆgå€¼ï¼‰0x0f
 *  \param unsigned char * Range : Address of Range
 *                        3 -> 2G
 *                        5 -> 4G
 *                        8 -> 8G
 *                       12 -> 16G
 ***************************************************************************************/
kal_bool BMA2XX_Get_Range(kal_uint8* BMA2XXRange)
{
	kal_bool temp_result = KAL_FALSE;

	temp_result = BMA2XX_ReadBytes(BMA2XXRange, BMA2XX_RANGE_SEL__REG);

	if(KAL_FALSE == temp_result)
	{
		ms_dbg_print_ps("BMA2XX get range fail!\n\r");
		return KAL_FALSE;
	}
	else
	{
		ms_dbg_print_ps("BMA2XX get range succeed!\n\r");
		return KAL_TRUE;
	}
}

/***********************************************************************************
 * Description: *//**\brief This API is used to set Ranges(g value) of the sensor
 *
 *				    è¯¥APIç”¨äºŽè®¾ç½®ä¼ æ„Ÿå™¨çš„èŒƒå›´ï¼ˆgå€¼ï¼‰0x0f
 *  \param unsigned char Range
 *                        3 -> 2G
 *                        5 -> 4G
 *                        8 -> 8G
 *                       12 -> 16G
 ************************************************************************************/
kal_bool BMA2XX_Set_Range(kal_uint8 BMA2XXRange)
{
	kal_bool temp_result = KAL_FALSE;
	
	if(BMA2XXRange > BMA2XX_RANGE_16G)
	{
		ASSERT(0);
	}
	
	temp_result = BMA2XX_WriteBytes(BMA2XX_RANGE_SEL__REG ,BMA2XXRange);
	if(KAL_FALSE == temp_result)
	{
		kal_prompt_trace(MOD_AUX,"BMA2XX set range:%x fail!\n\r", BMA2XXRange);
		return KAL_FALSE;
	}
	else
	{
		kal_prompt_trace(MOD_AUX,"BMA2XX set range:%x succeed!\n\r", BMA2XXRange);
		return KAL_TRUE;
	}
}

/***********************************************************************************
 * Description: *//**\brief This API is used to get the bandwidth of the sensor
 *
  *                        è¯¥APIç”¨äºŽèŽ·å–ä¼ æ„Ÿå™¨çš„å¸¦å®½ 0x10
 *  \param  unsigned char * BW : Address of * BW
 *                       8 -> 7.81HZ
 *                       9 -> 15.63HZ
 *                      10 -> 31.25HZ
 *                      11 -> 62.50HZ
 *                      12 -> 125HZ
 *                      13 -> 250HZ
 *                      14 -> 500HZ
 *                      15 -> 1000HZ
 ************************************************************************************/
int BMA2XX_get_bandwidth(unsigned char * BW)
{
   int comres = C_BMA2XX_Zero_U8X ;
   unsigned char data;
   {
    comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_BANDWIDTH__REG,C_BMA2XX_One_U8X );
    data = BMA2XX_GET_BITSLICE(data, BMA2XX_BANDWIDTH);
    *BW = data ;
   }
   return comres;
}


/*******************************************************************************
 * Description: *//**\brief This API is used to set Bandwidth of the sensor
 *
 *						  è¯¥APIç”¨äºŽè®¾ç½®ä¼ æ„Ÿå™¨çš„å¸¦å®½0x10
 *  \param unsigned char BW
 *                       8 -> 7.81HZ
 *                       9 -> 15.63HZ
 *                      10 -> 31.25HZ
 *                      11 -> 62.50HZ
 *                      12 -> 125HZ
 *                      13 -> 250HZ
 *                      14 -> 500HZ
 *                      15 -> 1000HZ
 ******************************************************************************/
int BMA2XX_set_bandwidth(unsigned char BW)
{
   int comres = C_BMA2XX_Zero_U8X ;
   unsigned char data;
   int Bandwidth ;
   {

        if (BW > C_BMA2XX_Seven_U8X && BW < C_BMA2XX_Fifteen_U8X)
        {
            switch (BW)
            {
               case BMA2XX_BW_7_81HZ:
                  Bandwidth = BMA2XX_BW_7_81HZ;

                  /*  7.81 Hz      64000 uS   */
                  break;
               case BMA2XX_BW_15_63HZ:
                  Bandwidth = BMA2XX_BW_15_63HZ;

                  /*  15.63 Hz     32000 uS   */
                  break;
               case BMA2XX_BW_31_25HZ:
                  Bandwidth = BMA2XX_BW_31_25HZ;

                  /*  31.25 Hz     16000 uS   */
                  break;
               case BMA2XX_BW_62_50HZ:
                  Bandwidth = BMA2XX_BW_62_50HZ;

                  /*  62.50 Hz     8000 uS   */
                  break;
               case BMA2XX_BW_125HZ:
                  Bandwidth = BMA2XX_BW_125HZ;

                  /*  125 Hz       4000 uS   */
                  break;
               case BMA2XX_BW_250HZ:
                  Bandwidth = BMA2XX_BW_250HZ;

                  /*  250 Hz       2000 uS   */
                  break;
               case BMA2XX_BW_500HZ:
                  Bandwidth = BMA2XX_BW_500HZ;

                  /*  500 Hz       1000 uS   */
                  break;
               default:
                  break;
            }
            comres = BMA2XX_ReadBytes_Mul(&data, BMA2XX_BANDWIDTH__REG,C_BMA2XX_One_U8X );
            data = BMA2XX_SET_BITSLICE(data, BMA2XX_BANDWIDTH, Bandwidth );
			ms_dbg_print(MOD_AUX, "Motion Sensor bandwidth %d\r\n",data);
            comres += BMA2XX_WriteBytes( BMA2XX_BANDWIDTH__REG,data);
        }
        else
        {
            comres = E_OUT_OF_RANGE ;
        }
   }
   return comres;
}

/***************************************************************************************
 * Description: *//**\brief This API is used to get the operating modes of the sensor
 *
 *							¸ÃAPIÓÃÓÚ»ñÈ¡´«¸ÐÆ÷µÄ²Ù×÷Ä£Ê½0x11
 *  \param unsigned char * Mode : Address of Mode
 *                       0 -> NORMAL
 *                       1 -> LOWPOWER1
 *                       2 -> SUSPEND
 *                       3 -> DEEP_SUSPEND
 *                       4 -> LOWPOWER2
 *                       5 -> STANDBY
 **************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_mode(unsigned char * Mode )
{
   int comres=C_BMA2XX_Zero_U8X ;
   unsigned char data1,data2;
   {
      comres = BMA2XX_ReadBytes_Mul( &data1, BMA2XX_MODE_CTRL_REG,C_BMA2XX_One_U8X );
      comres = BMA2XX_ReadBytes_Mul( &data2,BMA2XX_LOW_NOISE_CTRL_REG,C_BMA2XX_One_U8X );

      data1  = (data1 & 0xE0) >> 5;
      data2  = (data2 & 0x40) >> 6;

      //*Mode  = (*Mode) >> C_Six_U8X;
      if ((data1 == 0x00) && (data2 == 0x00))
      {
         *Mode  = BMA2XX_MODE_NORMAL;
      }
      else
      {
        if ((data1 == 0x02) && (data2 == 0x00))
        {
            *Mode  = BMA2XX_MODE_LOWPOWER1;
        }
        else
        {
            if ((data1 == 0x04 || data1 == 0x06) && (data2 == 0x00))
            {
               *Mode  = BMA2XX_MODE_SUSPEND;
            }
            else
            {
                if (((data1 & 0x01) == 0x01))
                {
                  *Mode  = BMA2XX_MODE_DEEP_SUSPEND;
                }
                else
                {
                    if ((data1 == 0x02) && (data2 == 0x01))
                    {
                        *Mode  = BMA2XX_MODE_LOWPOWER2;
                    }
                    else
                    {
                        if ((data1 == 0x04) && (data2 == 0x01))
                        {
                            *Mode  = BMA2XX_MODE_STANDBY;
                        }
                        else
                        {
                            *Mode = BMA2XX_MODE_DEEP_SUSPEND ;
                        }
                    }
                }
            }
        }
      }
   }
   return comres;
}

/***************************************************************************************
 * Description: *//**\brief This API is used to set the operating Modes of the sensor
 *
 *					            ¸ÃAPIÓÃÓÚÉèÖÃ´«¸ÐÆ÷µÄ¹¤×÷Ä£Ê½0x11
 *  \param unsigned char Mode
 *                       0 -> NORMAL
 *                       1 -> LOWPOWER1
 *                       2 -> SUSPEND
 *                       3 -> DEEP_SUSPEND
 *                       4 -> LOWPOWER2
 *                       5 -> STANDBY
 ***************************************************************************************/
unsigned char BMA2XX_set_mode(unsigned char Mode)
{
	int comres=C_BMA2XX_Zero_U8X;
	unsigned char data1,data2;
	{
	   if (Mode < C_BMA2XX_Six_U8X)
	   {
		   comres = BMA2XX_ReadBytes_Mul(&data1,BMA2XX_MODE_CTRL_REG, C_BMA2XX_One_U8X);
		   comres = BMA2XX_ReadBytes_Mul(&data2,BMA2XX_LOW_NOISE_CTRL_REG, C_BMA2XX_One_U8X);
		   switch (Mode)
           {
            case BMA2XX_MODE_NORMAL:
				data1  = BMA2XX_SET_BITSLICE(data1, BMA2XX_MODE_CTRL, C_BMA2XX_Zero_U8X);
				data2  = BMA2XX_SET_BITSLICE(data2, BMA2XX_LOW_POWER_MODE, C_BMA2XX_Zero_U8X);
				comres += BMA2XX_WriteBytes( BMA2XX_MODE_CTRL_REG, data1);
				BMA2XX_DelayMS(1);/*A minimum delay of atleast 450us is required for the low power modes, as per the data sheet.*/
				comres += BMA2XX_WriteBytes( BMA2XX_LOW_NOISE_CTRL_REG,data2);
				break;
            case BMA2XX_MODE_LOWPOWER1:
				data1  = BMA2XX_SET_BITSLICE(data1, BMA2XX_MODE_CTRL, C_BMA2XX_Two_U8X);
				data2  = BMA2XX_SET_BITSLICE(data2, BMA2XX_LOW_POWER_MODE, C_BMA2XX_Zero_U8X);
				comres += BMA2XX_WriteBytes( BMA2XX_MODE_CTRL_REG, data1);
				BMA2XX_DelayMS(1);/*A minimum delay of atleast 450us is required for the low power modes, as per the data sheet.*/
				comres += BMA2XX_WriteBytes( BMA2XX_LOW_NOISE_CTRL_REG, data2);
				break;
			case BMA2XX_MODE_SUSPEND:
				data1  = BMA2XX_SET_BITSLICE(data1, BMA2XX_MODE_CTRL, C_BMA2XX_Four_U8X);
				data2  = BMA2XX_SET_BITSLICE(data2, BMA2XX_LOW_POWER_MODE, C_BMA2XX_Zero_U8X);
				comres += BMA2XX_WriteBytes( BMA2XX_LOW_NOISE_CTRL_REG, data2);
				BMA2XX_DelayMS(1);/*A minimum delay of atleast 450us is required for the low power modes, as per the data sheet.*/
				comres += BMA2XX_WriteBytes( BMA2XX_MODE_CTRL_REG, data1);
               break;
			case BMA2XX_MODE_DEEP_SUSPEND:
				data1  = BMA2XX_SET_BITSLICE(data1, BMA2XX_MODE_CTRL, C_BMA2XX_One_U8X);
				data2  = BMA2XX_SET_BITSLICE(data2, BMA2XX_LOW_POWER_MODE, C_BMA2XX_One_U8X);
				comres += BMA2XX_WriteBytes( BMA2XX_MODE_CTRL_REG, data1);
				BMA2XX_DelayMS(1);/*A minimum delay of atleast 450us is required for the low power modes, as per the data sheet.*/
				comres += BMA2XX_WriteBytes( BMA2XX_LOW_NOISE_CTRL_REG, data2);
				break;
			case BMA2XX_MODE_LOWPOWER2:
				data1  = BMA2XX_SET_BITSLICE(data1, BMA2XX_MODE_CTRL, C_BMA2XX_Two_U8X);
				data2  = BMA2XX_SET_BITSLICE(data2, BMA2XX_LOW_POWER_MODE, C_BMA2XX_One_U8X);
				comres += BMA2XX_WriteBytes( BMA2XX_MODE_CTRL_REG, data1);
				BMA2XX_DelayMS(1);/*A minimum delay of atleast 450us is required for the low power modes, as per the data sheet.*/
				comres += BMA2XX_WriteBytes( BMA2XX_LOW_NOISE_CTRL_REG, data2);
				break;
			case BMA2XX_MODE_STANDBY:
				data1  = BMA2XX_SET_BITSLICE(data1, BMA2XX_MODE_CTRL, C_BMA2XX_Four_U8X);
				data2  = BMA2XX_SET_BITSLICE(data2, BMA2XX_LOW_POWER_MODE, C_BMA2XX_One_U8X);
				comres += BMA2XX_WriteBytes( BMA2XX_LOW_NOISE_CTRL_REG, data2);
				BMA2XX_DelayMS(1);/*A minimum delay of atleast 450us is required for the low power modes, as per the data sheet.*/
				comres += BMA2XX_WriteBytes( BMA2XX_MODE_CTRL_REG, data1);
				break;
		   }
		}
		else
        {
	        comres = E_OUT_OF_RANGE ;
        }
	}
	return comres;
}

/********************************************************************************************
 * Description: *//**\brief This API is used to get the sleep duration status of the sensor
 *
 *						è¯¥APIç”¨äºŽèŽ·å–ä¼ æ„Ÿå™¨çš„ç¡çœ æŒç»­æ—¶é—´çŠ¶æ€0x11
 *  \param  unsigned char *sleep_dur : Address of sleep_dur
 *                       5 -> 0.5MS
 *                       6 -> 1MS
 *                       7 -> 2MS
 *                       8 -> 4MS
 *                       9 -> 6MS
 *                      10 -> 10MS
 *                      11 -> 25MS
 *                      12 -> 50MS
 *                      13 -> 100MS
 *                      14 -> 500MS
 *                      15 -> 1S
 *********************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_sleep_dur(unsigned char *sleep_dur)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      //SLEEP DURATION
      comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_SLEEP_DUR__REG,C_BMA2XX_One_U8X);
      *sleep_dur = BMA2XX_GET_BITSLICE(data,BMA2XX_SLEEP_DUR);
   }
   return comres;
}

/************************************************************************************
 * Description: *//**\brief This API is used to set Sleep Duration of the sensor
 *
 *							è¯¥APIç”¨äºŽè®¾ç½®ä¼ æ„Ÿå™¨çš„ç¡çœ æŒç»­æ—¶é—´0x11
 *  \param unsigned char sleep_dur
 *                       5 -> 0.5MS
 *                       6 -> 1MS
 *                       7 -> 2MS
 *                       8 -> 4MS
 *                       9 -> 6MS
 *                      10 -> 10MS
 *                      11 -> 25MS
 *                      12 -> 50MS
 *                      13 -> 100MS
 *                      14 -> 500MS
 *                      15 -> 1S
 **************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_sleep_dur (unsigned char sleep_dur)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X ;
   int sleep_duration;
   {
      if (sleep_dur > C_BMA2XX_Four_U8X && sleep_dur < C_BMA2XX_Sixteen_U8X)
      {
        switch (sleep_dur)
        {
            case BMA2XX_SLEEP_DUR_0_5MS:
               sleep_duration = BMA2XX_SLEEP_DUR_0_5MS;

               /*  0.5 MS   */
               break;
            case BMA2XX_SLEEP_DUR_1MS:
               sleep_duration = BMA2XX_SLEEP_DUR_1MS;

               /*  1 MS  */
               break;
            case BMA2XX_SLEEP_DUR_2MS:
               sleep_duration = BMA2XX_SLEEP_DUR_2MS;

               /*  2 MS  */
               break;
            case BMA2XX_SLEEP_DUR_4MS:
               sleep_duration = BMA2XX_SLEEP_DUR_4MS;

               /*  4 MS   */
               break;
            case BMA2XX_SLEEP_DUR_6MS:
               sleep_duration = BMA2XX_SLEEP_DUR_6MS;

               /*  6 MS  */
               break;
            case BMA2XX_SLEEP_DUR_10MS:
               sleep_duration = BMA2XX_SLEEP_DUR_10MS;

               /*  10 MS  */
               break;
            case BMA2XX_SLEEP_DUR_25MS:
               sleep_duration = BMA2XX_SLEEP_DUR_25MS;

               /*  25 MS  */
               break;
            case BMA2XX_SLEEP_DUR_50MS:
               sleep_duration = BMA2XX_SLEEP_DUR_50MS;

               /*  50 MS   */
               break;
            case BMA2XX_SLEEP_DUR_100MS:
               sleep_duration = BMA2XX_SLEEP_DUR_100MS;

               /*  100 MS  */
               break;
            case BMA2XX_SLEEP_DUR_500MS:
               sleep_duration = BMA2XX_SLEEP_DUR_500MS;

               /*  500 MS   */
               break;
            case BMA2XX_SLEEP_DUR_1S:
               sleep_duration = BMA2XX_SLEEP_DUR_1S;

               /*  1 SECS   */
               break;
            default:
               break;
        }
        //SLEEP DURATION
        comres = BMA2XX_ReadBytes_Mul(&data, BMA2XX_SLEEP_DUR__REG,C_BMA2XX_One_U8X);
        data = BMA2XX_SET_BITSLICE(data, BMA2XX_SLEEP_DUR, sleep_duration);
        comres = BMA2XX_WriteBytes( BMA2XX_SLEEP_DUR__REG, data);
      }
      else
      {
         comres = E_OUT_OF_RANGE ;
      }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get the sleep timer mode status
 *
 *				           ¸ÃAPIÓÃÓÚ»ñÈ¡Ë¯Ãß¶¨Ê±Æ÷Ä£Ê½×´Ì¬ 0x11
 *  \param  unsigned char *sleep_tmr : Address of sleep_tmr
 *                  sleep_tmr -> [0:1]
 *                  0 => enable EventDrivenSampling(EDT)
 *                  1 => enable Eqidistant sampling mode(EST)
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE  BMA2XX_get_sleeptmr_mode(unsigned char *sleep_tmr)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
       //SLEEP TIMER MODE
       comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_SLEEP_TIMER__REG,C_BMA2XX_One_U8X);
      *sleep_tmr = BMA2XX_GET_BITSLICE(data,BMA2XX_SLEEP_TIMER);
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set the sleep timer mode status
 * 
 *					   ¸ÃAPIÓÃÓÚÉèÖÃË¯Ãß¶¨Ê±Æ÷Ä£Ê½×´Ì¬ 0x11
 *  \param unsigned char sleep_tmr
 *                  sleep_tmr -> [0:1]
 *                  0 => enable EventDrivenSampling(EDT)
 *                  1 => enable Eqidistant sampling mode(EST)
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_sleeptmr_mode (unsigned char sleep_tmr)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
        if (sleep_tmr < C_BMA2XX_Two_U8X)
        {
            //SLEEP TIMER MODE
            comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_SLEEP_TIMER__REG,C_BMA2XX_One_U8X);
            data = BMA2XX_SET_BITSLICE(data, BMA2XX_SLEEP_TIMER, sleep_tmr);
            comres = BMA2XX_WriteBytes( BMA2XX_SLEEP_TIMER__REG, data);
        }
        else
        {
            comres = E_OUT_OF_RANGE ;
        }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get shadow dis
 *
 *				      Õâ¸öAPI±»ÓÃÀ´»ñµÃÓ°×Ódis£¨0x13£©
 *  \param unsigned char *shadow_dis : Address of shadow_dis
 *                    1 -> No MSB Locking
 *                    0 -> MSB is Locked
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_shadow_dis (unsigned char *shadow_dis)
{
    int comres = C_BMA2XX_Zero_U8X;
    unsigned char data;
    {
        comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_DIS_SHADOW_PROC__REG,C_BMA2XX_One_U8X);
        *shadow_dis = BMA2XX_GET_BITSLICE(data,BMA2XX_DIS_SHADOW_PROC);
    }
    return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set shadow dis
 *
 *					    Õâ¸öAPIÓÃÓÚÉèÖÃÓ°×Ódis £¨0x13£©
 *  \param unsigned char shadow_dis
 *                   1 -> No MSB Locking
 *                   0 -> MSB is Locked
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_shadow_dis (unsigned char shadow_dis)
{
   int comres = C_BMA2XX_Zero_U8X;
   unsigned char data;
   {
      comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_DIS_SHADOW_PROC__REG, C_BMA2XX_One_U8X);
      data =BMA2XX_SET_BITSLICE(data,BMA2XX_DIS_SHADOW_PROC, shadow_dis);
	  ms_dbg_print(MOD_AUX, "Motion Sensor shadow_dis is %d\r\n",data);
      comres = BMA2XX_WriteBytes(BMA2XX_DIS_SHADOW_PROC__REG, data);
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get high bandwidth
 *
 *					   è¿™ä¸ªAPIè¢«ç”¨æ¥èŽ·å¾—é«˜å¸¦å®½ 0x13
 *  \param unsigned char *high_bw : Address of high_bw
 *                   1 -> Unfiltered High Bandwidth
 *                   0 -> Filtered Low Bandwidth
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_high_bw (unsigned char *high_bw)
{
    char comres;
    unsigned char data;
    {
       comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_DATA_HIGH_BW__REG,C_BMA2XX_One_U8X);
       *high_bw = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_DATA_HIGH_BW);
    }
    return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set high bandwidth
 *
 *						è¿™ä¸ªAPIç”¨äºŽè®¾ç½®é«˜å¸¦å®½ 0x13
 *  \param unsigned char high_bw
 *                    1 -> Unfiltered High Bandwidth
 *                    0 -> Filtered Low Bandwidth
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_high_bw (unsigned char high_bw)
{
    int comres = C_BMA2XX_Zero_U8X;
    unsigned char data;
    {
       comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_DATA_HIGH_BW__REG,C_BMA2XX_One_U8X);
       data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_DATA_HIGH_BW, high_bw);
	   ms_dbg_print(MOD_AUX, "Motion Sensor high_bw is %d\r\n",data);
       comres = BMA2XX_WriteBytes( BMA2XX_EN_DATA_HIGH_BW__REG, data);
    }
    return comres;
}

/****************************************************************************************
 * Description: *//**\brief This API is used to set interrupt enable bits of the sensor
 *
 *						    è¯¥APIç”¨äºŽè®¾ç½®ä¼ æ„Ÿå™¨çš„ä¸­æ–­ä½¿èƒ½ä½
 *  \param unsigned char InterruptType , unsigned char value
 *                        0 -> Low_G_Interrupt
 *                        1 -> High_G_X_Interrupt
 *                        2 -> High_G_Y_Interrupt
 *                        3 -> High_G_Z_Interrupt
 *                        4 -> DATA_EN
 *                        5 -> Slope_X_Interrupt
 *                        6 -> Slope_Y_Interrupt
 *                        7 -> Slope_Z_Interrupt
 *                        8 -> Single_Tap_Interrupt
 *                        9 -> Double_Tap_Interrupt
 *                       10 -> Orient_Interrupt
 *                       11 -> Flat_Interrupt
 ****************************************************************************************/
int BMA2XX_set_Int_Enable(unsigned char InterruptType , unsigned char value )
{
   int comres=C_BMA2XX_Zero_U8X;
   unsigned char data1,data2;
   {
      comres = BMA2XX_ReadBytes_Mul( &data1,BMA2XX_INT_ENABLE1_REG,C_BMA2XX_One_U8X );
      comres = BMA2XX_ReadBytes_Mul( &data2,BMA2XX_INT_ENABLE2_REG,C_BMA2XX_One_U8X );

      value = value & C_BMA2XX_One_U8X;
      switch (InterruptType)
      {
         case BMA2XX_Low_G_Interrupt:
            /* Low G Interrupt  */
            data2 = BMA2XX_SET_BITSLICE(data2, BMA2XX_EN_LOWG_INT, value );
            break;
         case BMA2XX_High_G_X_Interrupt:
            /* High G X Interrupt */
            data2 = BMA2XX_SET_BITSLICE(data2, BMA2XX_EN_HIGHG_X_INT, value );
            break;
         case BMA2XX_High_G_Y_Interrupt:
            /* High G Y Interrupt */
            data2 = BMA2XX_SET_BITSLICE(data2, BMA2XX_EN_HIGHG_Y_INT, value );
            break;
         case BMA2XX_High_G_Z_Interrupt:
            /* High G Z Interrupt */
            data2 = BMA2XX_SET_BITSLICE(data2, BMA2XX_EN_HIGHG_Z_INT, value );
            break;
         case BMA2XX_DATA_EN:
            /*Data En Interrupt  */
            data2 = BMA2XX_SET_BITSLICE(data2, BMA2XX_EN_NEW_DATA_INT, value );
            break;
         case BMA2XX_Slope_X_Interrupt:
            /* Slope X Interrupt */
            data1 = BMA2XX_SET_BITSLICE(data1, BMA2XX_EN_SLOPE_X_INT, value );
            break;
         case BMA2XX_Slope_Y_Interrupt:
            /* Slope Y Interrupt */
            data1 = BMA2XX_SET_BITSLICE(data1, BMA2XX_EN_SLOPE_Y_INT, value );
            break;
         case BMA2XX_Slope_Z_Interrupt:
            /* Slope Z Interrupt */
            data1 = BMA2XX_SET_BITSLICE(data1, BMA2XX_EN_SLOPE_Z_INT, value );
            break;
         case BMA2XX_Single_Tap_Interrupt:
            /* Single Tap Interrupt */
            data1 = BMA2XX_SET_BITSLICE(data1, BMA2XX_EN_SINGLE_TAP_INT, value );
            break;
         case BMA2XX_Double_Tap_Interrupt:
            /* Double Tap Interrupt */
            data1 = BMA2XX_SET_BITSLICE(data1, BMA2XX_EN_DOUBLE_TAP_INT, value );
            break;
         case BMA2XX_Orient_Interrupt:
            /* Orient Interrupt  */
            data1 = BMA2XX_SET_BITSLICE(data1, BMA2XX_EN_ORIENT_INT, value );
            break;
         case BMA2XX_Flat_Interrupt:
            /* Flat Interrupt */
            data1 = BMA2XX_SET_BITSLICE(data1, BMA2XX_EN_FLAT_INT, value );
            break;
         default:
            comres = E_BMA2XX_OUT_OF_RANGE;
            break;
      }
      comres = BMA2XX_WriteBytes(BMA2XX_INT_ENABLE1_REG,data1);
      comres = BMA2XX_WriteBytes(BMA2XX_INT_ENABLE2_REG,data2);
   }
   return comres;
}

/*********************************************************************************************
 * Description: *//**\brief This API is used to get the status of slow/no motion interrupt
 *
 *					       ¸ÃAPIÓÃÓÚ»ñÈ¡Âý/ÎÞ¶¯×÷ÖÐ¶ÏµÄ×´Ì¬(0x18)
 *  \param unsigned char channel,unsigned char *slo_data
 *           channel -->
 *           BMA2XX_SLO_NO_MOT_EN_X     ->     0
 *           BMA2XX_SLO_NO_MOT_EN_Y     ->     1
 *           BMA2XX_SLO_NO_MOT_EN_Z     ->     2
 *           BMA2XX_SLO_NO_MOT_EN_SEL   ->     3
 *           slo_data --> 1
 **********************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_slo_no_mot (unsigned char channel,unsigned char *slo_data)
{
     unsigned char data;
     int comres=C_BMA2XX_Zero_U8X;
     switch (channel)
     {
            case BMA2XX_SLO_NO_MOT_EN_X:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT_SLO_NO_MOT_EN_X_INT__REG, C_BMA2XX_One_U8X);
               *slo_data = BMA2XX_GET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_X_INT);
               break;
            case BMA2XX_SLO_NO_MOT_EN_Y:
               comres = BMA2XX_ReadBytes_Mul( &data, BMA2XX_INT_SLO_NO_MOT_EN_Y_INT__REG, C_BMA2XX_One_U8X);
               *slo_data = BMA2XX_GET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_Y_INT);
               break;
            case BMA2XX_SLO_NO_MOT_EN_Z:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT_SLO_NO_MOT_EN_Z_INT__REG, C_BMA2XX_One_U8X);
               *slo_data = BMA2XX_GET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_Z_INT);
               break;
            case BMA2XX_SLO_NO_MOT_EN_SEL:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT__REG, C_BMA2XX_One_U8X);
               *slo_data = BMA2XX_GET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
     }
     return comres;
}

/*********************************************************************************************
 * Description: *//**\brief This API is used to get the status of slow/no motion interrupt
 *
 *					        ¸ÃAPIÓÃÓÚ»ñÈ¡Âý/ÎÞ¶¯×÷ÖÐ¶ÏµÄ×´Ì¬(0x18)
 *  \param unsigned char channel,unsigned char *slo_data
 *           channel -->
 *           BMA2XX_SLO_NO_MOT_EN_X     ->     0
 *           BMA2XX_SLO_NO_MOT_EN_Y     ->     1
 *           BMA2XX_SLO_NO_MOT_EN_Z     ->     2
 *           BMA2XX_SLO_NO_MOT_EN_SEL   ->     3
 *           slo_data --> 1
 ********************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE  BMA2XX_set_slo_no_mot (unsigned char channel,unsigned char slo_data)
{
    {
      unsigned char data;
      int comres=C_BMA2XX_Zero_U8X;
      {
        switch (channel)
        {
            case BMA2XX_SLO_NO_MOT_EN_X:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_INT_SLO_NO_MOT_EN_X_INT__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_X_INT, slo_data);
               comres = BMA2XX_WriteBytes( BMA2XX_INT_SLO_NO_MOT_EN_X_INT__REG, data);
               break;
            case BMA2XX_SLO_NO_MOT_EN_Y:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT_SLO_NO_MOT_EN_Y_INT__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_Y_INT, slo_data);
               comres = BMA2XX_WriteBytes( BMA2XX_INT_SLO_NO_MOT_EN_Y_INT__REG, data);
               break;
            case BMA2XX_SLO_NO_MOT_EN_Z:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT_SLO_NO_MOT_EN_Z_INT__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_Z_INT, slo_data);
               comres = BMA2XX_WriteBytes( BMA2XX_INT_SLO_NO_MOT_EN_Z_INT__REG, data);
               break;
            case BMA2XX_SLO_NO_MOT_EN_SEL:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT__REG,C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT, slo_data);
               comres = BMA2XX_WriteBytes( BMA2XX_INT_SLO_NO_MOT_EN_SEL_INT__REG, data);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
        } 
      }
    return comres;
    }
}

/*********************************************************************************************
 * Description: *//**\brief This API is used to get the status of slow/no motion interrupt
 *
 *					¸ÃAPIÓÃÓÚ»ñÈ¡Âý/ÎÞ¶¯×÷ÖÐ¶ÏµÄ×´Ì¬(0x19 and 0x1B)
 *  \param unsigned char channel,unsigned char *int_slo_no_mot
 *                                 channel -->
 *                                 BMA2XX_INT1_SLO_NO_MOT     ->    0
 *                                 BMA2XX_INT2_SLO_NO_MOT     ->    1
 *                                 int_slo_no_mot --> 1
 *********************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_int_slo_no_mot (unsigned char channel,unsigned char *int_slo_no_mot)
{
    unsigned char data;
    int comres=C_BMA2XX_Zero_U8X;
    {
        switch (channel)
        {
            case BMA2XX_INT1_SLO_NO_MOT:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_INT1_PAD_SLO_NO_MOT__REG,C_BMA2XX_One_U8X);
               *int_slo_no_mot = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_INT1_PAD_SLO_NO_MOT);
               break;
            case BMA2XX_INT2_SLO_NO_MOT:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_INT2_PAD_SLO_NO_MOT__REG,C_BMA2XX_One_U8X);
               *int_slo_no_mot = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_INT2_PAD_SLO_NO_MOT);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
        }
    }
    return comres;
}

/*********************************************************************************************
 * Description: *//**\brief This API is used to set the status of slow/no motion interrupt
 *
 *				  ¸ÃAPIÓÃÓÚÉèÖÃÂý/ÎÞ¶¯×÷ÖÐ¶ÏµÄ×´Ì¬(0x19 and 0x1B)
 *  \param unsigned char channel,unsigned char int_slo_no_mot
 *                            channel -->
 *                            BMA2XX_INT1_SLO_NO_MOT     ->    0
 *                            BMA2XX_INT2_SLO_NO_MOT     ->    1
 *                            int_slo_no_mot --> 1
 *  \return communication results
 **********************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_int_slo_no_mot (unsigned char channel,unsigned char int_slo_no_mot)
{
    {
      unsigned char data;
      int comres=C_BMA2XX_Zero_U8X;
      {
        switch (channel)
        {
            case BMA2XX_INT1_SLO_NO_MOT:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_EN_INT1_PAD_SLO_NO_MOT__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_INT1_PAD_SLO_NO_MOT, int_slo_no_mot);
               comres = BMA2XX_WriteBytes( BMA2XX_EN_INT1_PAD_SLO_NO_MOT__REG, data);
               break;
            case BMA2XX_INT2_SLO_NO_MOT:
               comres = BMA2XX_ReadBytes_Mul(&data, BMA2XX_EN_INT2_PAD_SLO_NO_MOT__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_INT2_PAD_SLO_NO_MOT, int_slo_no_mot);
               comres = BMA2XX_WriteBytes( BMA2XX_EN_INT2_PAD_SLO_NO_MOT__REG, data);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
        }
      }
   return comres;
   }
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get the status of new data
 *  
 *							è¿™ä¸ªAPIç”¨äºŽèŽ·å–æ–°æ•°æ®çš„çŠ¶æ€			 
 *	
 *
 *  \param unsigned char channel,unsigned char *int_newdata
 *                         channel -->
 *                         BMA2XX_INT1_NDATA     ->    0
 *                         BMA2XX_INT2_NDATA     ->    1
 *                         int_newdata --> 1
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_newdata (unsigned char channel,unsigned char *int_newdata)
{
    unsigned char data;
    int comres=C_BMA2XX_Zero_U8X;

    switch (channel)
    {
        case BMA2XX_INT1_NDATA:
            comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_INT1_PAD_NEWDATA__REG,C_BMA2XX_One_U8X);
            *int_newdata = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_INT1_PAD_NEWDATA);
            break;
        case BMA2XX_INT2_NDATA:
            comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_INT2_PAD_NEWDATA__REG,C_BMA2XX_One_U8X);
            *int_newdata = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_INT2_PAD_NEWDATA);
            break;
        default:
            comres = E_BMA2XX_OUT_OF_RANGE;
            break;
    }
    return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set the status of new data
 *
 *							è¿™ä¸ªAPIç”¨äºŽè®¾ç½®æ–°æ•°æ®çš„çŠ¶æ€
 *  \param unsigned char channel,unsigned char int_newdata
 *                        channel -->
 *                        BMA2XX_INT1_NDATA     ->    0
 *                        BMA2XX_INT2_NDATA     ->    1
 *                        int_newdata --> 1
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_newdata (unsigned char channel,unsigned char int_newdata)
{
    unsigned char data;
    int comres=C_BMA2XX_Zero_U8X;
    {
        switch (channel)
        {
            case BMA2XX_INT1_NDATA:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_INT1_PAD_NEWDATA__REG,C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_INT1_PAD_NEWDATA, int_newdata);
               comres = BMA2XX_WriteBytes(BMA2XX_EN_INT1_PAD_NEWDATA__REG,data);
               break;
            case BMA2XX_INT2_NDATA:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_INT2_PAD_NEWDATA__REG,C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_INT2_PAD_NEWDATA, int_newdata);
               comres = BMA2XX_WriteBytes(BMA2XX_EN_INT2_PAD_NEWDATA__REG,data);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
        }
    }
    return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get the source status data
 *
 *								´ËAPIÓÃÓÚ»ñÈ¡Ô´×´Ì¬Êý¾Ý    
 *  \param unsigned char channel,unsigned char *int_source
 *                     Channel ->
 *                  BMA2XX_SRC_LOWG         0
 *                  BMA2XX_SRC_HIGHG        1
 *                  BMA2XX_SRC_SLOPE        2
 *                  BMA2XX_SRC_SLO_NO_MOT   3
 *                  BMA2XX_SRC_TAP          4
 *                  BMA2XX_SRC_DATA         5
 *                  int_source -> 1
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_source (unsigned char channel,unsigned char *int_source)
{
    {
      unsigned char data;
      int comres=C_BMA2XX_Zero_U8X;

        {
         switch (channel)
         {
            case BMA2XX_SRC_LOWG:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_LOWG__REG,C_BMA2XX_One_U8X);
               *int_source = BMA2XX_GET_BITSLICE(data,BMA2XX_UNFILT_INT_SRC_LOWG);
               break;
            case BMA2XX_SRC_HIGHG:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_HIGHG__REG,C_BMA2XX_One_U8X);
               *int_source = BMA2XX_GET_BITSLICE(data,BMA2XX_UNFILT_INT_SRC_HIGHG);
               break;
            case BMA2XX_SRC_SLOPE:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_SLOPE__REG,C_BMA2XX_One_U8X);
               *int_source = BMA2XX_GET_BITSLICE(data,BMA2XX_UNFILT_INT_SRC_SLOPE);
               break;
            case BMA2XX_SRC_SLO_NO_MOT:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT__REG,C_BMA2XX_One_U8X);
               *int_source = BMA2XX_GET_BITSLICE(data,BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT);
               break;
            case BMA2XX_SRC_TAP:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_TAP__REG,C_BMA2XX_One_U8X);
               *int_source = BMA2XX_GET_BITSLICE(data,BMA2XX_UNFILT_INT_SRC_TAP);
               break;
            case BMA2XX_SRC_DATA:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_DATA__REG,C_BMA2XX_One_U8X);
               *int_source = BMA2XX_GET_BITSLICE(data,BMA2XX_UNFILT_INT_SRC_DATA);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
         }
        }
    return comres;
    }
}

/*******************************************************************************
 * Description: *//**\brief  This API is used to set source status data
 *
 *							¸ÃAPIÓÃÓÚÉèÖÃÔ´×´Ì¬Êý¾Ý(0x1E)
 *  \param unsigned char channel,unsigned char int_source
 *                  Channel ->
 *                  BMA2XX_SRC_LOWG         0
 *                  BMA2XX_SRC_HIGHG        1
 *                  BMA2XX_SRC_SLOPE        2
 *                  BMA2XX_SRC_SLO_NO_MOT   3
 *                  BMA2XX_SRC_TAP          4
 *                  BMA2XX_SRC_DATA         5
 *                  int_source -> 1
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE  BMA2XX_set_source (unsigned char channel,unsigned char int_source)
{
    unsigned char data;
    int comres=C_BMA2XX_Zero_U8X;
    {
        switch (channel)
        {
            case BMA2XX_SRC_LOWG:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_LOWG__REG,C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_UNFILT_INT_SRC_LOWG, int_source);
               comres = BMA2XX_WriteBytes(BMA2XX_UNFILT_INT_SRC_LOWG__REG, data);
               break;
            case BMA2XX_SRC_HIGHG:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_HIGHG__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_UNFILT_INT_SRC_HIGHG, int_source);
               comres = BMA2XX_WriteBytes(BMA2XX_UNFILT_INT_SRC_HIGHG__REG, data);
               break;
            case BMA2XX_SRC_SLOPE:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_SLOPE__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_UNFILT_INT_SRC_SLOPE, int_source);
               comres = BMA2XX_WriteBytes(BMA2XX_UNFILT_INT_SRC_SLOPE__REG, data);
               break;
            case BMA2XX_SRC_SLO_NO_MOT:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT, int_source);
               comres = BMA2XX_WriteBytes(BMA2XX_UNFILT_INT_SRC_SLO_NO_MOT__REG, data);
               break;
            case BMA2XX_SRC_TAP:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_TAP__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_UNFILT_INT_SRC_TAP, int_source);
               comres = BMA2XX_WriteBytes( BMA2XX_UNFILT_INT_SRC_TAP__REG, data);
               break;
            case BMA2XX_SRC_DATA:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_UNFILT_INT_SRC_DATA__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_UNFILT_INT_SRC_DATA, int_source);
               comres = BMA2XX_WriteBytes(BMA2XX_UNFILT_INT_SRC_DATA__REG, data);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
        }
    }
    return comres;
}


/*******************************************************************************
 * Description: *//**\brief This API is used to get Active Level status
 *
 *							´ËAPIÓÃÓÚ»ñÈ¡Active Level×´Ì¬
 *  \param unsigned char channel,unsigned char *int_lvl
 *                  BMA2XX_INT1_LEVEL    ->    0
 *                  BMA2XX_INT2_LEVEL    ->    1
 *                  int_lvl : Active HI   ->   1
 *                            Active LO   ->   0
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_int_lvl (unsigned char channel,unsigned char *int_lvl)
{
    {
      unsigned char data;
      int comres=C_BMA2XX_Zero_U8X;
      {
        switch (channel)
        {
            case BMA2XX_INT1_LEVEL:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_INT1_PAD_ACTIVE_LEVEL__REG,C_BMA2XX_One_U8X);
               *int_lvl = BMA2XX_GET_BITSLICE(data,BMA2XX_INT1_PAD_ACTIVE_LEVEL);
               break;
            case BMA2XX_INT2_LEVEL:
               comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_INT2_PAD_ACTIVE_LEVEL__REG,C_BMA2XX_One_U8X);
               *int_lvl = BMA2XX_GET_BITSLICE(data,BMA2XX_INT2_PAD_ACTIVE_LEVEL);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
        }
      }
    return comres;
    }
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set Active Level status
 *
 *						´ËAPIÓÃÓÚÉèÖÃ»î¶¯¼¶±ð×´Ì¬(0x20)
 *  \param (unsigned char channel,unsigned char int_lvl)
 *                  BMA2XX_INT1_LEVEL    ->    0
 *                  BMA2XX_INT2_LEVEL    ->    1
 *                  int_lvl : Active HI   ->   1
 *                            Active LO   ->   0
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE  BMA2XX_set_int_lvl (unsigned char channel,unsigned char int_lvl)
{
    unsigned char data;
    int comres=C_BMA2XX_Zero_U8X;
    {
        switch (channel)
        {
            case BMA2XX_INT1_LEVEL:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT1_PAD_ACTIVE_LEVEL__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_INT1_PAD_ACTIVE_LEVEL, int_lvl);
               comres = BMA2XX_WriteBytes( BMA2XX_INT1_PAD_ACTIVE_LEVEL__REG, data);
               break;
            case BMA2XX_INT2_LEVEL:
               comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_INT2_PAD_ACTIVE_LEVEL__REG, C_BMA2XX_One_U8X);
               data = BMA2XX_SET_BITSLICE(data, BMA2XX_INT2_PAD_ACTIVE_LEVEL, int_lvl);
               comres = BMA2XX_WriteBytes( BMA2XX_INT2_PAD_ACTIVE_LEVEL__REG, data);
               break;
            default:
               comres = E_BMA2XX_OUT_OF_RANGE;
               break;
        }
    }
    return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get the latch duration
 *
 *							¸ÃAPIÓÃÓÚ»ñÈ¡Ëø´æÊ±¼ä(0x21)
 *  \param unsigned char *latch_int : Address of latch_int
 *                  0 -> NON_LATCH
 *                  1 -> 250MS
 *                  2 -> 500MS
 *                  3 -> 1S
 *                  4 -> 2S
 *                  5 -> 4S
 *                  6 -> 8S
 *                  7 -> LATCH
 *                  8 -> NON_LATCH1
 *                  9 -> 250US
 *                 10 -> 500US
 *                 11 -> 1MS
 *                 12 -> 12.5MS
 *                 13 -> 25MS
 *                 14 -> 50MS
 *                 15 -> LATCH1
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE  BMA2XX_get_latch_int (unsigned char *latch_int)
{
    unsigned char data;
    int comres=C_BMA2XX_Zero_U8X;
    {
        comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_LATCH_INT__REG,C_BMA2XX_One_U8X);
        *latch_int = BMA2XX_GET_BITSLICE(data,BMA2XX_LATCH_INT);
    }
    return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set the latch duration
 *
 *                     ¸ÃAPIÓÃÓÚÉèÖÃËø´æÊ±¼ä(0x21)
 *  \param unsigned char latch_int
 *                  0 -> NON_LATCH
 *                  1 -> 250MS
 *                  2 -> 500MS
 *                  3 -> 1S
 *                  4 -> 2S
 *                  5 -> 4S
 *                  6 -> 8S
 *                  7 -> LATCH
 *                  8 -> NON_LATCH1
 *                  9 -> 250US
 *                 10 -> 500US
 *                 11 -> 1MS
 *                 12 -> 12.5MS
 *                 13 -> 25MS
 *                 14 -> 50MS
 *                 15 -> LATCH1
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_latch_int (unsigned char latch_int)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X ;
   int latch_duration ;
   {
      if (latch_int < C_BMA2XX_Sixteen_U8X)
      {
        switch (latch_int)
        {
            case BMA2XX_LATCH_DUR_NON_LATCH:
               latch_duration = BMA2XX_LATCH_DUR_NON_LATCH;

               /*  NON LATCH   */
               break;
            case BMA2XX_LATCH_DUR_250MS:
               latch_duration = BMA2XX_LATCH_DUR_250MS;

               /*  250 MS  */
               break;
            case BMA2XX_LATCH_DUR_500MS:
               latch_duration = BMA2XX_LATCH_DUR_500MS;

               /*  500 MS  */
               break;
            case BMA2XX_LATCH_DUR_1S:
               latch_duration = BMA2XX_LATCH_DUR_1S;

               /*  1 S   */
               break;
            case BMA2XX_LATCH_DUR_2S:
               latch_duration = BMA2XX_LATCH_DUR_2S;

               /*  2 S  */
               break;
            case BMA2XX_LATCH_DUR_4S:
               latch_duration = BMA2XX_LATCH_DUR_4S;

               /*  4 S  */
               break;
            case BMA2XX_LATCH_DUR_8S:
               latch_duration = BMA2XX_LATCH_DUR_8S;

               /*  8 S  */
               break;
            case BMA2XX_LATCH_DUR_LATCH:
               latch_duration = BMA2XX_LATCH_DUR_LATCH;

               /*  LATCH  */
               break;
            case BMA2XX_LATCH_DUR_NON_LATCH1:
               latch_duration = BMA2XX_LATCH_DUR_NON_LATCH1;

               /*  NON LATCH1  */
               break;
            case BMA2XX_LATCH_DUR_250US:
               latch_duration = BMA2XX_LATCH_DUR_250US;

               /*  250 US   */
               break;
            case BMA2XX_LATCH_DUR_500US:
               latch_duration = BMA2XX_LATCH_DUR_500US;

               /*  500 US   */
               break;
            case BMA2XX_LATCH_DUR_1MS:
               latch_duration = BMA2XX_LATCH_DUR_1MS;

               /*  1 MS   */
               break;
            case BMA2XX_LATCH_DUR_12_5MS:
               latch_duration = BMA2XX_LATCH_DUR_12_5MS;

               /*  12.5 MS   */
               break;
            case BMA2XX_LATCH_DUR_25MS:
               latch_duration = BMA2XX_LATCH_DUR_25MS;

               /*  25 MS   */
               break;
            case BMA2XX_LATCH_DUR_50MS:
               latch_duration = BMA2XX_LATCH_DUR_50MS;

               /*  50 MS   */
               break;
            case BMA2XX_LATCH_DUR_LATCH1:
               latch_duration = BMA2XX_LATCH_DUR_LATCH1;

               /*  LATCH1   */
               break;
            default:
               break;
        }
        comres = BMA2XX_ReadBytes_Mul(&data, BMA2XX_LATCH_INT__REG,C_BMA2XX_One_U8X);
        data = BMA2XX_SET_BITSLICE(data, BMA2XX_LATCH_INT, latch_duration);
        comres = BMA2XX_WriteBytes( BMA2XX_LATCH_INT__REG, data);
      }
      else
      {
        comres = E_OUT_OF_RANGE ;
      }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get duration
 *
 *						Õâ¸öAPIÓÃÀ´»ñµÃ³ÖÐøÊ±¼ä
 *  \param unsigned char channel,unsigned char *dur
 *          BMA2XX_LOW_DURATION            0,1
 *          BMA2XX_HIGH_DURATION           1,2
 *          BMA2XX_SLOPE_DURATION          2,3
 *          BMA2XX_SLO_NO_MOT_DURATION     3,4
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_dur(unsigned char channel,unsigned char *dur)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      switch (channel)
      {
         case BMA2XX_LOW_DURATION:
            //LOW DURATION
            comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_LOW_DURN_REG,C_BMA2XX_One_U8X);
            *dur = data;
            break;
         case BMA2XX_HIGH_DURATION:
            //HIGH DURATION
            comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_HIGH_DURN_REG,C_BMA2XX_One_U8X);
            *dur = data;
            break;
         case BMA2XX_SLOPE_DURATION:
            //SLOPE DURATION
            comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_SLOPE_DUR__REG,C_BMA2XX_One_U8X);
            *dur = BMA2XX_GET_BITSLICE(data,BMA2XX_SLOPE_DUR);
            break;
         case BMA2XX_SLO_NO_MOT_DURATION:
            //SLO NO MOT DURATION
            comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_SLO_NO_MOT_DUR__REG,C_BMA2XX_One_U8X);
            *dur = BMA2XX_GET_BITSLICE(data,BMA2XX_SLO_NO_MOT_DUR);
            break;
         default:
            comres = E_BMA2XX_OUT_OF_RANGE;
            break;
      }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set duration
 *
 *						Õâ¸öAPIÓÃÓÚÉèÖÃ³ÖÐøÊ±¼ä
 *  \param unsigned char channel,unsigned char dur
 *          BMA2XX_LOW_DURATION            0,1
 *          BMA2XX_HIGH_DURATION           1,2
 *          BMA2XX_SLOPE_DURATION          2,3
 *          BMA2XX_SLO_NO_MOT_DURATION     3,4
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_dur (unsigned char channel,unsigned char dur)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      switch (channel)
      {
         case BMA2XX_LOW_DURATION:
            //LOW DURATION
            data = dur;
            comres = BMA2XX_WriteBytes( BMA2XX_LOW_DURN_REG, data);
            break;
         case BMA2XX_HIGH_DURATION:
            //HIGH DURATION
            data = dur;
            comres = BMA2XX_WriteBytes( BMA2XX_HIGH_DURN_REG, data);
            break;
         case BMA2XX_SLOPE_DURATION:
            //SLOPE DURATION
            comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_SLOPE_DUR__REG, C_BMA2XX_One_U8X);
            data = BMA2XX_SET_BITSLICE(data, BMA2XX_SLOPE_DUR, dur);
            comres = BMA2XX_WriteBytes( BMA2XX_SLOPE_DUR__REG, data);
            break;
         case BMA2XX_SLO_NO_MOT_DURATION:
            //SLO NO MOT DURATION 0x27
            comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_SLO_NO_MOT_DUR__REG,C_BMA2XX_One_U8X);
            data = BMA2XX_SET_BITSLICE(data, BMA2XX_SLO_NO_MOT_DUR, dur);
            comres = BMA2XX_WriteBytes( BMA2XX_SLO_NO_MOT_DUR__REG, data);
            break;
         default:
            comres = E_BMA2XX_OUT_OF_RANGE;
            break;
      }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get threshold
 *
 *						Õâ¸öAPIÓÃÓÚÉèÖÃ³ÖÐøÊ±¼ä
 *  \param unsigned char channel,unsigned char *thr
 *               BMA2XX_LOW_THRESHOLD            0,FE
 *               BMA2XX_HIGH_THRESHOLD           1,01
 *               BMA2XX_SLOPE_THRESHOLD          2,01
 *               BMA2XX_SLO_NO_MOT_THRESHOLD     3,00
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_thr(unsigned char channel,unsigned char *thr)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      switch (channel)
      {
         case BMA2XX_LOW_THRESHOLD:
            //LOW THRESHOLD
            comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_LOW_THRES_REG,C_BMA2XX_One_U8X);
            *thr = data;
            break;
         case BMA2XX_HIGH_THRESHOLD:
            //HIGH THRESHOLD
            comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_HIGH_THRES_REG,C_BMA2XX_One_U8X);
            *thr = data;
            break;
         case BMA2XX_SLOPE_THRESHOLD:
            //SLOPE THRESHOLD
            comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_SLOPE_THRES_REG,C_BMA2XX_One_U8X);
            *thr = data;
            break;
         case BMA2XX_SLO_NO_MOT_THRESHOLD:
            //SLO NO MOT THRESHOLD
            comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_SLO_NO_MOT_THRES_REG,C_BMA2XX_One_U8X);
            *thr = data;
            break;
         default:
            comres = E_BMA2XX_OUT_OF_RANGE;
            break;
      }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set threshold
 *
 *						Õâ¸öAPIÓÃÓÚÉèÖÃãÐÖµ
 *  \param unsigned char channel,unsigned char thr
 *               BMA2XX_LOW_THRESHOLD            0,FE
 *               BMA2XX_HIGH_THRESHOLD           1,01
 *               BMA2XX_SLOPE_THRESHOLD          2,01
 *               BMA2XX_SLO_NO_MOT_THRESHOLD     3,00
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_thr (unsigned char channel,unsigned char thr)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
     switch (channel)
     {
         case BMA2XX_LOW_THRESHOLD:
            //LOW THRESHOLD
            data = thr;
            comres = BMA2XX_WriteBytes( BMA2XX_LOW_THRES_REG,data);
            break;
         case BMA2XX_HIGH_THRESHOLD:
            //HIGH THRESHOLD
            data = thr;
            comres = BMA2XX_WriteBytes( BMA2XX_HIGH_THRES_REG,data);
            break;
         case BMA2XX_SLOPE_THRESHOLD:
            //SLOPE THRESHOLD
            data = thr;
            comres = BMA2XX_WriteBytes(BMA2XX_SLOPE_THRES_REG,data);
            break;
         case BMA2XX_SLO_NO_MOT_THRESHOLD:
            //SLO NO MOT THRESHOLD
            data = thr;
            comres = BMA2XX_WriteBytes(BMA2XX_SLO_NO_MOT_THRES_REG,data);
            break;
         default:
            comres = E_BMA2XX_OUT_OF_RANGE;
            break;
     }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is for to get Self Test Axis
 *                     
 *						Õâ¸öAPIÓÃÓÚ»ñÈ¡×Ô²âÖá
 *  \param unsigned char *self_test_axis : Address of self_test_axis
 *                          Possible values [1:0] are 0 to 3.
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_self_test_axis (unsigned char *self_test_axis)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_SELF_TEST__REG,C_BMA2XX_One_U8X);
      *self_test_axis = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_SELF_TEST);
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is for to Set Self Test Axis
 *
 *							¸ÃAPIÓÃÓÚÉèÖÃ×Ô¼ìÖá
 *  \param unsigned char self_test_axis
 *
 *                      Possible values [1:0] are 0 to 3.
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_self_test_axis (unsigned char self_test_axis)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      if (self_test_axis < C_BMA2XX_Four_U8X)
      {
        comres = BMA2XX_ReadBytes_Mul( &data,BMA2XX_EN_SELF_TEST__REG, C_BMA2XX_One_U8X);
        data = BMA2XX_SET_BITSLICE(data,BMA2XX_EN_SELF_TEST, self_test_axis);
        comres = BMA2XX_WriteBytes( BMA2XX_EN_SELF_TEST__REG, data);
      }
      else
      {
        comres = E_OUT_OF_RANGE ;
      }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is for to get Self Test sign
 *
 *				   Õâ¸öAPIÊÇÎªÁËµÃµ½×ÔÎÒ²âÊÔµÄ±êÖ¾(0x32)
 *  \param unsigned char *self_test_sign : Address of self_test_sign
 *                              0 => '+'ve sign
 *                              1 => '-'ve sign
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_self_test_sign (unsigned char *self_test_sign)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_NEG_SELF_TEST__REG,C_BMA2XX_One_U8X);
      *self_test_sign = BMA2XX_GET_BITSLICE(data,BMA2XX_NEG_SELF_TEST);
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is for to set Self Test sign
 *
 *				Õâ¸öAPIÊÇÎªÁËÉèÖÃ×ÔÎÒ²âÊÔµÄ±êÖ¾(0x32)
 *  \param  unsigned char self_test_sign
 *          0 => '+'ve sign
 *          1 => '-'ve sign
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_self_test_sign (unsigned char self_test_sign)
{
   unsigned char data;
   int comres = C_BMA2XX_Zero_U8X;
   {
      if (self_test_sign < C_BMA2XX_Two_U8X)
      {
        comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_NEG_SELF_TEST__REG,C_BMA2XX_One_U8X);
        data = BMA2XX_SET_BITSLICE(data,BMA2XX_NEG_SELF_TEST, self_test_sign);
        comres = BMA2XX_WriteBytes( data,BMA2XX_NEG_SELF_TEST__REG);
      }
      else
      {
        comres = E_OUT_OF_RANGE ;
      }
   }
   return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get the status slow compensation
 *
 *								è¿™ä¸ªAPIç”¨æ¥èŽ·å¾—çŠ¶æ€ç¼“æ…¢çš„è¡¥å¿
 *  \param unsigned char channel,unsigned char *slow_comp
 *                     BMA2XX_SLOW_COMP_X              0
 *                     BMA2XX_SLOW_COMP_Y              1
 *                     BMA2XX_SLOW_COMP_Z              2
 *
 *            slow_comp : 1 -> enable
 *                        0 -> disable slow offset
 ******************************************************************************/

BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_slow_comp(unsigned char channel,unsigned char *slow_comp)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	switch (channel)
	{
	case BMA2XX_SLOW_COMP_X:
		//SLOW COMP X
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_SLOW_COMP_X__REG,C_BMA2XX_One_U8X);
		*slow_comp = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_SLOW_COMP_X);
		break;
	case BMA2XX_SLOW_COMP_Y:
		//SLOW COMP Y
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_SLOW_COMP_Y__REG,C_BMA2XX_One_U8X);
		*slow_comp = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_SLOW_COMP_Y);
		break;
	case BMA2XX_SLOW_COMP_Z:
		//SLOW COMP Z
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_SLOW_COMP_Z__REG,C_BMA2XX_One_U8X);
		*slow_comp = BMA2XX_GET_BITSLICE(data,BMA2XX_EN_SLOW_COMP_Z);
		break;
	default:
		comres = E_BMA2XX_OUT_OF_RANGE;
		break;
	}
	return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set the status slow compensation
 *
 *							è¿™ä¸ªAPIç”¨æ¥è®¾ç½®çŠ¶æ€ç¼“æ…¢çš„è¡¥å¿
 *  \param unsigned char channel,unsigned char slow_comp
 *          BMA2XX_SLOW_COMP_X              0
 *          BMA2XX_SLOW_COMP_Y              1
 *          BMA2XX_SLOW_COMP_Z              2
 *
 *            slow_comp : 1 -> enable
 *                        0 -> disable slow offset
 ******************************************************************************/

BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_slow_comp (unsigned char channel,unsigned char slow_comp)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	switch (channel)
	{
	case BMA2XX_SLOW_COMP_X:
		//SLOW COMP X
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_SLOW_COMP_X__REG,C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_SLOW_COMP_X, slow_comp);
		comres = BMA2XX_WriteBytes( BMA2XX_EN_SLOW_COMP_X__REG, data);
		break;
	case BMA2XX_SLOW_COMP_Y:
		//SLOW COMP Y
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_SLOW_COMP_Y__REG,C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_SLOW_COMP_Y, slow_comp);
		comres = BMA2XX_WriteBytes( BMA2XX_EN_SLOW_COMP_Y__REG,data);
		break;
	case BMA2XX_SLOW_COMP_Z:
		//SLOW COMP Z
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_EN_SLOW_COMP_Z__REG, C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data, BMA2XX_EN_SLOW_COMP_Z, slow_comp);
		comres = BMA2XX_WriteBytes( BMA2XX_EN_SLOW_COMP_Z__REG, data);
		break;
	default:
		comres = E_BMA2XX_OUT_OF_RANGE;
		break;
	}
	return comres;
}


/***************************************************************************************************
 * Description: *//**\brief This API is used to get the status of fast offset compensation(cal rdy)
 *
 *
 *								è¯¥APIç”¨äºŽèŽ·å–å¿«é€Ÿåç§»è¡¥å¿ï¼ˆcal rdyï¼‰çš„çŠ¶æ€ï¼Œ
 *
 *  \param unsigned char *rdy
 *                 Read Only Possible
 ****************************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_cal_rdy (unsigned char *rdy)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	{
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_FAST_CAL_RDY_S__REG,C_BMA2XX_One_U8X);
		*rdy = BMA2XX_GET_BITSLICE(data,BMA2XX_FAST_CAL_RDY_S);
	}
	return comres;
}


/*******************************************************************************
 * Description: *//**\brief This API is used to set the status of cal rdy
 *
 *						è¿™ä¸ªAPIç”¨äºŽè®¾ç½®caldyçš„çŠ¶æ€
 *  \param unsigned char rdy
 *
 *  \return communication results
 *
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_cal_rdy (unsigned char rdy)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	{
		comres = BMA2XX_ReadBytes_Mul(&data, BMA2XX_FAST_CAL_RDY_S__REG,C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data,BMA2XX_FAST_CAL_RDY_S, rdy);
		comres = BMA2XX_WriteBytes( BMA2XX_FAST_CAL_RDY_S__REG, data);
	}
	return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get the status of cal trig
 *
 *						è¿™ä¸ªAPIç”¨äºŽèŽ·å–cal trigçš„çŠ¶æ€
 *  \param unsigned char *cal_trig
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_cal_trig (unsigned char *cal_trig)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	{
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_CAL_TRIGGER__REG,C_BMA2XX_One_U8X);
		*cal_trig = BMA2XX_GET_BITSLICE(data,BMA2XX_CAL_TRIGGER);
	}
	return comres;
}

/*****************************************************************************************************
 * Description: *//**\brief This API is used to set the status of fast offset calculation(cal trig)
 *
 *						è¿™ä¸ªAPIç”¨äºŽè®¾ç½®å¿«é€Ÿåç§»é‡è®¡ç®—çš„çŠ¶æ€ï¼ˆcal trigï¼‰
 *  \param unsigned char cal_trig
 *                  Write only possible
 *****************************************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_cal_trig (unsigned char cal_trig)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	{
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_CAL_TRIGGER__REG,C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data,BMA2XX_CAL_TRIGGER, cal_trig);
		comres = BMA2XX_WriteBytes( BMA2XX_CAL_TRIGGER__REG, data);
	}
	return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set the status of offset reset
 *
 *						è¯¥APIç”¨äºŽè®¾ç½®åç§»é‡ç½®çš„çŠ¶æ€
 *  \param unsigned char offset_reset
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_offset_reset (unsigned char offset_reset)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	comres = BMA2XX_ReadBytes_Mul(&data, BMA2XX_RESET_OFFSET_REGS__REG, C_BMA2XX_One_U8X);
	data = BMA2XX_SET_BITSLICE(data,BMA2XX_RESET_OFFSET_REGS, offset_reset);
	comres = BMA2XX_WriteBytes( BMA2XX_RESET_OFFSET_REGS__REG, data);
	return comres;
}


/*******************************************************************************
 * Description: *//**\brief This API is used to get the status of offset reset
 *
 *						è¯¥APIç”¨äºŽèŽ·å–åç§»é‡ç½®çš„çŠ¶æ€
 *  \param unsigned char channel,unsigned char *offset
 *                     Channel ->
 *               BMA2XX_CUT_OFF              ->    0
 *               BMA2XX_OFFSET_TRIGGER_X     ->    1
 *               BMA2XX_OFFSET_TRIGGER_Y     ->    2
 *               BMA2XX_OFFSET_TRIGGER_Z     ->    3
 *               offset ->
 *               CUT_OFF -> 0 or 1
 *               BMA2XX_OFFSET_TRIGGER_X   -> 0,1,2,3
 *               BMA2XX_OFFSET_TRIGGER_Y   -> 0,1,2,3
 *               BMA2XX_OFFSET_TRIGGER_Z   -> 0,1,2,3
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_offset_target(unsigned char channel,unsigned char *offset)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	switch (channel)
	{
	case BMA2XX_CUT_OFF:
		//CUT-OFF
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_COMP_CUTOFF__REG,C_BMA2XX_One_U8X);
		*offset = BMA2XX_GET_BITSLICE(data,BMA2XX_COMP_CUTOFF);
		break;
	case BMA2XX_OFFSET_TRIGGER_X:
		//OFFSET TRIGGER X
		comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_COMP_TARGET_OFFSET_X__REG,C_BMA2XX_One_U8X);
		*offset = BMA2XX_GET_BITSLICE(data,BMA2XX_COMP_TARGET_OFFSET_X);
		break;
	case BMA2XX_OFFSET_TRIGGER_Y:
		//OFFSET TRIGGER Y
		comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_COMP_TARGET_OFFSET_Y__REG,C_BMA2XX_One_U8X);
		*offset = BMA2XX_GET_BITSLICE(data,BMA2XX_COMP_TARGET_OFFSET_Y);
		break;
	case BMA2XX_OFFSET_TRIGGER_Z:
		//OFFSET TRIGGER Z
		comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_COMP_TARGET_OFFSET_Z__REG,C_BMA2XX_One_U8X);
		*offset = BMA2XX_GET_BITSLICE(data,BMA2XX_COMP_TARGET_OFFSET_Z);
		break;
	default:
		comres = E_BMA2XX_OUT_OF_RANGE;
		break;
	}
	return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set the status of offset reset
 *
 *									è¯¥APIç”¨äºŽè®¾ç½®åç§»é‡ç½®çš„çŠ¶æ€
 *  \param unsigned char channel,unsigned char offset
 *               Channel ->
 *               BMA2XX_CUT_OFF              ->    0
 *               BMA2XX_OFFSET_TRIGGER_X     ->    1
 *               BMA2XX_OFFSET_TRIGGER_Y     ->    2
 *               BMA2XX_OFFSET_TRIGGER_Z     ->    3
 *               offset ->
 *               CUT_OFF -> 0 or 1
 *               BMA2XX_OFFSET_TRIGGER_X   -> 0,1,2,3
 *               BMA2XX_OFFSET_TRIGGER_Y   -> 0,1,2,3
 *               BMA2XX_OFFSET_TRIGGER_Z   -> 0,1,2,3
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_offset_target (unsigned char channel,unsigned char offset)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	switch (channel)
	{
	case BMA2XX_CUT_OFF:
		//CUT-OFF
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_COMP_CUTOFF__REG,C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data, BMA2XX_COMP_CUTOFF, offset);
		comres = BMA2XX_WriteBytes( BMA2XX_COMP_CUTOFF__REG,data);
		break;
	case BMA2XX_OFFSET_TRIGGER_X:
		//OFFSET TARGET X
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_COMP_TARGET_OFFSET_X__REG,C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data, BMA2XX_COMP_TARGET_OFFSET_X, offset);
		comres = BMA2XX_WriteBytes( BMA2XX_COMP_TARGET_OFFSET_X__REG, data);
		break;
	case BMA2XX_OFFSET_TRIGGER_Y:
		//OFFSET TARGET Y
		comres = BMA2XX_ReadBytes_Mul(&data, BMA2XX_COMP_TARGET_OFFSET_Y__REG,C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data, BMA2XX_COMP_TARGET_OFFSET_Y, offset);
		comres = BMA2XX_WriteBytes( BMA2XX_COMP_TARGET_OFFSET_Y__REG, data);
		break;
	case BMA2XX_OFFSET_TRIGGER_Z:
		//OFFSET TARGET Z
		comres = BMA2XX_ReadBytes_Mul( &data, BMA2XX_COMP_TARGET_OFFSET_Z__REG, C_BMA2XX_One_U8X);
		data = BMA2XX_SET_BITSLICE(data, BMA2XX_COMP_TARGET_OFFSET_Z, offset);
		comres = BMA2XX_WriteBytes( BMA2XX_COMP_TARGET_OFFSET_Z__REG, data);
		break;
	default:
		comres = E_BMA2XX_OUT_OF_RANGE;
		break;
	}
	return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to get the status of offset
 *
 *								è¿™ä¸ªAPIç”¨äºŽèŽ·å–åç§»é‡çš„çŠ¶æ€
 *  \param unsigned char channel,unsigned char *offset
 *                         Channel ->
 *                   BMA2XX_X_AXIS     ->      0
 *                   BMA2XX_Y_AXIS     ->      1
 *                   BMA2XX_Z_AXIS     ->      2
 *                   offset -> Any valid value
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_get_offset(unsigned char channel,unsigned char *offset)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	switch (channel)
	{
	case BMA2XX_X_AXIS:
		comres = BMA2XX_ReadBytes_Mul(&data,BMA2XX_OFFSET_X_AXIS_REG,C_BMA2XX_One_U8X);
		ms_dbg_print(MOD_AUX, "Motion Sensor init offsetx = %d\n\r");
		*offset = data;
		break;
	case BMA2XX_Y_AXIS:
		comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_OFFSET_Y_AXIS_REG,C_BMA2XX_One_U8X);
		ms_dbg_print(MOD_AUX, "Motion Sensor init offsety = %d\n\r");
		*offset = data;
		break;
	case BMA2XX_Z_AXIS:
		comres =BMA2XX_ReadBytes_Mul(&data,BMA2XX_OFFSET_Z_AXIS_REG,C_BMA2XX_One_U8X);
		ms_dbg_print(MOD_AUX, "Motion Sensor init offsetz = %d\n\r");
		*offset = data;
		break;
	default:
		comres = E_BMA2XX_OUT_OF_RANGE;
		break;
	}
	return comres;
}

/*******************************************************************************
 * Description: *//**\brief This API is used to set the status of offset
 *
 *						è¿™ä¸ªAPIç”¨äºŽè®¾ç½®åç§»é‡çš„çŠ¶æ€
 *  \param unsigned char channel,unsigned char offset
 *                   Channel ->
 *                   BMA2XX_X_AXIS     ->      0
 *                   BMA2XX_Y_AXIS     ->      1
 *                   BMA2XX_Z_AXIS     ->      2
 *                   offset -> Any valid value
 ******************************************************************************/
BMA2XX_RETURN_FUNCTION_TYPE BMA2XX_set_offset (unsigned char channel,unsigned char offset)
{
	unsigned char data;
	int comres = C_BMA2XX_Zero_U8X;

	switch (channel)
	{
	case BMA2XX_X_AXIS:
		data = offset;
		comres = BMA2XX_WriteBytes( BMA2XX_OFFSET_X_AXIS_REG, data);
		break;
	case BMA2XX_Y_AXIS:
		data = offset;
		comres = BMA2XX_WriteBytes( BMA2XX_OFFSET_Y_AXIS_REG, data);
		break;
	case BMA2XX_Z_AXIS:
		data = offset;
		comres = BMA2XX_WriteBytes( BMA2XX_OFFSET_Z_AXIS_REG, data);
		break;
	default:
		comres = E_BMA2XX_OUT_OF_RANGE;
		break;
	}
	return comres;
}


/******************************************
	motion sensor softreset function
*******************************************/
kal_bool BMA2XX_SW_Reset(void)
{
	kal_bool temp_result = KAL_FALSE;

	temp_result = BMA2XX_WriteBytes(BMA2XX_RESET_REG,BMA2XX_EN_SOFT_RESET_VALUE);
	BMA2XX_DelayMS(5);		//ensure delay more than 2ms
	
	if(KAL_FALSE == temp_result)
	{
		ms_dbg_print_ps("Motion sensor soft reset fail!\n\r");
		return KAL_FALSE;
	}
	else
	{
		ms_dbg_print_ps("Motion sensor soft reset succeed!\n\r");
		return KAL_TRUE;
	}
}

/******************************************
	motion sensor power control function
[parameters]:
	TRUE 	power on
	FALSE	power off
*******************************************/
kal_bool BMA2XX_Power_Ctrl(kal_bool ON)
{

	if(ON != g_ms_is_power_on)
	{
		BMA2XX_set_mode(BMA2XX_MODE_DEEP_SUSPEND);
		BMA2XX_DelayMS(1);
	}
	else
    {
	    BMA2XX_set_mode(BMA2XX_MODE_NORMAL);
	    BMA2XX_DelayMS(1);
	}
	
	g_ms_is_power_on = ON;
		
	ms_dbg_print(MOD_AUX, "Motion Sensor power ctr:%d succeed!", ON);
	ms_dbg_print_ps("Motion Sensor power ctr:%d succeed!\n\r", ON);
	
	return KAL_TRUE;
}

/******************************************
	motion sensor power up function
*******************************************/
void BMA2XX_pwr_up(void)
{
	kal_bool pwr_result = KAL_FALSE;

	dbg_print("BMA2XX_pwr_up\n\r");

	if(KAL_FALSE == g_ms_is_power_on)
	{
		pwr_result = BMA2XX_Power_Ctrl(KAL_TRUE);
	
		if(KAL_FALSE == pwr_result)
		{
			ms_dbg_print(MOD_AUX, "Motion Sensor power up fail!");			
			ms_dbg_print_ps("Motion Sensor power up fail!\n\r");
		}
		else
		{
			ms_dbg_print_ps("Motion Sensor power up succeed!\n\r");
		}
	}
}

/******************************************
	motion sensor power down function
*******************************************/
void BMA2XX_pwr_down(void)
{
	kal_bool pwr_result = KAL_FALSE;
	
	dbg_print("BMA2XX_pwr_down\n\r");

	if(KAL_TRUE == g_ms_is_power_on)
	{
		pwr_result = BMA2XX_Power_Ctrl(KAL_FALSE);
	
		if(KAL_FALSE == pwr_result)
		{
			ms_dbg_print(MOD_AUX, "Motion Sensor power down fail!");
			ms_dbg_print_ps("Motion Sensor power down fail!\n\r");
		}
		else
		{
			ms_dbg_print_ps("Motion Sensor power down succeed!\n\r");
		}
	}
}

/******************************************
	motion sensor read ID function
*******************************************/
kal_uint8 BMA2XX_ReadID(void)
{
	kal_uint8 ID_data = 0;
	kal_bool temp_result = KAL_FALSE;
	
	temp_result = BMA2XX_ReadBytes(&ID_data, BMA2XX_CHIP_ID_REG);
	
	if(KAL_FALSE == temp_result)
	{
		ms_dbg_print(MOD_AUX, "Motion Sensor read id fail");
		ms_dbg_print_ps("Motion Sensor read id fail\n\r");
	}
	else
	{
		ms_dbg_print(MOD_AUX, "CHIP_ID:%d", ID_data);
		ms_dbg_print_ps("CHIP_ID:%x \n\r", ID_data);
	}

	return ID_data;
}

/******************************************
	motion sensor init function
*******************************************/
void BMA2XX_init(void)
{
	kal_bool pwr_result = KAL_FALSE;
	kal_bool reset_result = KAL_FALSE;
	kal_bool init_result = KAL_FALSE;
	kal_uint8 ID_data = 0;
	
	ms_dbg_print_ps("in BMA2XX_init\r\n");

	//Init sck&data pin	
	MS_CLK_PIN_GPIO_MODE;
	MS_I2C_CLK_OUTPUT;
	MS_I2C_CLK_HIGH;

	MS_DATA_PIN_GPIO_MODE;
	MS_I2C_DATA_OUTPUT;
	MS_I2C_DATA_HIGH;

	ID_data = BMA2XX_ReadID();
	if(ID_data != BMA223_CHIP_ID)
	{
		kal_prompt_trace(MOD_AUX,"Motion Sensor ID fail!\n\r");	
	}
	else
	{
		kal_prompt_trace(MOD_AUX,"Motion Sensor ID success!\n\r");
	}

	//do soft reset to ensure all register are default value
	reset_result = BMA2XX_SW_Reset();
	if(KAL_FALSE == reset_result)
	{
		kal_prompt_trace(MOD_AUX,"Motion Sensor soft reset fail!\n\r");
	}

	//motion sensor mode select
	pwr_result = BMA2XX_Power_Ctrl(KAL_TRUE);
	if(KAL_FALSE == pwr_result)
	{
		ms_dbg_print(MOD_AUX, "Motion Sensor init power on fail!");
		ms_dbg_print_ps("Motion Sensor init power on fail!\n\r");
	}

	//set range value
	init_result = BMA2XX_Set_Range(BMA2XX_RANGE_4G);
	if(KAL_FALSE == pwr_result||KAL_FALSE == init_result||KAL_FALSE == reset_result)
	{
		kal_prompt_trace(MOD_AUX,"Motion Sensor init fail!\n\r");
	}
	else
	{
		kal_prompt_trace(MOD_AUX,"Motion Sensor init succeed!\n\r");
	}

	BMA2XX_set_shadow_dis(MSB_NO_Locking);

	BMA2XX_set_high_bw(Unfiltered_HIGH_Bandwidth);

	BMA2XX_set_bandwidth(BMA2XX_BW_31_25HZ);

	BMA2XX_set_offset(BMA2XX_X_AXIS,C_BMA2XX_Zero_U8X);
	BMA2XX_set_offset(BMA2XX_Y_AXIS,C_BMA2XX_Zero_U8X);
	BMA2XX_set_offset(BMA2XX_Z_AXIS,C_BMA2XX_Zero_U8X);
}

/******************************************
	motion sensor read x,y,z acc function
*******************************************/
void BMA2XX_ReadXYZ(kal_uint16 *X, kal_uint16 *Y, kal_uint16 *Z)
{
	//Read 3 continous bytes of X, Y and Z 6-bit data
	kal_uint8 XYZdata[6] = {0};
	kal_bool temp_result = KAL_FALSE;

	temp_result = BMA2XX_ReadBytes_Mul(XYZdata, MS_ACC_REG,C_BMA2XX_Six_U8X);
	if(KAL_FALSE == temp_result)
	{
		kal_prompt_trace(MOD_AUX,"Motion Sensor get data fail!\n\r");
	}
	else
	{	
//		if(XYZdata[0] == 1 && XYZdata[2] == 1 && XYZdata[4] == 1)
//		{		
			*X = (kal_uint16)(XYZdata[1]);
			*Y = (kal_uint16)(XYZdata[3]);
			*Z = (kal_uint16)(XYZdata[5]);
//		}

		kal_prompt_trace(MOD_AUX,"After Acc_x:%d, Acc_y:%d, Acc_z:%d \n\r", *X, *Y, *Z);
	}
}

kal_uint8  Motion_detection_deal(void)
{
	kal_uint8 ACC_flag = 0;
	static kal_uint16 RX = 0,RY = 0,RZ = 0;
	kal_uint16 ACC_X,ACC_Y,ACC_Z;
	BMA2XX_ReadXYZ(&ACC_X,&ACC_Y,&ACC_Z);
	if(ACC_X < (RX -3) || ACC_X > (RX + 3) || ACC_Y < (RY - 3) || ACC_Y > (RY + 3) || ACC_Z < (RZ - 3) || ACC_Z > (RZ + 3))
	{
		ACC_flag =  1;
	}
	else
	{
		ACC_flag = 0;
	}

	RX = ACC_X;
	RY = ACC_Y;
	RZ = ACC_Z;
	
	return ACC_flag;
}

MotionSensor_custom_data_struct* BMA2XX_get_data(void) 
{
   return (&ms_custom_data_def);
}

/*******************************************
	customizaton function pointer
********************************************/
MotionSensor_customize_function_struct ms_custom_func=
{
	BMA2XX_get_data,
    BMA2XX_ReadXYZ,
    BMA2XX_init,
    BMA2XX_pwr_up,
	BMA2XX_pwr_down,
	NULL,//mma_sensor_read_int_status,
	NULL,//mma_sensor_clear_int_status,
	NULL,//mma_sensor_configure_low_g,
	NULL,//mma_sensor_configure_high_g   
	NULL,
    NULL,
    NULL
};
MotionSensor_customize_function_struct *ms_GetFunc(void)
{
   return (&ms_custom_func);  
}

#endif

