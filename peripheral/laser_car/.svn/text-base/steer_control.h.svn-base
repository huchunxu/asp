/*********************************************************************
 *   Module Name:  Steer.h
 *   Programmers:  WangJian、JianLu、WeiYiying.(XS128 version)
 *   Description:  Header for Steer.c.
 *   Modification History	: 2011.10.15
 *	(c) Copyright 2011 RENESAS LAB, Department of CSE, HUST.
                        All Rights Reserved
*********************************************************************/
#ifndef _STEER_H_
#define _STEER_H_

#include "t_stddef.h"

#define RUN_IN_LEFT_CURVE     1
#define RUN_OUT_LEFT_CURVE    2
#define RUN_IN_RIGHT_CURVE    3
#define RUN_OUT_RIGHT_CURVE   4
#define IN_TO_OUT_CURVE       5
#define OUT_TO_IN_CURVE       6
#define STRAIGHT_TO_LEFT_CURVE   1
#define STRAIGHT_TO_RIGHT_CURVE   2

extern uint32_t    gl_crossline;
extern volatile int16_t gl_FollowAngle;
extern int8_t		gl_leftBlackNum;
extern int8_t      gl_rightBlackNum;
extern uint16_t	gl_distanceTotal;

void setSteerAngle(void);
int16_t SteerOnCurve(void);
int16_t steerOnStraight(void);
uint32_t judgeCarStateOnCurve(void);
void judgeStraightToCurve(void);
void judgeLongStraight(void);
int16_t steerIncrementLongStraightToCurve(void);

/**
 * 	@brief:OutputSteerServo.
 *
 * 	@note:Output Steer Servo PWM
 *
 * 	@return:none
 */
void outputSteerPWM(void);

#endif
