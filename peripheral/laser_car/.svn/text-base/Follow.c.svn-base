/*
 * Follow.c
 *
 *  Created on: 2012-5-28
 *      Author: caoshen
 */
#include "Follow.h"
#include "FarSignalProcess.h"
#include "steer.h"
#include "switch.h"

const int16_t SignalLeftOffsetWeight[UPLEFT_LASER_NUM] = {0, 0, -1, -2, -3, -4, -5, -6, -7, -8};
const int16_t SignalRightOffsetWeight[UPRIGHT_LASER_NUM] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8};
const int16_t SignalSingleLeftOffsetWeight[UPLEFT_LASER_NUM] = {4, 3, 2, 1, 0, -1, -2, -3, -4, -5};
const int16_t SignalSingleRightOffsetWeight[UPRIGHT_LASER_NUM] = {-4, -3, -2, -1, 0, 1, 2, 3, 4, 5};

uint32_t gl_DataSendInt[10] = {0};

static uint8_t s_crossline = 1;
static int16_t s_boundary = 0;
uint8_t gl_curveModel = 0;
uint8_t gl_followModel = 0;
static int16_t s_followAngleIncrement = 0;
int16_t gl_singleSignalOffset =0;
int16_t gl_signalOffset = 0;
uint8_t gl_compensation = 0;

volatile int16_t gl_FollowAngle =0;

/*--------------------------------------------------------------------
   *    Function:     CaculateFollowAngle
   *    Programmers： WangJian、JianLu、WeiYiying.
   *    Description:  calculate follow angle.
   *    Return:       FollowServoTemp
--------------------------------------------------------------------*/
void setFollowAngle(void)
{
    int16_t farSignalOffset = 0;
    static int16_t s_followServoTemp = 0;
    int16_t followAngleIncrement = 0;
    //static uint8_t s_firstTime = 1;
    // static int16_t s_followAngleFirst = 0;
    // static uint8_t s_flag = 0;
    // int16_t followAngleCompensation = 0;

    int8_t switchTemp = 0;
    switchTemp =  (switch_read_8bit(SWITCH_GROUP0) & 0X3F)  >> 1;

    s_followAngleIncrement = 0;
    if(!gl_outType)
    {
        farSignalOffset = getFarSignalOffset();
        gl_signalOffset = farSignalOffset;
        gl_DataSendInt[0] =farSignalOffset;        // for datasend

        if((gl_rightBlackNum == 2) || (gl_leftBlackNum == 2))
        {
            followAngleIncrement = 0;
        }
        else
        {
            followAngleIncrement = followAngle_PID(farSignalOffset);
        }
    }
    else
    {
        followAngleIncrement = followAngle_out();
    }

    s_followServoTemp += followAngleIncrement;
   // gl_DataSendInt[1] = followAngleIncrement;
    //gl_DataSendInt[2] = s_followServoTemp;
   // gl_DataSendInt[5] = followAngleCompensation;

    if(gl_crossline)
    {
        s_followServoTemp = followAngle_crossline();
    }
    else
    {
        s_crossline = 1;
    }
    //gl_DataSendInt[4] = s_crossline;

    if(gl_longStraight)
    {
        s_followServoTemp = 0;
    }

    /* set boundary */
    if(s_followServoTemp > STEER_SERVO_MAX_VALUE)
    {
        s_followServoTemp =  STEER_SERVO_MAX_VALUE;
    }
    else if(s_followServoTemp < STEER_SERVO_MIN_VALUE)
    {
        s_followServoTemp = STEER_SERVO_MIN_VALUE;
    }
    gl_FollowAngle = s_followServoTemp;
}


/*--------------------------------------------------------------------
   *    Function:     followAngle_crossLine
   *    Programmers： WangJian
   *    Description:  set the follow servo angle when the car run into crossline.
   *    Return:       followAngleIncrement.
--------------------------------------------------------------------*/
/*int16_t judgeCrosslineBackOut(void)
{

}*/

int16_t bigFollowAngleCompensation(void)
{
    int16_t followAngleCompensation = 0;
    static uint8_t s_noIncrementTimes = 0;
    int16_t followAngle = 0;

    int8_t rightNum = 0;
    int8_t leftNum = 0;

    rightNum = gl_rightBlackNum;
    leftNum = gl_leftBlackNum;
    followAngle = gl_FollowAngle;

    if((rightNum <= 2) && (leftNum <= 2))
    {
        s_noIncrementTimes ++;
    }
    else
    {
        s_noIncrementTimes = 0;
    }

    if(s_noIncrementTimes >= 3)
    {
        if((followAngle < 1050) && (followAngle > -1050))                         // 700
        {
            followAngleCompensation = 18;                                                    // 15
        }
        if((followAngle < 1250) && (followAngle > -1250))                         // 1000
        {
            followAngleCompensation = 31;                                                    // 25
        }
        else if((followAngle < 1625) && (followAngle > -1625))               // 1300
        {
            followAngleCompensation = 37;                                                    // 30
        }
        else
        {
            followAngleCompensation = 43;                                                      // 35
        }
    }

    if(followAngleCompensation > 0)
    {
        gl_compensation = 1;
    }
    else
    {
        gl_compensation = 0;
    }

    if(followAngle > 0)
    {
        followAngleCompensation = 0 - followAngleCompensation;
    }

    return followAngleCompensation;
}
/*--------------------------------------------------------------------
   *    Function:     followAngle_crossLine
   *    Programmers： WangJian
   *    Description:  set the follow servo angle when the car run into crossline.
   *    Return:       followAngleIncrement.
--------------------------------------------------------------------*/
int16_t followAngle_crossline(void)
{
    int16_t followAngle = 0;

  //  static uint8_t s_oneTime = 1;
   //static uint8_t s_firstTime = 1;
    static int16_t s_decrement = 0;
    //int8_t leftOffsetNum = 0;
    //int8_t rightOffsetNum = 0;
    //int16_t leftOffset = 0;
    //int16_t rightOffset = 0;
   // static int16_t s_offset = 0;
    static uint8_t s_followAngleFlag = 0;
    //int16_t followErrSum = 0;
    int16_t followCompensation = 0;

    uint8_t switchTemp = 0;
    switchTemp = ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F)  >> 1);

    followAngle = gl_FollowAngle;
    followCompensation = crosslineCompensation();

    if(s_crossline)
    {
       s_crossline = 0;
       //getCrosslineNum();
       //leftOffsetNum = s_crosslineLeftNum;
       //rightOffsetNum = s_crosslineRightNum;

       // do with the follow servo offset.
       if((gl_FollowAngle >  2125 ) || (gl_FollowAngle <  -2125))            // 1700
       {
           s_decrement = 81;                                    // 65
           s_boundary = (gl_FollowAngle * 9) / 20;
       }
       else if((gl_FollowAngle > 1875) || (gl_FollowAngle < -1875))   // 1400 -- 1700
       {
           s_decrement = 81;
           s_boundary = (gl_FollowAngle * 8) / 20;
       }
       else if((gl_FollowAngle > 1625) || (gl_FollowAngle < -1625))   // 1300 -- 1400
       {
           s_decrement = 81;
           s_boundary = (gl_FollowAngle * 7) / 20;
       }
       else if((gl_FollowAngle > 1375) || (gl_FollowAngle < -1375))   // 1100 -- 1300
       {
           s_decrement = 75;
           s_boundary = (gl_FollowAngle * 7) / 20;
       }
       else if((gl_FollowAngle > 1125) || (gl_FollowAngle < - 1125))     //900 -- 1100
       {
           s_decrement = 75;
           s_boundary = (gl_FollowAngle * 7) / 20;
       }
       else if((gl_FollowAngle > 875) || (gl_FollowAngle < - 875))     // 700 -- 900
       {
           s_decrement = 75;
           s_boundary = (gl_FollowAngle * 5) / 20;
       }
       else if((gl_FollowAngle > 625) || (gl_FollowAngle < - 625))     // 500 -- 700
       {
           s_decrement = 75;
           s_boundary = (gl_FollowAngle * 5) / 20;
       }
       else if((gl_FollowAngle > 375) || (gl_FollowAngle < - 375))     // 300 -- 500
       {
           s_decrement = 75;
           s_boundary = (gl_FollowAngle * 9) / 20;
       }
       else                                                           // 0 -- 300
       {
           s_decrement = 75;
           //s_boundary = (gl_FollowAngle * 8) / 10;
           s_boundary = (gl_FollowAngle * 8) / 10;
       }

       s_boundary += followCompensation;

       gl_DataSendInt[3] = followCompensation;

       if(gl_FollowAngle > s_boundary)
       {
           s_followAngleFlag = FOLLOW_RIGHT;
       }
       else if(gl_FollowAngle < s_boundary)
       {
           s_followAngleFlag = FOLLOW_LEFT;
       }
       else
       {
           s_followAngleFlag = FOLLOW_MIDDLE;
       }
    }

    // set followAngle.
    if(s_followAngleFlag == FOLLOW_LEFT)
    {
        followAngle = gl_FollowAngle + s_decrement;
        if(followAngle > s_boundary)
        {
            followAngle = s_boundary;
        }
    }
    else if(s_followAngleFlag == FOLLOW_RIGHT)
    {
        followAngle = gl_FollowAngle - s_decrement;
        if(followAngle < s_boundary)
        {
            followAngle = s_boundary;
        }
    }
    else
    {
        followAngle = s_boundary;
    }

    return followAngle;
}

/*--------------------------------------------------------------------
   *    Function:     crosslineCompensation
   *    Programmers： WangJian
   *    Description:  before run into the crossline, there should have compensation for
                      followAngle boundary.
   *    Return:       followAngleIncrement.
--------------------------------------------------------------------*/
int16_t crosslineCompensation(void)
{
    int16_t leftIncrementEnable = 0;
    int16_t rightIncrementEnable = 0;
    int16_t rightFollowErrSum = 0;
    int16_t leftFollowErrSum = 0;
    int16_t followErrSum = 0;
    int16_t i = 0;
    int16_t followCompensation = 0;

    uint8_t switchTemp = 0;
    switchTemp = ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F) >> 1);

    rightIncrementEnable = 1;
    leftIncrementEnable = 1;

    if(rightIncrementEnable)
    {
        for(i=0; i<10; i++)
        {
            rightFollowErrSum += gl_rightNumHistory[i+CROSSLINE_BOUNDARY+1];
        }

        rightFollowErrSum = rightFollowErrSum / 10;
    }

    if(leftIncrementEnable)
    {
        for(i=0; i<10; i++)
        {
            leftFollowErrSum += gl_leftNumHistory[i+CROSSLINE_BOUNDARY+1];
        }

        leftFollowErrSum = leftFollowErrSum / 10;

        leftFollowErrSum = 0 - leftFollowErrSum;
    }

    followErrSum = leftFollowErrSum + rightFollowErrSum;

    if((followErrSum < 2) && (followErrSum > -2))
    {
        followCompensation = followErrSum * 30;
    }
    else if((followErrSum < 5) && (followErrSum > -5))
    {
        followCompensation = followErrSum * 40;
    }
    else
    {
        followCompensation = followErrSum * 50;
    }

    if(followCompensation > 250)
    {
        followCompensation = 250;
    }
    else if(followCompensation < -250)
    {
        followCompensation = -250;
    }

    return followCompensation;
}
/*--------------------------------------------------------------------
   *    Function:     followAngle_out
   *    Programmers： WangJian
   *    Description:  set the follow servo angle by gl_outType.
   *    Return:       followAngleIncrement.
--------------------------------------------------------------------*/
int16_t followAngle_out(void)
{
    int16_t followAngleIncrement = 0;
    int16_t outType = 0;

    // int16_t switchTemp = 0;
    // switchTemp = (ReadSwitch(0X3f) >> 1);

    if(gl_outType < 0)
    {
        outType = 0 - gl_outType;
    }
    else
    {
        outType = gl_outType;
    }

    if(outType == 1)
    {
        followAngleIncrement = 70;
    }
    else if(outType == 2)
    {
        followAngleIncrement = 70;
    }
    else if(outType == 3)
    {
        followAngleIncrement = 70;
    }
    else if(outType == 4)
    {
        followAngleIncrement = 70;
    }

    if(gl_outType > 0)
    {
        followAngleIncrement = 0 - followAngleIncrement;
    }

    return followAngleIncrement;
}

/*--------------------------------------------------------------------
   *    Function:     getFarSignalOffset
   *    Programmers： WangJian、JianLu、WeiYiying.
   *    Description:  calculate signal Offset by far signal value
   *    Return:       offset of signal
--------------------------------------------------------------------*/
int16_t getFarSignalOffset(void)
{
    int16_t offsetLeft = 0;
    int16_t offsetRight = 0;
    int16_t offset = 0;

    // get left offset
    if(gl_leftBlackNum >= 0)
    {
        offsetLeft = SignalLeftOffsetWeight[gl_leftBlackNum];
    }
    else
    {
        offsetLeft = 0;
    }

    // get right offset
    if(gl_rightBlackNum >= 0)
    {
        offsetRight = SignalRightOffsetWeight[gl_rightBlackNum];
    }
    else
    {
        offsetRight = 0;
    }

   // gl_DataSendInt[1] = offsetLeft;
   // gl_DataSendInt[2] = offsetRight;

    // get offset
    offset = offsetLeft + offsetRight;

    return offset;
}


/*--------------------------------------------------------------------
   *    Function:     FollowAnglePID
   *    Programmers： WangJian、JianLu、WeiYiying.
   *    Description:  calculate the increase followangle value by PID
   *    Return:       the increase of follow angle
--------------------------------------------------------------------*/
int16_t followAngle_PID(int16_t farSignalOffset)
{
    int8_t leftBlackNum = 0;
    int8_t rightBlackNum = 0;
    int16_t followAngleIncrement = 0;
    int16_t ptemp = 0;
    int16_t P = 60;
 //   static int16_t followAngleIncrementLast = 0;
  //  int16_t followErrD = 0;
   // int16_t followErrDTemp = 0;

   // static int16_t s_offsetLast = 0;
    int16_t offset = 0;
    //int16_t offsetErr = 0;

    leftBlackNum = gl_leftBlackNum;
    rightBlackNum = gl_rightBlackNum;
    offset = farSignalOffset;


    //for adjust PID value

    ptemp = (int)(((switch_read_8bit(SWITCH_GROUP0) & 0X3F) >> 1));

    if((leftBlackNum >= 2) && (rightBlackNum < 2))
    {
        if(leftBlackNum <= 1)
        {
            P = 0;
        }
        else if(leftBlackNum == 3)
        {
            P = 30;
        }
        else if(leftBlackNum == 4)
        {
            P = 35;
        }
        else if(leftBlackNum == 5)
        {
            P = 45;
        }
        else if(leftBlackNum == 6)
        {
            P = 50;
        }
        else if(leftBlackNum == 7)
        {
            P = 55;
        }
        else if(leftBlackNum == 8)
        {
            P = 60;
        }
        else if(leftBlackNum == 9)
        {
            P = 68;
        }
    }
    else if((rightBlackNum >= 2) && (leftBlackNum < 2))
    {
        if(rightBlackNum <= 1)
        {
            P = 0;
        }
        else if(rightBlackNum == 3)
        {
            P = 30;
        }
        else if(rightBlackNum == 4)
        {
            P = 35;
        }
        else if(rightBlackNum == 5)
        {
            P = 45;
        }
        else if(rightBlackNum == 6)
        {
            P = 50;
        }
        else if(rightBlackNum == 7)
        {
            P = 55;
        }
        else if(rightBlackNum == 8)
        {
            P = 60;
        }
        else if(rightBlackNum == 9)
        {
            P = 68;
        }
    }
    else if((leftBlackNum >= 2) && (rightBlackNum >= 2))
    {
        if(leftBlackNum <= rightBlackNum)
        {
            if(leftBlackNum <= 1)
            {
                P = 0;
            }
            else if(leftBlackNum == 3)
            {
                P = 30;
            }
            else if(leftBlackNum == 4)
            {
                P = 35;
            }
            else if(leftBlackNum == 5)
            {
                P = 45;
            }
            else if(leftBlackNum == 6)
            {
                P = 50;
            }
            else if(leftBlackNum == 7)
            {
                P = 55;
            }
            else if(leftBlackNum == 8)
            {
                P = 60;
            }
            else if(leftBlackNum == 9)
            {
                P = 68;
            }
        }
        else          //if(leftBlackNum > rightBlackNum)
        {

            if(rightBlackNum <= 1)
            {
                P = 0;
            }
            else if(rightBlackNum == 3)
            {
                P = 30;
            }
            else if(rightBlackNum == 4)
            {
                P = 35;
            }
            else if(rightBlackNum == 5)
            {
                P = 45;
            }
            else if(rightBlackNum == 6)
            {
                P = 50;
            }
            else if(rightBlackNum == 7)
            {
                P = 55;
            }
            else if(rightBlackNum == 8)
            {
                P = 60;
            }
            else if(rightBlackNum == 9)
            {
                P = 68;
            }
        }
    }

   ptemp = 8;
    P =  (ptemp * P) / 4;
    followAngleIncrement = (P * farSignalOffset) / 10 ;

    return followAngleIncrement;
}


/*--------------------------------------------------------------------
   *    Function:     singleFollow
   *    Programmers： WangJian
   *    Description:  solve the single follow and double follow problem.
   *    Return:
--------------------------------------------------------------------*/
int16_t singleFollow(void)
{
    int16_t followAngleIncrement = 0;
    followAngleIncrement = s_followAngleIncrement;

    if(gl_followModel == DOUBLE_TO_SINGLE)
    {
        judgeDoubleToSingleFollow();
    }
    else if(gl_followModel == SINGLE_FOLLOW)
    {
        followAngleIncrement = setSingleFollowIncrement();
        judgeSingleToDoubleFollow();
    }

    return  followAngleIncrement;
}


/*--------------------------------------------------------------------
   *    Function:     judgeSingleToDoubleFollow
   *    Programmers： WangJian
   *    Description:  judge if follow model change from single follow to double follow.
   *    Return:
--------------------------------------------------------------------*/
void judgeSingleToDoubleFollow(void)
{
    static int16_t s_peakFollowAngle = 0;
    int16_t followAngleTemp = 0;

    followAngleTemp = gl_FollowAngle;

    if(gl_curveModel == LEFT_CURVE)
    {
        if(followAngleTemp > 900)
        {
            if(followAngleTemp > s_peakFollowAngle)
            {
                s_peakFollowAngle = followAngleTemp;
            }
        }

        if(s_peakFollowAngle > 900)
        {
            if(s_peakFollowAngle > 1500)
            {
                if((s_peakFollowAngle - followAngleTemp) > 500)
                {
                    gl_followModel = DOUBLE_FOLLOW;
                }
            }
            else if(s_peakFollowAngle > 1200)
            {
                if((s_peakFollowAngle - followAngleTemp) > 1000)
                {
                    gl_followModel = DOUBLE_FOLLOW;
                }
            }
            else
            {
                if((s_peakFollowAngle - followAngleTemp) > 900)
                {
                    gl_followModel = DOUBLE_FOLLOW;
                }
            }

        }
        else
        {
            s_peakFollowAngle = 0;
        }

    }
    else if(gl_curveModel == RIGHT_CURVE)
    {
        if(followAngleTemp < -900)
        {
            if(followAngleTemp < s_peakFollowAngle)
            {
                s_peakFollowAngle = followAngleTemp;
            }
        }

        if(s_peakFollowAngle < -900)
        {
            if(s_peakFollowAngle < -1500)
            {
                if((s_peakFollowAngle - followAngleTemp) < -1000)
                {
                    gl_followModel = DOUBLE_FOLLOW;
                }
            }
            else if(s_peakFollowAngle < -1200)
            {
                if((s_peakFollowAngle - followAngleTemp) < -1000)
                {
                    gl_followModel = DOUBLE_FOLLOW;
                }
            }
            else
            {
                if((s_peakFollowAngle - followAngleTemp) < -900)
                {
                    gl_followModel = DOUBLE_FOLLOW;
                }
            }

        }
        else
        {
            s_peakFollowAngle = 0;
        }
    }
    else
    {
        s_peakFollowAngle = 0;
    }

    if(gl_crossline)
    {
        gl_followModel = DOUBLE_FOLLOW;
    }
}

/*--------------------------------------------------------------------
   *    Function:     judgeDoubleToSingleFollow
   *    Programmers： WangJian
   *    Description:  judge if follow model change from double follow to single follow.
   *    Return:
--------------------------------------------------------------------*/
void judgeDoubleToSingleFollow(void)
{
    int16_t followAngleTemp = 0;
    static uint8_t s_firstTime = 1;
    static uint_t s_distance = 0;

    followAngleTemp = gl_FollowAngle;

    if(s_firstTime)
    {
        s_distance = gl_distanceTotal;
        s_firstTime = 0;
    }

   /* if(((followAngleTemp - s_boundary) > 400) || ((s_boundary - followAngleTemp) > 400))
    {
        gl_followModel = SINGLE_FOLLOW;

        if(followAngleTemp > 0)
        {
            gl_curveModel = LEFT_CURVE;
        }
        else
        {
            gl_curveModel = RIGHT_CURVE;
        }

        s_firstTime = 1;
    }*/

    if(((followAngleTemp - s_boundary) > 400) && (followAngleTemp > 0))
    {
        gl_followModel = SINGLE_FOLLOW;
        gl_curveModel = LEFT_CURVE;
    }
    else if(((s_boundary - followAngleTemp) > 400) && (followAngleTemp < 0))
    {
        gl_followModel = SINGLE_FOLLOW;
        gl_curveModel = RIGHT_CURVE;
    }

    if(gl_followModel == SINGLE_FOLLOW)
    {
        s_firstTime = 1;
    }

    if(((gl_distanceTotal - s_distance) > 90) || ((gl_distanceTotal - s_distance) < 50))
    {
        gl_followModel = DOUBLE_FOLLOW;
        s_firstTime = 1;
    }
}


/*--------------------------------------------------------------------
   *    Function:     setSingleFollowIncrement
   *    Programmers： WangJian
   *    Description:  calculate follow increment in single follow model.
   *    Return:       followAngleIncrement
--------------------------------------------------------------------*/
int16_t setSingleFollowIncrement(void)
{
    int16_t signalOffset = 0;
    int16_t followAngleIncrement = 0;
    int16_t P = 0;
    int16_t switchTemp = 0;
    int8_t blackNum = 0;

    switchTemp = ( (switch_read_8bit(SWITCH_GROUP0)& 0X3F)  >> 1);

    blackNum = getBlackNumForSingleFollow();
    gl_DataSendInt[2] = blackNum;

    if(blackNum < 0)
    {
        signalOffset = 0;
    }
    else
    {
        if(gl_curveModel == LEFT_CURVE)
        {
            signalOffset = SignalSingleRightOffsetWeight[blackNum];
        }
        else if(gl_curveModel == RIGHT_CURVE)
        {
            signalOffset = SignalSingleLeftOffsetWeight[blackNum];
        }
    }

    //gl_singleSignalOffset = signalOffset;

    if((blackNum == 3) || (blackNum == 5))
    {
        P = 5;
        // P = switchTemp;
    }
    else if((blackNum == 6) || (blackNum == 7) || (blackNum == 2) || (blackNum == 1))
    {
        // P = 8;
        P = 7;
        // P = switchTemp;
    }
    else if((blackNum == 8) || (blackNum == 0))
    {
        // P = 15;
        P = 7;
        // P = switchTemp;
    }
    else if(blackNum == 9)
    {
        // P = 8;
        P = 8;
        // P = switchTemp;
    }


    followAngleIncrement = P * signalOffset;

    return followAngleIncrement;
}



/*--------------------------------------------------------------------
   *    Function:     getBlackNumForSingleFollow
   *    Programmers： WangJian
   *    Description:  get the right blackNum for single follow.
   *    Return:       blackNum
--------------------------------------------------------------------*/
int8_t getBlackNumForSingleFollow(void)
{
    int8_t blackNum = 4;
    int8_t leftNumLast = 4;
    int8_t rightNumLast = 4;
    int8_t leftNumNow = 4;
    int8_t rightNumNow = 4;

    if(gl_curveModel == LEFT_CURVE)
    {
       rightNumLast = gl_rightNumHistory[1];
       rightNumNow = gl_rightNumHistory[0];

       if((rightNumLast == 9) && (rightNumNow < 7))
       {
           blackNum = 9;
       }
       else if((rightNumLast == 8) && (rightNumNow < 6))
       {
           blackNum = 9;
       }
       else if((rightNumLast == 0) && (rightNumNow < 0))
       {
           blackNum = 0;
       }
       else if((rightNumLast == 1) && (rightNumNow < 0))
       {
           blackNum = 1;
       }
       else if(rightNumNow < 0)
       {
           if((rightNumLast > 1) && (rightNumLast < 8))
           {
               blackNum = rightNumLast;
           }
       }
       else
       {
           blackNum = gl_rightBlackNum;
       }

       gl_rightNumHistory[0] = blackNum;
    }
    else if(gl_curveModel == RIGHT_CURVE)
    {
       leftNumLast = gl_leftNumHistory[1];
       leftNumNow = gl_leftNumHistory[0];

       if((leftNumLast == 9) && (leftNumNow < 7))
       {
           blackNum = 9;
       }
       else if((leftNumLast == 8) && (leftNumNow < 6))
       {
           blackNum = 9;
       }
       else if((leftNumLast == 0) && (leftNumNow < 0))
       {
           blackNum = 0;
       }
       else if((leftNumLast == 1) && (leftNumNow < 0))
       {
           blackNum = 1;
       }
       else if(leftNumNow < 0)
       {
           if((leftNumLast > 1) && (leftNumLast < 8))
           {
               blackNum = leftNumLast;
           }
       }
       else
       {
           blackNum = gl_leftBlackNum;
       }

       gl_leftNumHistory[0] = blackNum;
    }

    return blackNum;
}

/**
 * 	@brief:OutputFollowServo.
 *
 * 	@note:Output Follow Servo PWM
 *
 * 	@return:none
 */
void outputFollowPWM(void)
{
    uint16_t angleTemp = 0;			     //  Servo Output Temp ,Used to Set PWM Duty
    int16_t  angle0 = 0;

    /*if(ReadFunctionSwitch())
    {
        angle0 = (ReadSwitch(0x3F) >> 1) * 5;
    }
    else
    {
        angle0 = 0 - (ReadSwitch(0x3F) >> 1) * 5;
    }*/
    angle0  =  -155;
    angleTemp  =  gl_FollowAngle;
    angleTemp =  angleTemp + angle0;                  /*测舵机中心值是否正确 */
    //angleTemp = angle0;
    steer_output_value(STEER_SERVO, angleTemp);
}
