/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     GetSignalValues.c
 *
 *        @brief    获取上排激光信号
 *
 *        @version  0.1
 *        @date     2012-5-26  19：00
 *
 *        @author:  Cao Shen , caoshen.1992@gmail.com
 * =====================================================================================
 *  @0.1    Cao Shen   2012-5-26    create orignal file
 * =====================================================================================
 */
#include "getLaserInfo.h"
#include "GetFarSignalValue.h"

/**
 *  Variables Declearation
 */
uint32_t  gl_leftLaserValueTemp = 0;
uint32_t  gl_rightLaserValueTemp = 0;

/**
 *    @brief   getLeftLaserInfo
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *		@note:get left laser infomation.
 *
 */
void getLeftLaserInfo(void)
{
     uint8_t i = 0;

     gl_leftLaserValueTemp = 0;

     // open 4-16 decoder1, then left lasers can be chosen.
     FarSignalControlReg(0,0);
     delay_10us(5);

     for(i=0; i<UPLEFT_LASER_NUM; i++)
     {
         leftLaserSend(i);
         rightLaserSend(i);

         delay_10us(LASER_DELAY_TIME);

         getLeftLaserValue(i);
         getRightLaserValue(i);
     }
}

/**
 *    @brief   getRightLaserInfo
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *		@note:get right laser infomation.
 *
 *		@return:       none
 */
void getRightLaserInfo(void)
{
     uint8_t i = 0;

     gl_rightLaserValueTemp = 0;

     FarSignalControlReg(0,1); 	/*open 4-16 decoder2, then right laser can be chosen.*/
     delay_10us(2);

     for(i=0; i<UPRIGHT_LASER_NUM; i++)
     {
         rightLaserSend(i);
         delay_10us(LASER_DELAY_TIME);
         getRightLaserValue(i);
     }
}

/**
 *    @brief   getLaserInfo
 *
 *		@note:get laser infomation.
 *
 *		@return:       none
 */
void getLaserInfo(void)
{
    uint8_t  i = 0;
    uint8_t   j = 0;
   // uint8_t   number = 0;

    gl_leftLaserValueTemp = 0;
    gl_rightLaserValueTemp = 0;
    gl_NearSignalValue = 0;

    // open right 4-16 decoder
    FarSignalControlReg(0,1);
    delay_10us(5);

    for(i=0; i<10; i++)
    {
        j = i + 14;
        rightLaserSend(j);
        //delay(40000);
       // delay(LASER_DELAY_TIME);
        delay_10us(5);
        getRightLaserValue(i);
    }

    closeFarSignal();
    delay_10us(2);

    // open left 4-16 decoder
    FarSignalControlReg(1,0);
    delay_10us(5);

    for(i=2; i<12; i++)
    {
        leftLaserSend(i);
        //delay(40000);
        delay_10us(LASER_DELAY_TIME);
        getLeftLaserValue(i);
    }

    //if(gl_ramp || ReadStartSwitch())
/*    if(gl_ramp)
    {
       NearSignalControlReg(0);
       delay(5);

       for(i=0; i<LASER_NEAR_NUM; i++)
       {
           FarSignalSend(i);    // use the same decoder address with far signal.
           delay(LASER_DELAY_TIME);
           GetNearSignalValue(i);
       }
       CloseNearSignal();
    }*/

    closeFarSignal();
}

/**
 *    @brief   简单的延时函数
 *
 *    @param   ntime  延时周期10us
 *
 *    @note:system clock为100MHz，那么一个机器周期为12/100us=0.12us，84*0.12=10.08us
 */
void delay_10us(uint32_t ntime)
{
    uint32_t i = 0, j = 0;

    for(i=0;i<ntime;i++)
        for(j=0;j<84;j++);
}

/**
 * @brief:delay 1us by PIT_CVAL0
 *
 * @note:PIT_CVAL0为一个递减计数器，每递减一次时间为0.02us。
 *
 * @return:none
 */
void delay_us(uint32_t us_1)
{
    uint32_t TCNTtemp0 =  PIT_CVAL0;
    uint32_t TCNTtemp1 = PIT_CVAL0;
    uint32_t timeTemp = us_1 * 50;

    while((TCNTtemp0- TCNTtemp1) < timeTemp)
    {
        TCNTtemp1 = PIT_CVAL0;
    }
}
