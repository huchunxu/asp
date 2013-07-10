/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     GetFarSignalValue.h
 *
 *        @brief    获取激光信号
 *
 *        @version  0.1
 *        @date     2012/5/26  19：00
 *
 *        @author:  Cao Shen , caoshen.1992@gmail.com
 * =====================================================================================
 *  @0.1    Cao Shen   2012/5/26    create orignal file
 * =====================================================================================
 */

#ifndef GETFARSIGNALVALUE_H_
#define GETFARSIGNALVALUE_H_

#include "laser.h"

extern uint32_t  gl_leftLaserValueTemp;
extern uint32_t  gl_rightLaserValueTemp;

#define UP_LASER_NUM         20
#define UPLEFT_LASER_NUM        10
#define UPRIGHT_LASER_NUM       10
#define LASER_SINGLE_NUM      10
#define HISTORY_SIGNAL_NUM    40
/**
 * 	@brief:	initFarSignalSend
 *
 * 	@note:K60引脚初始化过程中，设置引脚为输出时同时设置了该引脚的电平
 */
void initFarSignalSend(intptr_t exinf);

/**
 * 	@brief:	FarSignalControlReg.
 *
 * 	@note:	select one of the 4 parts lasers to take function
 *
 * 	@return:	none
 */
void FarSignalControlReg(uint8_t reg1, uint8_t reg0);

/**
 * 	@brief:	SignalAddressReg.
 *
 * 	@note:	select one of the 6 lasers in the specific part to take function
 *
 * 	@return:	none
 */
void SignalAddressReg(uint8_t reg3, uint8_t reg2, uint8_t reg1, uint8_t reg0);

/**
 *   	@brief:         closeFarSignal.
 *
 *    	@author： WangJian
 *
 *    	@note:         close all the lasers
 *
 *    @	return:      none
*/
void closeFarSignal(void);

/**
 *		@brief:Function:     leftLaserSend.
 *
 *    	@author： WangJian
 *
 *     @note:
 *
 *     @return:       none
 */
void leftLaserSend(uint8_t number);

/**
 *		@brief:Function:     rightLaserSend.
 *
 *    	@author： WangJian
 *
 *     @note:
 *
 *     @return:       none
 */
void rightLaserSend(uint8_t number);

/**
 *		Function:     FarSignalSend.
 *    	Programmers： WangJian
 *    	Description:  make the specific laser take function according to "number"
 *    	Return:       none
*/
void FarSignalSend(uint8_t number);

/**
 *    @brief:   InitialLaserReceive.
 *
 *    @author: WangZhi
 *
 *    @note:Initial Laser Receive function
 *    @return:       none
 */
void initFarSignalReceive(intptr_t exinf);

/**
 * 	@breif:getLeftLaserValue.
 *
 * 	@author:WangJian
 *
 * 	@return:none
 */
void getLeftLaserValue(uint8_t number);

/**
 * 	@breif:getRightLaserValue.
 *
 * 	@author:WangJian
 *
 * 	@return:none
 */
void getRightLaserValue(uint8_t number);

#endif /* GETFARSIGNALVALUE_H_ */
