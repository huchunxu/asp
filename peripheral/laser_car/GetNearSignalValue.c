/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     GetNearSignalValue.c
 *
 *        @brief    获取激光信号
 *
 *        @version  0.1
 *        @date     2012/5/24  19：00
 *
 *        @author:  Cao Shen , caoshen.1992@gmail.com
 * =====================================================================================
 *  @0.1    Cao Shen   2012/5/26    create orignal file
 * =====================================================================================
 */

#include "GetNearSignalValue.h"
/**
 * Variables Declearation
 */
uint8_t gl_NearSignalBit[DOWN_LASER_NUM] = {0};
uint32_t  gl_NearSignalValue = 0;

/**
 * 	@brief:initNearSignalSend.
 *
 * 	@note:Initial Laser Send function
 *
 * 	@return:none
 */
void initNearSignalSend(void)
{
	down_laser_control_init();
}

/**
 * 	@brief:NearSignalControlReg
 *
 * 	@note:select one of the 4 parts lasers to take function
 *
 * 	@return:none
 */
void NearSignalControlReg(uint8_t reg0)
{
	if(reg0==0)
		down_laser_control_open(DOWN_LASER_CONTROL0);
	else if(reg0==1)
		down_laser_control_close(DOWN_LASER_CONTROL0);
}

/**
 * 	@brief:NearFarSignal
 *
 * 	@note:close all the lasers
 *
 * 	@return:none
 */
void CloseNearSignal(void)
{
    NearSignalControlReg(1);
}

/**
 * 	@brief:NearSignalEncoder.
 *
 * 	@note:make the specific laser take function according to "number"
 *
 * 	@return:none
 */
void NearSignalSend(uint8_t number)
{
    switch(number)
    {
       /* case 0:
            NearSignalControlReg(0,1);
            SignalAddressReg(1,0,1);
            break;
        case 1:
            NearSignalControlReg(0,1);
            SignalAddressReg(1,0,0);
            break;
        case 2:
            NearSignalControlReg(0,1);
            SignalAddressReg(0,1,1);
            break;
        case 3:
            NearSignalControlReg(0,1);
            SignalAddressReg(0,1,0);
            break;
        case 4:
            NearSignalControlReg(0,1);
            SignalAddressReg(0,0,0);
            break;
        case 5:
            NearSignalControlReg(0,1);
            SignalAddressReg(0,0,1);
            break;
        case 6:
            NearSignalControlReg(1,0);
            SignalAddressReg(1,0,1);
            break;
        case 7:
            NearSignalControlReg(1,0);
            SignalAddressReg(1,1,0);
            break;
        case 8:
            NearSignalControlReg(1,0);
            SignalAddressReg(1,0,0);
            break;
        case 9:
            NearSignalControlReg(1,0);
            SignalAddressReg(0,1,1);
            break;
        case 10:
            NearSignalControlReg(1,0);
            SignalAddressReg(0,1,0);
            break;
        case 11:
            NearSignalControlReg(1,0);
            SignalAddressReg(0,0,1);
            break;*/
        default:
            CloseNearSignal();
            break;
    }
}

/**
 * 	@brief:initNearSignalReceive.
 *
 * 	@note:Initial Laser Receive function
 *
 * 	@return:none
 */
void initNearSignalReceive(void)
{
	down_laser_receive_init();
}

/**
 * 	@brief:ReadLaserNear.
 *
 * 	@note:read sensor value on the specific bit
 *
 * 	@return:none
 */
void GetNearSignalValue(uint8_t  number)
{
	/* (number > 11)&&(number < 24) */
    uint16_t temp = 0;
    uint16_t temp1 = 0;

//    uint8_t nearNumber = 0;

    uint16_t tempValue = 0;

    temp=down_laser_receive(DOWN_LASER_RECEIVE0);
    temp1 = down_laser_receive(DOWN_LASER_RECEIVE1);

    if(number < 6)                                    //one receiving tube can receive four lasers
    {
        tempValue = temp;
    }
    else if(number < 12)
    {
        tempValue = temp1;
    }

    gl_NearSignalValue |= (tempValue << number);

    gl_NearSignalBit[number] = (uint8_t )tempValue;   //  only for printp
}

