/*********************************************************************

 *   Module Name:  Steer.c
 *   Programmers:  WangJian、JianLu、WeiYiying(XS128 version)
 *   Description:  Steer Control Process
 *   Modification History	:  2011.10.15
 *	(c) Copyright 2011 RENESAS LAB, Department of CSE, HUST.
                        All Rights Reserved
*********************************************************************/
#include "steer_control.h"
#include "steer.h"
#include "switch.h"

uint32_t gl_carStateOnCurve = 0;
uint32_t gl_longStraight = 0;
uint32_t gl_maybeLongStraightToCurve = 0;
static   uint32_t s_firstTime0 = 1;
static   uint32_t s_longStraigthToCurveType = 0;
uint32_t gl_steerStraightToCurveAddTime = 0;
static   uint_t s_longStraightDistance = 0;
static   int16_t s_followAngleErr = 0;
volatile int16_t gl_SteerAngle = 0;

/*--------------------------------------------------------------------
   *    Function:     CaculateSteerAngle.
   *    Programmers:  WangJian、JianLu、WeiYiying(XS128 version)
   *    Description:  calculate steer angle by signal
   *    Retrun:       steer angle
--------------------------------------------------------------------*/
void setSteerAngle(void)
{
    static int16_t s_steerAngleTemp = 0;
    int16_t steerAngleIncrement = 0;
    static uint32_t s_firstTime = 1;

    int16_t followAngle = 0;
    static int16_t followAngleLast = 0;

    followAngle = gl_FollowAngle;
    s_followAngleErr = followAngle - followAngleLast;
    followAngleLast  = followAngle;

    judgeLongStraight();
    judgeStraightToCurve();

    if(gl_crossline)
    {
        gl_longStraight = 0;
        gl_maybeLongStraightToCurve = 0;
    }

    if(gl_longStraight)
    {
        s_firstTime = 1;
        s_steerAngleTemp = 0;
        steerAngleIncrement = steerOnStraight();    // trace on straight
    }
    else
    {
        s_firstTime0 = 1;
        if((s_firstTime) && (gl_maybeLongStraightToCurve))
        {
            steerAngleIncrement = gl_SteerAngle;
            s_firstTime = 0;
        }
        else
        {
            steerAngleIncrement = SteerOnCurve();        // trace on curve
        }
    }

    s_steerAngleTemp = steerAngleIncrement;

    if(s_steerAngleTemp > STEER_SERVO_MAX_VALUE)    // STEER_SERVO_MAX_VALUE
    {
        s_steerAngleTemp =  STEER_SERVO_MAX_VALUE;
    }
    else if(s_steerAngleTemp < STEER_SERVO_MAX_VALUE)  //STEER_SERVO_MIN_VALUE
    {
        s_steerAngleTemp = STEER_SERVO_MAX_VALUE;
    }

   gl_SteerAngle = s_steerAngleTemp;
}

/*--------------------------------------------------------------------
   *    Function:     SteerOnCurve.
   *    Programmers： WangZhi
   *    Description:  steer strategy on curve road
   *    Retrun:       steer angle
--------------------------------------------------------------------*/
int16_t SteerOnCurve(void)
{
    int16_t steerAngleTemp = 0;
    int16_t followAngleTemp = 0;
    int16_t followAngleAbs = 0;

    int16_t steerAngleIncrement = 0;
    int16_t steerFactor = 0;
    int16_t followErrD = 0;

//    char Ptemp = 30;
    int16_t switchTemp = 0;

    followAngleTemp = gl_FollowAngle;

  /*  if(!ReadFunctionSwitch())
    {
        switchTemp = ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F)  >> 1);
    }
    else
    {
        switchTemp = 0 - ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F)  >> 1);
    }

    Ptemp = switchTemp;*/
    switchTemp =  ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F)  >> 1);

    if(followAngleTemp < 0)
    {
        followAngleAbs = 0 - followAngleTemp;
    }
    else
    {
        followAngleAbs = followAngleTemp;
    }

    steerFactor = followAngleAbs / (10 * 18);
    steerFactor  +=  switchTemp;

    if(followAngleAbs < 375)    // 0 -- 300
    {
        followErrD = (s_followAngleErr * 4) / 20;
    }
    else if(followAngleAbs < 500) // 300 -- 400
    {
        followErrD = (s_followAngleErr * 5) / 20;
    }
    else if(followAngleAbs < 625) // 400 -- 500
    {
        followErrD = (s_followAngleErr * 6) / 20;
    }
    else if(followAngleAbs < 750) // 500 -- 600
    {
        followErrD = (s_followAngleErr * 7) / 20;
    }
    else
    {
        followErrD = (s_followAngleErr * 8) / 20;  // beyond 1000
    }

    if(followErrD > 10)
    {
        followErrD = 10;
    }
    else if(followErrD < (0 - 10))
    {
        followErrD = 0 - 10;
    }

    if(gl_crossline)
    {
        	steerFactor += 6;
    }

    //gl_DataSendInt[4] = followErrD;
    steerAngleIncrement = followAngleTemp / steerFactor + followErrD;
    steerAngleTemp = steerAngleIncrement * 20;

    return steerAngleTemp;
}

/*--------------------------------------------------------------------
   *    Function:     steerOnStraight.
   *    Programmers： WangJian
   *    Description:  set steer angle for straight trace.
   *    Retrun:       steer angle
--------------------------------------------------------------------*/
int16_t steerOnStraight(void)
{
    int16_t signalOffset = 0;
    int16_t offsetLeft = 0;
    int16_t offsetRight = 0;
    int16_t switchTemp = 0;
    //int16_t Ptemp = 90;

    int16_t leftNum = 0;
    int16_t rightNum = 0;
    int16_t steerAngleTemp = 0;
    static int16_t s_steerAngleTemp = 0;

    leftNum = gl_leftBlackNum;
    rightNum = gl_rightBlackNum;

    switchTemp =  (switch_read_8bit(SWITCH_GROUP0)& 0X3F)  >> 1;

    if(s_firstTime0)
    {
        s_firstTime0 = 0;
        s_steerAngleTemp = 0;
    }

    offsetLeft = 2 - gl_leftBlackNum;
    offsetRight = gl_rightBlackNum - 2;

    signalOffset = offsetRight;

    /* left erroneous tendency */
    if((leftNum == 2) || (rightNum == 2))
    {
        steerAngleTemp = 0;
    }
    else if((leftNum == 3) && (rightNum == 1))
    {
        steerAngleTemp = -112;                                       // -45
    }
    else if((leftNum == 3) && (rightNum == 0))
    {
        steerAngleTemp = -150;                                      // -60
    }
    else if((leftNum == 3) && (rightNum == -1))
    {
        steerAngleTemp = -175;                                      //-70
    }
    else if((leftNum == 4) && (rightNum == 1))
    {
        steerAngleTemp = -200;                                    //-80
    }
    else if((leftNum == 4) && (rightNum == 0))
    {
        steerAngleTemp = -225;                                  //-90
    }
    else if((leftNum == 4) && (rightNum == -1))
    {
        steerAngleTemp = -250;                             // -100
    }
    else if((leftNum == 5) && (rightNum == 0))
    {
        steerAngleTemp = -275;                           // -110
    }
    else if((leftNum == 5) && (rightNum == -1))
    {
        steerAngleTemp = -300;                           // -120
    }
    /* right erroneous tendency */
    else if((rightNum == 3) && (leftNum == 1))
    {
        steerAngleTemp = 112;                            //45
    }
    else if((rightNum == 3) && (leftNum == 0))
    {
        steerAngleTemp = 150;                            // 60
    }
    else if((rightNum == 3) && (leftNum == -1))
    {
        steerAngleTemp = 175 ;                           //70
    }
    else if((rightNum == 4) && (leftNum == 1))
    {
        steerAngleTemp = 200;                             //80
    }
    else if((rightNum == 4) && (leftNum == 0))
    {
        steerAngleTemp = 225 ;                             //90
    }
    else if((rightNum == 4) && (leftNum == -1))
    {
        steerAngleTemp = 250;                         //100
    }
    else if((rightNum == 5) && (leftNum == 0))
    {
        steerAngleTemp = 275;                      //110
    }
    else if((rightNum == 5) && (leftNum == -1))
    {
        steerAngleTemp = 300;                        //120
    }
    else
    {
        steerAngleTemp = s_steerAngleTemp;
    }
    s_steerAngleTemp = steerAngleTemp;

    return steerAngleTemp;
}

/*--------------------------------------------------------------------
   *    Function:     judgeCarStateOnCurve.
   *    Programmers： WangJian.
   *    Description:  judge car state on curve,there can be 4 states.
   *    Retrun:       carState.
--------------------------------------------------------------------*/
uint32_t judgeCarStateOnCurve(void)
{
	uint32_t carState = 0;
    static uint32_t s_carStateLast = 0;
    static int16_t s_followAngleLast = 0;
    int16_t followAngleTemp = 0;
    static uint_t s_equalTimes = 0;  //when state change (from in_curve to out_curve or from out_curve to in_curve),the gl_FollowAngle canbe same for many times.

    followAngleTemp = gl_FollowAngle;

    if(followAngleTemp > 0) // left curve
    {
        if(followAngleTemp >= s_followAngleLast)
        {
            carState = RUN_IN_LEFT_CURVE;
            s_equalTimes = 0;
        }
        else if(followAngleTemp <= s_followAngleLast)
        {
            carState = RUN_OUT_LEFT_CURVE;
            s_equalTimes = 0;
        }
    }
    else if(followAngleTemp < 0) // right curve
    {
        if(followAngleTemp <= s_followAngleLast)
        {
            carState = RUN_IN_RIGHT_CURVE;
        }
        else if(followAngleTemp >= s_followAngleLast)
        {
            carState = RUN_OUT_RIGHT_CURVE;
        }
    }
    else
    {
        carState = 0;
    }

    s_followAngleLast = followAngleTemp;
    s_carStateLast = carState;

    return carState;
}


/*--------------------------------------------------------------------
   *    Function:     judgeLongStraight
   *    Programmers： WangJian
   *    Description:
   *    Return:       none.
--------------------------------------------------------------------*/
void judgeLongStraight(void)
{
    static uint32_t s_startRecord = 1;
    static uint_t  s_distance = 0;
    int16_t steerAngleTemp = 0;
    int16_t followAngleTemp = 0;

    steerAngleTemp = gl_SteerAngle;
    followAngleTemp = gl_FollowAngle;

    if(gl_maybeLongStraightToCurve)
    {
        s_startRecord = 1;
        s_distance = gl_distanceTotal;
    }
    if((gl_leftBlackNum <= 5)&&(gl_rightBlackNum <= 5))
    {
      /* XS */
    	//if((((steerAngleTemp < 250) && (steerAngleTemp > -250)) && ((followAngleTemp > -200) && (followAngleTemp < 200))) || gl_longStraight)
    	/* K60 */
    	if((((steerAngleTemp < 625) && (steerAngleTemp > -625)) && ((followAngleTemp > -250) && (followAngleTemp < 250))) || gl_longStraight)
    	{
            if(s_startRecord)
            {
                s_distance = gl_distanceTotal;
                s_startRecord = 0;
            }
        }
        else
        {
            s_startRecord = 1;
            s_distance = gl_distanceTotal;
        }
    }
    else
    {
        s_startRecord = 1;
        s_distance = gl_distanceTotal;
    }

    if(gl_crossline)
    {
        s_startRecord = 1;
        s_distance = gl_distanceTotal;
    }

    //gl_DataSendUint[3] = s_distance;
    if((gl_distanceTotal - s_distance) > 100)
    {
        gl_longStraight = 1;
    }
}

/*--------------------------------------------------------------------
   *    Function:     judgeStraightToCurve
   *    Programmers： WangJian
   *    Description:
   *    Return:       none.
--------------------------------------------------------------------*/
void judgeStraightToCurve(void)
{
    int16_t steerAngleTemp = 0;
    static uint32_t s_maybeStraightToCurveCnt = 0;
    static uint32_t s_blackNum5Cnt = 0;
    static uint32_t s_longStraightToCurveTimes = 0;

    steerAngleTemp = gl_SteerAngle;

    if(gl_longStraight)
    {
        if((gl_leftBlackNum > 4) || (gl_rightBlackNum > 4))
        {
            if((gl_leftBlackNum == 5) || (gl_rightBlackNum == 5))
            {
                s_blackNum5Cnt ++;
            }
            else
            {
                s_blackNum5Cnt = 0;
            }

            if((gl_leftBlackNum > 5) || (gl_rightBlackNum > 5))
            {
                s_maybeStraightToCurveCnt ++;
            }
            else
            {
                s_maybeStraightToCurveCnt = 0;
            }
            //s_maybeStraightToCurveCnt ++;
        }
        else
        {
            s_maybeStraightToCurveCnt = 0;
            s_blackNum5Cnt = 0;
        }


        if((s_maybeStraightToCurveCnt >= 3) || (s_blackNum5Cnt >= 10))
        //if(s_maybeStraightToCurveCnt >= 5)
        {
            s_maybeStraightToCurveCnt = 0;
            s_blackNum5Cnt = 0;
            gl_maybeLongStraightToCurve = 1;
            gl_longStraight = 0;
        }

        if((steerAngleTemp > 750) || (steerAngleTemp < -750))
        {
            s_maybeStraightToCurveCnt = 0;
            gl_maybeLongStraightToCurve = 1;
            gl_longStraight = 0;
        }

        if(steerAngleTemp > 0)
        {
            s_longStraigthToCurveType = STRAIGHT_TO_LEFT_CURVE;
        }
        else if(steerAngleTemp < 0)
        {
            s_longStraigthToCurveType = STRAIGHT_TO_RIGHT_CURVE;
        }

       /* if(gl_longStraightToCurve)
        {
            if((gl_distanceTotal - s_longStraightDistance) < 50)
            {
                gl_longStraightToCurve = 0;
                s_longStraigthToCurveType = 0;
            }
        }*/
    }
    else
    {
        s_longStraightDistance =  gl_distanceTotal;
        s_maybeStraightToCurveCnt = 0;
        s_blackNum5Cnt = 0;
    }

    if(gl_maybeLongStraightToCurve)
    {
        s_longStraightToCurveTimes ++;
    }

    if(s_longStraightToCurveTimes > 2)
    {
        gl_maybeLongStraightToCurve = 0;
        s_longStraightToCurveTimes = 0;
    }
}

/**
 * 	@brief:OutputSteerServo.
 *
 * 	@note:Output Steer Servo PWM
 *
 * 	@return:none
 */
void outputSteerPWM(void)
{
    uint16_t angleTemp = 0;			     //  Servo Output Temp ,Used to Set PWM Duty
    int16_t angle0 = 0;

    if(switch_read(SWITCH_START1))
    {
        angle0 = ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F) >> 1) * 5;
    }
    else
    {
        angle0 = 0 - ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F) >> 1) * 5;
    }
    angle0 = -590;

    angleTemp = gl_FollowAngle;
    angleTemp = angleTemp + angle0;           /*测舵机中心值是否正确 */
    //angleTemp = angle0 - 590;
    steer_output_value(STEER_DIR, angleTemp);
}
