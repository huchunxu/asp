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

#ifndef GETNEARSIGNALVALUE_H_
#define GETNEARSIGNALVALUE_H_

#include "laser.h"

#define DOWN_LASER_NUM      8
/**
 * 	@brief:initNearSignalSend.
 *
 * 	@note:Initial Laser Send function
 *
 * 	@return:none
 */
void initNearSignalSend(void);

/**
 * 	@brief:NearSignalControlReg
 *
 * 	@note:select one of the 4 parts lasers to take function
 *
 * 	@return:none
 */
void NearSignalControlReg(uint8_t reg0);

/**
 * 	@brief:NearFarSignal
 *
 * 	@note:close all the lasers
 *
 * 	@return:none
 */
void CloseNearSignal(void);

/**
 * 	@brief:NearSignalEncoder.
 *
 * 	@note:make the specific laser take function according to "number"
 *
 * 	@return:none
 */
void NearSignalSend(uint8_t number);

/**
 * 	@brief:initNearSignalReceive.
 *
 * 	@note:Initial Laser Receive function
 *
 * 	@return:none
 */
void initNearSignalReceive(void);

/**
 * 	@brief:ReadLaserNear.
 *
 * 	@note:read sensor value on the specific bit
 *
 * 	@return:none
 */
void GetNearSignalValue(uint8_t number);

#endif /* GETNEARSIGNALVALUE_H_ */
