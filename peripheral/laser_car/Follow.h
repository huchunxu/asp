/*
 * Follow.h
 *
 *  Created on: 2012-5-28
 *      Author: caoshen
 */

#ifndef FOLLOW_H_
#define FOLLOW_H_

#include "t_stddef.h"
#include "GetFarSignalValue.h"

#define FOLLOW_LEFT    1
#define FOLLOW_RIGHT   2
#define FOLLOW_MIDDLE  3

#define DOUBLE_FOLLOW    0
#define SINGLE_FOLLOW    1
#define SINGLE_TO_DOUBLE 2
#define DOUBLE_TO_SINGLE 3

#define LEFT_ONLINE   1
#define RIGHT_ONLINE  2

extern uint32_t    gl_crossline;
extern uint16_t	gl_distanceTotal;
extern int8_t		gl_leftBlackNum;
extern int8_t      gl_rightBlackNum;
extern int16_t	    gl_outType;
extern uint32_t gl_longStraight;
extern int8_t   gl_leftNumHistory[HISTORY_SIGNAL_NUM];
extern int8_t   gl_rightNumHistory[HISTORY_SIGNAL_NUM];

void setFollowAngle(void);
int16_t getFarSignalOffset(void);
int16_t followAngle_PID(int16_t farSignalOffset);
int16_t followAngle_out(void);
int16_t followAngle_crossline(void);
int16_t crosslineCompensation(void);
int16_t bigFollowAngleCompensation(void);


/* single follow */
int16_t singleFollow(void);
void judgeDoubleToSingleFollow(void);
void judgeSingleToDoubleFollow(void);
int8_t getBlackNumForSingleFollow(void);
int16_t setSingleFollowIncrement(void);
/**
 * 	@brief:OutputFollowServo.
 *
 * 	@note:Output Follow Servo PWM
 *
 * 	@return:none
 */
void outputFollowPWM(void);

#endif /* FOLLOW_H_ */
