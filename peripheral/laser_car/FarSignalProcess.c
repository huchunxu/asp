/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     FarSignalProcess.c
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

#include "FarSignalProcess.h"
#include "Follow.h"
#include "speaker.h"

/**
 * 文件内部宏定义
 */
int8_t		gl_leftBlackNum = 2;
int8_t      gl_rightBlackNum = 2;
int8_t  	gl_leftNumOrig = 2;
int8_t   	gl_rightNumOrig = 2;
uint32_t    gl_leftValue = 0;
uint32_t    gl_rightValue = 0;
uint32_t    gl_leftValueOrig = 0;
uint32_t    gl_rightValueOrig = 0;
int16_t	    gl_outType = 0;
uint32_t    gl_crossline = 0;
uint8_t     gl_straightToCurve = 0;
static      uint8_t s_crosslineBack = 0;   // if car run from crossline to normal line, set this variable.

int8_t   gl_leftNumHistory[HISTORY_SIGNAL_NUM] = {0};
int8_t   gl_rightNumHistory[HISTORY_SIGNAL_NUM] = {0};
int8_t   gl_leftOrigNumHistory[HISTORY_SIGNAL_NUM] = {0};
int8_t   gl_rightOrigNumHistory[HISTORY_SIGNAL_NUM] = {0};

static int8_t s_leftBlackNumLast = 2;
static int8_t s_rightBlackNumLast = 2;

uint8_t  gl_ramp = 0;

/**
 * 	@brief:farSignalProcess
 *
 * 	@note:
 */
void farSignalProcess(void)
{
    gl_leftNumOrig  = findLeftBlackNum();
    gl_rightNumOrig = findRightBlackNum();

    /* gl_leftValueOrig and gl_rightValueOrig are only for SD print. */
    gl_leftValueOrig  = getLeftValue(gl_leftNumOrig);
    gl_rightValueOrig = getRightValue(gl_rightNumOrig);


//    if(ReadSwitch(0X40))
//    {
//        //judgeStartline();
//    }

    /* judge if it is crossline, set gl_crossline, otherwise clear gl_crossline. */
    judgeCrossline();

    if(!gl_crossline)
    {
        farSignalFilter();
    }
    else
    {
        gl_leftBlackNum = -1;
        gl_rightBlackNum = -1;
    }

   // farSignalFilter();

    s_leftBlackNumLast = gl_leftBlackNum;
    s_rightBlackNumLast = gl_rightBlackNum;

    /* gl_leftValue and gl_rightValue are only for SD print. */
    gl_leftValue = getLeftValue(gl_leftBlackNum);
    gl_rightValue = getRightValue(gl_rightBlackNum);

    gl_outType = judgeOutType();

    getHistorySignal();

    if(gl_crossline)
    {
        speaker_open(SPEAKER0);
    	//BUZZER_ON();
    }
    else
    {
        speaker_close(SPEAKER0);
    	//BUZZER_OFF();
    }
}

/**
 * 	@breif: farSignalFilter
 *
 * 	@note:because the neighbouring two lasers's gap is 2.5cm, and the blackline  is also 2.5cm, so signal maybe wrong.and this function is to solve this problem.
 *
 * 	@return:none
 */
void farSignalFilter(void)
{
   laser2_5GapFilter();
   //if(!gl_startLine)
   {
	    filterByHistory();
   }
   if(gl_followModel == DOUBLE_FOLLOW)
   {
       alterManyTimesFilter();
   }
}

/**
 * 	@breif: getHistorySignal
 *
 * 	@note:get the last 10 times signals.
 *
 * 	@return:none
 */
void getHistorySignal(void)
{
    uint8_t i = 0;

    for(i=(HISTORY_SIGNAL_NUM-1); i>0; i--)
    {
        gl_leftNumHistory[i] = gl_leftNumHistory[i-1];
        gl_rightNumHistory[i] = gl_rightNumHistory[i-1];

        gl_leftOrigNumHistory[i] = gl_leftOrigNumHistory[i-1];
        gl_rightOrigNumHistory[i] = gl_rightOrigNumHistory[i-1];
    }

    gl_leftNumHistory[0] = gl_leftBlackNum;
    gl_rightNumHistory[0] = gl_rightBlackNum;

    gl_leftOrigNumHistory[0] = gl_leftNumOrig;
    gl_rightOrigNumHistory[0] = gl_rightNumOrig;
}


/**
 * 	@brief:	laser2_5GapFilter
 *
 * 	@note:	if filter alters same orignal signal for many times, then belive the orignal signal.
 *
 * 	@return:	none
 */
void laser2_5GapFilter(void)
{
    int8_t leftBlackNumOrig = 2;
    int8_t rightBlackNumOrig = 2;

    leftBlackNumOrig  = gl_leftNumOrig;
    rightBlackNumOrig = gl_rightNumOrig;

    gl_leftBlackNum  = gl_leftNumOrig;
    gl_rightBlackNum = gl_rightNumOrig;

    //////  a black dot apear in middle //////
    if(s_leftBlackNumLast < 0)
    {
        if((gl_leftBlackNum > 2) && (gl_leftBlackNum < 6))
        {
            gl_leftBlackNum = s_leftBlackNumLast;
        }
        else if(gl_leftBlackNum == 2)
        {
            if(s_rightBlackNumLast > 3)
            {
                gl_leftBlackNum = s_leftBlackNumLast;
            }
        }
    }
    if(s_rightBlackNumLast < 0)
    {
        if((gl_rightBlackNum > 2) && (gl_rightBlackNum < 6))
        {
            gl_rightBlackNum = s_rightBlackNumLast;
        }
        else if(gl_rightBlackNum == 2)
        {
            if(s_leftBlackNumLast > 3)
            {
                gl_rightBlackNum = s_rightBlackNumLast;
            }
        }
    }

    /////////  all white  //////////
    // if black line between two laser dots, then all white.
    if(leftBlackNumOrig < 0)
    {
        if((s_leftBlackNumLast > 2) && (s_leftBlackNumLast < 6))
        {
            gl_leftBlackNum = s_leftBlackNumLast;
        }
        else if((s_leftBlackNumLast < 2) && ((rightBlackNumOrig < 2) && (rightBlackNumOrig >= 0)))
        {
            gl_leftBlackNum = s_leftBlackNumLast;
        }
    }
    if(rightBlackNumOrig < 0)
    {
        if((s_rightBlackNumLast > 2) && (s_rightBlackNumLast < 6))
        {
            gl_rightBlackNum = s_rightBlackNumLast;
        }
        else if((s_rightBlackNumLast < 2 ) && ((leftBlackNumOrig <= 3) && (leftBlackNumOrig >= 0)))
        {
            gl_rightBlackNum = s_rightBlackNumLast;
        }
    }

    /////////  big change  //////////
    // if black line between two laser dots, there maybe apear black dot.
    if((s_leftBlackNumLast >= 0) && (gl_leftBlackNum >= 0))
    {
        if(gl_followModel != SINGLE_FOLLOW)
        {
            if(((s_leftBlackNumLast - gl_leftBlackNum) > 2) || ((gl_leftBlackNum - s_leftBlackNumLast) > 2))
            {
                gl_leftBlackNum = s_leftBlackNumLast;
            }
            /*else if((s_leftBlackNumLast - gl_leftBlackNum) == 2)
            {
                if(gl_FollowAngle > -1300)
                {
                    if((s_rightBlackNumLast >= gl_rightBlackNum) && (s_rightBlackNumLast >= 0))
                    {
                        gl_leftBlackNum = s_leftBlackNumLast;
                    }
                }
            }*/
        }
        else  //gl_followModel == SINGLE_FOLLOW
        {
            if(((s_leftBlackNumLast - gl_leftBlackNum) > 1) || ((gl_leftBlackNum - s_leftBlackNumLast) > 1))
            {
                gl_leftBlackNum = s_leftBlackNumLast;
            }
        }
    }
    if((s_rightBlackNumLast >= 0) && (gl_rightBlackNum >= 0))
    {
        if(gl_followModel != SINGLE_FOLLOW)
        {
            if(((s_rightBlackNumLast - gl_rightBlackNum) > 2) || ((gl_rightBlackNum - s_rightBlackNumLast) > 2))
            {
                gl_rightBlackNum = s_rightBlackNumLast;
            }
        /*    else if((s_rightBlackNumLast - gl_rightBlackNum) == 2)
            {
                if(gl_FollowAngle < 1300)
                {
                    if((s_leftBlackNumLast >= gl_leftBlackNum) && ( s_leftBlackNumLast >= 0))
                    {
                        gl_rightBlackNum = s_rightBlackNumLast;
                    }
                }
            }*/
        }
        else
        {
            if(((s_rightBlackNumLast - gl_rightBlackNum) > 1) || ((gl_rightBlackNum - s_rightBlackNumLast) > 1))
            {
                gl_rightBlackNum = s_rightBlackNumLast;
            }
        }
    }

    // if last time black line between two laser dots; but this time the signal is right.
    if(((leftBlackNumOrig - s_leftBlackNumLast) > 1) && (!gl_longStraight))
    {
        if(s_leftBlackNumLast >= 0)
        {
            gl_leftBlackNum = leftBlackNumOrig;
        }
    }
    if(((rightBlackNumOrig - s_rightBlackNumLast) > 1) && (!gl_longStraight))
    {
        if(s_rightBlackNumLast >= 0)
        {
            gl_rightBlackNum = rightBlackNumOrig;
        }
    }
}

/**
 * 	@brief:	alterManyTimesFilter
 *
 * 	@note:	if filter alters same orignal signal for many times, then belive the orignal siganl.
 *
 * 	@return:	none
 */
void alterManyTimesFilter(void)
{
//    static uint8_t s_onetimeFlag1 = 0;
//    static uint8_t s_onetimeFlag2 = 0;
    static uint8_t s_leftalterTimes = 0;     //after filter, if gl_leftBlackNum != gl_leftNumOrig, s_leftalterTimes ++
    static uint8_t s_rightalterTimes = 0;     //after filter, if gl_rightBlackNum != gl_rightNumOrig, s_rightalterTimes ++
    static int8_t s_leftNumTemp = 2;
    static int8_t s_rightNumTemp = 2;
    static int8_t s_leftOrigNumTemp = 2;
    static int8_t s_rightOrigNumTemp = 2;

    // if filter alters same orignal right signal for 10 times, then belive the orignal right siganl.
    // however, this is not apply to straight modle.
    if(gl_rightBlackNum != gl_rightNumOrig)
    {
       /* if(!s_onetimeFlag1)
        {
            s_rightNumTemp = gl_rightBlackNum;
            s_rightOrigNumTemp = gl_rightNumOrig;
            s_onetimeFlag1 = 1;
            s_rightalterTimes ++;
        }
        else
        {
            if((gl_rightBlackNum == s_rightNumTemp) && (s_rightOrigNumTemp == gl_rightNumOrig))
            {
                s_rightalterTimes ++;
            }
            else
            {
                s_rightalterTimes = 0;
                s_onetimeFlag1 = 0;
            }
        }*/
        s_rightalterTimes ++;
        s_rightNumTemp = gl_rightBlackNum;
        s_rightOrigNumTemp = gl_rightNumOrig;
    }
    else
    {
        //s_onetimeFlag1 = 0;
        s_rightalterTimes = 0;
    }

    if(s_rightalterTimes > 0)
    {
        if(((s_rightNumTemp - s_rightOrigNumTemp) > 2) || ((s_rightOrigNumTemp - s_rightNumTemp) > 2))
        {
            if(s_rightalterTimes >= 3)
            {
                gl_rightBlackNum = gl_rightNumOrig;
            }
        }
        else
        {
            if(s_rightalterTimes >= 2)
            {
                gl_rightBlackNum = gl_rightNumOrig;
            }
        }
    }

    // if filter alters same orignal left signal for 10 times, then belive the orignal left siganl.
    // however, this is not apply to straight modle.
    if(gl_leftBlackNum != gl_leftNumOrig)
    {
        /*if(!s_onetimeFlag2)
        {
            s_leftNumTemp = gl_leftBlackNum;
            s_leftOrigNumTemp = gl_leftNumOrig;
            s_onetimeFlag2 = 1;
            s_leftalterTimes ++;
        }
        else
        {
            if((gl_leftBlackNum == s_leftNumTemp) && (s_leftOrigNumTemp == gl_leftNumOrig))
            {
                s_leftalterTimes ++;
            }
            else
            {
                s_leftalterTimes = 0;
                s_onetimeFlag2 = 0;
            }
        }*/
        s_leftalterTimes ++;
        s_leftNumTemp = gl_leftBlackNum;
        s_leftOrigNumTemp = gl_leftNumOrig;
    }
    else
    {
        //s_onetimeFlag2 = 0;
        s_leftalterTimes = 0;
    }

    if(s_leftalterTimes > 0)
    {
        if(((s_leftNumTemp - s_leftOrigNumTemp) > 2) || ((s_leftOrigNumTemp - s_leftNumTemp) > 2))
        {
            if(s_leftalterTimes >= 3)
            {
                gl_leftBlackNum = gl_leftNumOrig;
            }
        }
        else
        {
            if(s_leftalterTimes >= 2)
            {
                gl_leftBlackNum = gl_leftNumOrig;
            }
        }
    }

    // if it's crossline, the alter times can't be to large.
    if((gl_leftBlackNum > 4) && (gl_rightNumOrig < 0))
    {
        if((gl_leftNumOrig < 0) && (s_leftalterTimes > 2))
        {
            gl_leftBlackNum = gl_leftNumOrig;
        }
    }
    if((gl_rightBlackNum > 4) && (gl_leftNumOrig < 0))
    {
        if((gl_rightNumOrig < 0) && (s_leftalterTimes > 2))
        {
            gl_rightBlackNum = gl_rightNumOrig;
        }
    }
}

/**
 * 	@brief:	filterByHistory
 *
 * 	@note:	 if this time signal has a big change with last 3 times, then not belive it.
 *
 * 	@return:	none
 */
void filterByHistory(void)
{
    static int8_t s_leftNumHistory[4] = {0};
    static int8_t s_rightNumHistory[4] = {0};
    int8_t leftNumMax = -1;
    int8_t leftNumMin = 9;
    int8_t rightNumMax = -1;
    int8_t rightNumMin = 9;

    uint8_t i = 0;
    static uint8_t s_cnt = 0;
    static uint8_t s_startRecord = 0;

    for(i=3; i>0; i--)
    {
        s_leftNumHistory[i] = s_leftNumHistory[i-1];
        s_rightNumHistory[i] = s_rightNumHistory[i-1];
    }

    s_leftNumHistory[0]  = gl_leftBlackNum;
    s_rightNumHistory[0] = gl_rightBlackNum;

    for(i=3; i>0; i--)
    {
        if(s_leftNumHistory[i] > leftNumMax)
        {
            leftNumMax = s_leftNumHistory[i];
        }
        if(s_leftNumHistory[i] < leftNumMin)
        {
            leftNumMin = s_leftNumHistory[i];
        }

        if(s_rightNumHistory[i] > rightNumMax)
        {
            rightNumMax = s_rightNumHistory[i];
        }
        if(s_rightNumHistory[i] < rightNumMin)
        {
            rightNumMin = s_rightNumHistory[i];
        }
    }

    if(s_crosslineBack)
    {
        s_startRecord = 1;
    }

    if(s_startRecord)
    {
        s_cnt ++;
        gl_leftBlackNum = gl_leftNumOrig;
        gl_rightBlackNum = gl_rightNumOrig;
    }
    if(s_cnt > 12)
    {
        s_cnt = 0;
        s_startRecord = 0;
    }

    if(!s_startRecord)
    {
        if(((gl_leftBlackNum - leftNumMax) > 3) || ((leftNumMin - gl_leftBlackNum) > 3))
        {
            gl_leftBlackNum = s_leftNumHistory[1];
        }
        if(((gl_rightBlackNum - rightNumMax) > 3) || ((rightNumMin - gl_rightBlackNum) > 3))
        {
            gl_rightBlackNum = s_rightNumHistory[1];
        }
    }

    // run from crossline model back to normal model.
    if(s_crosslineBack)
    {
        s_crosslineBack = 0;
        //gl_leftBlackNum = gl_leftNumOrig;
        //gl_rightBlackNum = gl_rightNumOrig;
    }
    // run from maybecrossline model back to normal model.
    /*if(s_maybecrosslineBack)
    {
        s_maybecrosslineBack = 0;
        gl_leftBlackNum = gl_leftNumOrig;
        gl_rightBlackNum = gl_rightNumOrig;
    }*/
}

/**
 * 	@brief:	filterByHistory
 *
 * 	@note:	 this module is for protect the car when it's running out the track.
 *
 * 	@return:	none
 */
int16_t judgeOutType(void)
{
    static int16_t  s_outType = 0;
    static int16_t s_outTypeLast = 0;

    static int8_t s_leftNumLast = 2;
    static int8_t s_rightNumLast = 2;

    int8_t leftNum = gl_leftBlackNum;
    int8_t rightNum = gl_rightBlackNum;

    static uint8_t s_crosslineflag = 0;
    uint8_t judgeDisable = 0;
    static uint8_t s_cnt = 0;
    int16_t followAngleTemp = 0;

    static uint8_t s_crosslineBackTemp = 0;
    static uint8_t s_rampBack0 = 0;

    followAngleTemp = gl_FollowAngle;

    // out from left
    if(gl_crossline)              // if the car run out crossline , don't judge outtype in the following 5 times.
    {
        s_crosslineflag = 1;
        judgeDisable = 1;
        s_outType = 0;
    }

    if(s_crosslineflag)
    {
        if(!gl_crossline)
        {
            s_cnt ++;
            if(s_cnt > 2)
            {
                s_crosslineflag = 0;
                //s_cnt = 0;
            }
            judgeDisable = 1;
        }
    }

    if(gl_ramp)
    {
        s_rampBack0 = 1;
    }
    if(s_rampBack0)
    {
        s_rampBack0 = 0;
        s_leftNumLast = gl_leftBlackNum;
        s_rightNumLast = gl_rightBlackNum;
    }

    if(!judgeDisable)
    {
        if(s_cnt > 2)    // run out when car back from crossline.
        {
            if((followAngleTemp > 500) && (rightNum >= 8))
            {
                s_outType = OUT_LEFT_1;
            }
            s_cnt = 0;
        }

        if((s_leftNumLast == 9) && (s_rightNumLast < 0) && (!s_outType))
        {
            if((leftNum < 4) && (rightNum < 0))
            {
                s_outType = OUT_LEFT_1;
            }
            else if(rightNum > 7)
            {
                s_outType = OUT_LEFT_2;
            }
        }
        else if((s_leftNumLast >= 3) && (s_rightNumLast < 0) && (!s_outType))
        {
            if(rightNum > 7)
            {
                s_outType = OUT_LEFT_1;
            }
        }
        else if(s_outType > 0)
        {
            if((leftNum < 4) && (rightNum < 0))
            {
                s_outType = OUT_LEFT_1;
            }
            else if(rightNum > 7)
            {
                s_outType = OUT_LEFT_2;
            }
            else if(rightNum > 4)
            {
                s_outType = OUT_LEFT_3;
            }
            else
            {
                s_outType = OUT_LEFT_4;
            }

            if((leftNum > 6) && (rightNum < 0))
            {
                s_outType = 0;
            }
            else if((leftNum <= 6) && (rightNum >= 3))
            {
                s_outType = 0;
            }
        }
        // out from right

        if(s_cnt > 2)    // run out when car back from crossline.
        {
            if((followAngleTemp < -500) && (leftNum >= 8))
            {
                s_outType = OUT_RIGHT_1;
            }
            s_cnt = 0;
        }

        if((s_rightNumLast == 9) && (s_leftNumLast < 0) && (!s_outType))  // out from left
        {
            if((rightNum < 4) && (leftNum < 0))
            {
                s_outType = OUT_RIGHT_1;
            }
            else if(leftNum > 7)
            {
                s_outType = OUT_RIGHT_2;
            }
        }
        else if((s_rightNumLast >= 3) && (s_leftNumLast < 0) && (!s_outType))
        {
            if(leftNum > 7)
            {
                s_outType = OUT_RIGHT_1;
            }
        }
        else if(s_outType < 0)
        {
            if((rightNum < 4) && (leftNum < 0))
            {
                s_outType = OUT_RIGHT_1;
            }
            else if(leftNum > 7)
            {
                s_outType = OUT_RIGHT_2;
            }
            else if(leftNum > 4)
            {
                s_outType = OUT_RIGHT_3;
            }
            else
            {
                s_outType = OUT_RIGHT_4;
            }

            if((rightNum > 6) && (leftNum < 0))
            {
                s_outType = 0;
            }
            else if((rightNum <= 6) && (rightNum >= 3))
            {
                s_outType = 0;
            }
        }

        /*if(((s_outTypeLast - s_outType) > 1) || ((s_outType - s_outTypeLast) > 1))
        {
            s_outType = 0;
        }*/
        if(s_crosslineBack)
        {
            s_crosslineBackTemp = 1;
        }

        if((s_leftNumLast == 9) && (!s_outType) && s_crosslineBackTemp)
        {
            if((rightNum == 9) && (leftNum == 9))
            {
                gl_leftBlackNum = 9;
                gl_rightBlackNum = -1;
            }
        }
        else if((s_rightNumLast == 9) && (!s_outType) && s_crosslineBackTemp)
        {
            if((rightNum == 9) && (leftNum == 9))
            {
                gl_rightBlackNum = 9;
                gl_leftBlackNum = -1;
            }
        }
        else
        {
            s_crosslineBackTemp = 0;
        }
    }

    //gl_DataSendInt[5] = s_outType;

    s_leftNumLast = gl_leftBlackNum;
    s_rightNumLast = gl_rightBlackNum;

    //if(gl_startLineFilter)
    {
        s_outType = 0;
    }

    s_outTypeLast = s_outType;

    return s_outType;
}



/**
 * 	@brief:	 judgeCrossLine
 *
 * 	@note:	 judge if crossLine or not.
 *
 * 	@return:	none
 */
void judgeCrossline(void)
{
	int8_t leftNumOrigTemp = 2;
    int8_t  rightNumOrigTemp = 2;
    static uint8_t s_mybeCrossLine = 0;
    //static uint8_t s_cnt = 0;
   // static uint16_t s_crosslinDistance = 0;
   // static uint8_t s_firstTime = 1;
    //static uint s_crosslineCnt = 0;
    static uint8_t s_maybeCrosslineBackTimes = 0;

    leftNumOrigTemp  = gl_leftNumOrig;
    rightNumOrigTemp = gl_rightNumOrig;

    if((leftNumOrigTemp < 0) && (rightNumOrigTemp < 0))
    {
        s_mybeCrossLine ++;
    }
    else
    {
        s_mybeCrossLine = 0;
    }

    // crossline model.
    if(s_mybeCrossLine > CROSSLINE_BOUNDARY)
    {
        gl_crossline = 1;
    }

    /* if the gesture is not so good at begginning, gl_crossline may be set, but it's wrong. */
    if((gl_distanceTotal < 40) && (gl_crossline))
    {
        gl_crossline = 0;
    }

    // crossline model back to normal model.
    if(gl_crossline)
    {
        if((leftNumOrigTemp >= 0) || (rightNumOrigTemp >= 0))
        {
            s_maybeCrosslineBackTimes ++;
        }
        else
        {
            s_maybeCrosslineBackTimes = 0;
        }

        if((leftNumOrigTemp >= 0) || (rightNumOrigTemp >= 0))
        {
            if((leftNumOrigTemp < 3) && (rightNumOrigTemp < 3))
            {
                if(s_maybeCrosslineBackTimes >= 2)
                {
                    gl_crossline = 0;
                    s_crosslineBack = 1;
                }
            }
            else
            {
                if(s_maybeCrosslineBackTimes >= 1)
                {
                    gl_crossline = 0;
                    s_crosslineBack = 1;
                }
            }
        }
    }
    else
    {
        s_maybeCrosslineBackTimes = 0;
    }

    if(gl_crossline)
    {
         if((gl_outType == OUT_LEFT_1) || (gl_outType == OUT_RIGHT_1))
         {
             gl_crossline = 0;
         }
    }
}

/**
 * 	@brief:	 findLeftBlackNum
 *
 * 	@note:	 1.find the black dot,scan from right to left,the first black dot is the right one.
 *
 * 	@return:	signalValue
 */
int8_t findLeftBlackNum(void)
{
    int8_t blackNum = -1;
    int8_t i = 0;

    if(gl_leftLaserValueTemp != 0)
    {
        for(i=UPLEFT_LASER_NUM-1; i>=0; i--)
        {
            if(gl_leftLaserBit[i])
            {
                blackNum = i;
                break;
            }
        }
    }
    else
    {
        blackNum = -1;
    }

    return blackNum;
}


/**
 * 	@brief:	 findRightBlackNum
 *
 * 	@note:	 1.find the black dot,scan from right to left,the first black dot is the right one.
 *
 * 	@return:	signalValue
 */
int8_t findRightBlackNum(void)
{
    int8_t blackNum = -1;
    int8_t i = 0;

    if(gl_rightLaserValueTemp != 0)
    {
        for(i=UPRIGHT_LASER_NUM-1; i>=0; i--)
        {
            if(gl_rightLaserBit[i])
            {
                blackNum = i;
                break;
            }
        }
    }
    else
    {
        blackNum = -1;
    }

    return blackNum;
}

/**
 * 	@brief:	  getLeftValue
 *
 * 	@note:	 1.calculate signal value using variable blackNum.
 * 					 2.left laser high, while right low.
 * 					 3.if changeNum -1, then set signal value 0.
 * 	@return:	signalValue
 */
uint16_t getLeftValue(int8_t blackNum)
{

    int8_t  blackNumTemp = 0;
    uint16_t  signalValue = 0;

    blackNumTemp = blackNum;

    if(blackNumTemp >= 0)
    {
        signalValue = (0x0001 << (uint8_t)((LASER_SINGLE_NUM - 1) - blackNumTemp));
    }
    else  //all white
    {
        signalValue = 0;
    }

    return signalValue;

}

/**
 * 	@brief:	  getRightValue
 *
 * 	@note:	 1.calculate signal value using variable blackNum.
 * 					 2.left laser high, while right low.
 * 					 3.if changeNum -1, then set signal value 0.
 * 	@return:	signalValue
 */
uint16_t getRightValue(int8_t blackNum)
{
    int8_t blackNumTemp = 0;
    uint16_t signalValue = 0;

    blackNumTemp = blackNum;

    if(blackNumTemp >= 0)
    {
        signalValue = (0x0001 << (uint8_t)blackNumTemp);
    }
    else  //all white
    {
        signalValue = 0;
    }

    return signalValue;
}

