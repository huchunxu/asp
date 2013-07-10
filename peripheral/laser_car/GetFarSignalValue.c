/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     GetFarSignalValue.c
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

#include "GetFarSignalValue.h"

/*  Variables Declearation  */
uint32_t gl_leftLaserBit[UPLEFT_LASER_NUM] = {0};
uint32_t gl_rightLaserBit[UPRIGHT_LASER_NUM] = {0};

/**
 * 	@brief:	initFarSignalSend
 *
 * 	@note:K60引脚初始化过程中，设置引脚为输出时同时设置了该引脚的电平
 */
void initFarSignalSend(intptr_t exinf)
{
	up_laser_control_init();            /*初始化上排激光片选，默认引脚电平为1：低点平使能*/

	laser_address_init();                 /*初始化4-16译码器，注意此时4根地址线均为1*/

	closeFarSignal();                        /*关闭所有译码器,初始化的防止干扰*/
}

/**
 * 	@brief:	FarSignalControlReg.
 *
 * 	@note:	select one of the 4 parts lasers to take function
 *
 * 	@return:	none
 */
void FarSignalControlReg(uint8_t reg1, uint8_t reg0)
{
	if(reg1==0)
		up_laser_control_open(UP_LASER_CONTROL_RIGHT);
	else if(reg1==1)
		up_laser_control_close(UP_LASER_CONTROL_RIGHT);

	if(reg0==0)
		up_laser_control_open(UP_LASER_CONTROL_LEFT);
	else if(reg0==1)
		up_laser_control_close(UP_LASER_CONTROL_LEFT);
}

/**
 * 	@brief:	SignalAddressReg.
 *
 * 	@note:	select one of the 6 lasers in the specific part to take function
 *
 * 	@return:	none
 */
void SignalAddressReg(uint8_t reg3, uint8_t reg2, uint8_t reg1, uint8_t reg0)
{
	if(reg0==0)
		laser_address_close(LASER_ADDRESS0);   /*设置该地址线为低电平*/
	else if(reg0==1)
		laser_address_open(LASER_ADDRESS0);  /*设置该地址线为低电平*/

	if(reg1==0)
		laser_address_close(LASER_ADDRESS1);
	else if(reg1==1)
		laser_address_open(LASER_ADDRESS1);

	if(reg2==0)
		laser_address_close(LASER_ADDRESS2);
	else if(reg2==1)
		laser_address_open(LASER_ADDRESS2);

	if(reg3==0)
		laser_address_close(LASER_ADDRESS3);
	else if(reg3==1)
		laser_address_open(LASER_ADDRESS3);
}

/**
 *   	@brief:         closeFarSignal.
 *
 *    	@author： WangJian
 *
 *    	@note:         close all the lasers
 *
 *    @	return:      none
*/
void closeFarSignal(void)
{
    FarSignalControlReg(1,1);
}

/**
 *		@brief:Function:     leftLaserSend.
 *
 *    	@author： WangJian
 *
 *     @note:
 *
 *     @return:       none
 */
void leftLaserSend(uint8_t number)
{
    FarSignalSend(number);
}

/**
 *		@brief:Function:     rightLaserSend.
 *
 *    	@author： WangJian
 *
 *     @note:
 *
 *     @return:       none
 */
void rightLaserSend(uint8_t number)
{
    FarSignalSend(number);
}

/**
 *		Function:     FarSignalSend.
 *    	Programmers： WangJian
 *    	Description:  make the specific laser take function according to "number"
 *    	Return:       none
*/
void FarSignalSend(uint8_t number)
{
    switch(number)
    {
        case 0:
        case 23:
            SignalAddressReg(0,0,0,0);
            break;
        case 1:
        case 22:
            SignalAddressReg(0,0,0,1);
            break;
        case 2:
        case 21:
            SignalAddressReg(0,0,1,1);
            break;
        case 3:
        case 20:
            SignalAddressReg(0,0,1,0);
            break;
        case 4:
        case 19:
            SignalAddressReg(0,1,1,0);
            break;
        case 5:
        case 18:
            SignalAddressReg(0,1,1,1);
            break;
        case 6:
        case 17:
            SignalAddressReg(0,1,0,1);
            break;
        case 7:
        case 16:
            SignalAddressReg(0,1,0,0);
            break;
        case 8:
        case 15:
            SignalAddressReg(1,0,0,0);
            break;
        case 9:
        case 14:
            SignalAddressReg(1,0,0,1);
            break;
        case 10:
        case 13:
            SignalAddressReg(1,0,1,1);
            break;
        case 11:
        case 12:
            SignalAddressReg(1,0,1,0);
            break;
        default:
            closeFarSignal();                //close all
            break;
    }

}

/**
 *    @brief:   InitialLaserReceive.
 *
 *    @author: WangZhi
 *
 *    @note:Initial Laser Receive function
 *    @return:       none
 */
void initFarSignalReceive(intptr_t exinf)
{
	up_laser_receive_init();    /*all receive port has to be set to INPUT state.*/
}

/**
 * 	@breif:getLeftLaserValue.
 *
 * 	@author:WangJian
 *
 * 	@return:none
 */
void getLeftLaserValue(uint8_t number)
{
		uint16_t valueTemp = 0;
		uint8_t numberTemp = 0;

	    uint16_t temp0 = 0;
	    uint16_t temp1 = 0;
	    uint16_t temp2 = 0;

	    numberTemp = number - 2;

	    temp0 = up_laser_receive(UP_LASER_RECEIVE0);
	    temp1 = up_laser_receive(UP_LASER_RECEIVE1);
	    temp2 = up_laser_receive(UP_LASER_RECEIVE2);

	    if(number < 4)                                    //one receiving tube can receive four lasers
	    {
	        valueTemp = temp0;
	    }
	    else if(number < 8)
	    {
	        valueTemp = temp1;
	    }
	    else
	    {
	        valueTemp = temp2;
	    }

	    gl_leftLaserValueTemp |= (valueTemp << (uint8_t)((LASER_SINGLE_NUM -1) - numberTemp)) ;
	    gl_leftLaserBit[numberTemp] = (uint8_t)valueTemp;
}

/**
 * 	@breif:getRightLaserValue.
 *
 * 	@author:WangJian
 *
 * 	@return:none
 */
void getRightLaserValue(uint8_t number)
{
		uint16_t valueTemp = 0;
		uint8_t numberTemp = 0;

	    uint16_t temp0 = 0;
	    uint16_t temp1 = 0;
	    uint16_t temp2 = 0;

	    temp0 =  up_laser_receive(UP_LASER_RECEIVE5);
	    temp1 =  up_laser_receive(UP_LASER_RECEIVE4);
	    temp2 =  up_laser_receive(UP_LASER_RECEIVE3);

	    numberTemp  = number + 2;

	    if(numberTemp < 4)                                    // one receiving tube can receive four lasers
	    {
	        valueTemp = temp0;
	    }
	    else if(numberTemp < 8)
	    {
	        valueTemp = temp1;
	    }
	    else
	    {
	        valueTemp = temp2;
	    }

	    gl_rightLaserValueTemp |= (valueTemp << (uint8_t)number) ;
	    gl_rightLaserBit[number] = (uint8_t)valueTemp;
}

