/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     getLaserInfo.h
 *
 *        @brief    获取上排激光信号头文件
 *
 *        @version  0.1
 *        @date     2012-5-26  19：00
 *
 *        @author:  Cao Shen , caoshen.1992@gmail.com
 * =====================================================================================
 *  @0.1    Cao Shen   2012-5-26    create orignal file
 * =====================================================================================
 */
#ifndef GETLASERINFO_H_
#define GETLASERINFO_H_

#include "t_stddef.h"

#define FAR_LEFT_LASER_NUM    10
#define FAR_RIGHT_LASER_NUM   10
#define FAR_SINGLE_ROW_NUM    10

#define LASER_DELAY_TIME  15


extern uint32_t  gl_NearSignalValue;

/**
 *    @brief   getLeftLaserInfo
 *
 *		@note:get left laser infomation.
 *
 */
void getLeftLaserInfo(void);

/**
 *    @brief   getRightLaserInfo
 *
 *		@note:get right laser infomation.
 *
 *		@return:       none
 */
void getRightLaserInfo(void);

/**
 *    @brief   getLaserInfo
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *		@note:get laser infomation.
 *
 *		@return:       none
 */
void getLaserInfo(void);

void delay_10us(uint32_t ntime);
void delay_us(uint32_t us_1);


#endif /* GETLASERINFO_H_ */
