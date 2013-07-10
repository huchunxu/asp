/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     FarSignalProcess.h
 *
 *        @brief    上排激光信号处理
 *
 *        @version  0.1
 *        @date     2012/5/24  19：00
 *
 *        @author:  Cao Shen , caoshen.1992@gmail.com
 * =====================================================================================
 *  @0.1    Cao Shen   2012/5/26    create orignal file
 * =====================================================================================
 */
#ifndef FARSIGNALPROCESS_H_
#define FARSIGNALPROCESS_H_

#include "t_stddef.h"
#include "GetFarSignalValue.h"

#define     NORMAL          0
#define     LEFT_CURVE       1
#define     RIGHT_CURVE      2

#define     NOPROTECTION    0
#define     LEFTOUT         1
#define     RIGHTOUT        2
#define     OTHER           3

#define     OUT_LEFT_1      1
#define     OUT_LEFT_2      2
#define     OUT_LEFT_3      3
#define     OUT_LEFT_4      4
#define     OUT_RIGHT_1    -1
#define     OUT_RIGHT_2    -2
#define     OUT_RIGHT_3    -3
#define     OUT_RIGHT_4    -4

#define     CROSSLINE_BOUNDARY  10

extern volatile int16_t gl_FollowAngle;
extern uint8_t gl_followModel;
extern uint32_t gl_leftLaserBit[UPLEFT_LASER_NUM];
extern uint32_t gl_rightLaserBit[UPRIGHT_LASER_NUM];


/**
 * 	@brief:farSignalProcess
 *
 * 	@note:
 */
void farSignalProcess(void);

/**
 * 	@breif: farSignalFilter
 *
 * 	@note:because the neighbouring two lasers's gap is 2.5cm, and the blackline  is also 2.5cm, so signal maybe wrong.and this function is to solve this problem.
 *
 * 	@return:none
 */
void farSignalFilter(void);

/**
 * 	@breif: getHistorySignal
 *
 * 	@note:get the last 10 times signals.
 *
 * 	@return:none
 */
void getHistorySignal(void);



/**
 * 	@brief:	laser2_5GapFilter
 *
 * 	@note:	if filter alters same orignal signal for many times, then belive the orignal signal.
 *
 * 	@return:	none
 */
void laser2_5GapFilter(void);

/**
 * 	@brief:	alterManyTimesFilter
 *
 * 	@note:	if filter alters same orignal signal for many times, then belive the orignal siganl.
 *
 * 	@return:	none
 */
void alterManyTimesFilter(void);

/**
 * 	@brief:	filterByHistory
 *
 * 	@note:	 if this time signal has a big change with last 3 times, then not belive it.
 *
 * 	@return:	none
 */
void filterByHistory(void);

/**
 * 	@brief:	filterByHistory
 *
 * 	@note:	 this module is for protect the car when it's running out the track.
 *
 * 	@return:	none
 */
int16_t judgeOutType(void);

/**
 * 	@brief:	judgeStartline
 *
 * 	@note:	 judge if startLine or not
 *
 * 	@return:	none
 */

void judgeCrossline(void);

/**
 * 	@brief:	 findLeftBlackNum
 *
 * 	@note:	 1.find the black dot,scan from right to left,the first black dot is the right one.
 *
 * 	@return:	signalValue
 */
int8_t findLeftBlackNum(void);

/**
 * 	@brief:	 findRightBlackNum
 *
 * 	@note:	 1.find the black dot,scan from right to left,the first black dot is the right one.
 *
 * 	@return:	signalValue
 */
int8_t findRightBlackNum(void);

/**
 * 	@brief:	  getLeftValue
 *
 * 	@note:	 1.calculate signal value using variable blackNum.
 * 					 2.left laser high, while right low.
 * 					 3.if changeNum -1, then set signal value 0.
 * 	@return:	signalValue
 */
uint16_t getLeftValue(int8_t blackNum);

/**
 * 	@brief:	  getRightValue
 *
 * 	@note:	 1.calculate signal value using variable blackNum.
 * 					 2.left laser high, while right low.
 * 					 3.if changeNum -1, then set signal value 0.
 * 	@return:	signalValue
 */
uint16_t getRightValue(int8_t blackNum);

#endif /* FARSIGNALPROCESS_H_ */
