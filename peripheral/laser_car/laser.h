/**
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *
 *        @file     laser.c
 *
 *        @brief    编码器硬件抽象层函数
 *
 *        @version  0.1
 *        @date     2012/5/24  19：00
 *
 *        @author:  Cao Shen , caoshen.1992@gmail.com
 * =====================================================================================
 *  @0.1    Cao Shen   2012/5/24    create orignal file
 * =====================================================================================
 */

#ifndef LASER_H_
#define LASER_H_

#include "target_gpio.h"
#include "t_soudef.h"

/**
 * laser address define
 */
#define  LASER_ADDRESS0 		(((PTD9<<16) | 0x0000))
#define  LASER_ADDRESS1 		(((PTD10<<16) | 0x0001))
#define  LASER_ADDRESS2 		(((PTD11<<16) | 0x0002))
#define  LASER_ADDRESS3 		(((PTD12<<16) | 0x0003))

/**
 * laser_address的状态宏定义
 */
#define LASER_ADDRESS_ON        HIGH_POWER
#define LASER_ADDRESS_OFF       LOW_POWER

/**
 * up_laser_control的状态宏定义
 */
#define UP_LASER_CONTROL_ON        LOW_POWER     /*74hc154片选低有效*/
#define UP_LASER_CONTROL_OFF       HIGH_POWER

/**
 * laser_near_control的状态宏定义
 */
#define DOWN_LASER_CONTROL_ON        LOW_POWER
#define DOWN_LASER_CONTROL_OFF       HIGH_POWER

/**
 * laser_far_receive的状态宏定义
 */
#define UP_LASER_RECEIVE_ON        HIGH_POWER
#define UP_LASER_RECEIVE_OFF       LOW_POWER

/**
 * laser_near_receive的状态宏定义
 */
#define DOWN_LASER_RECEIVE_ON        HIGH_POWER
#define DOWN_LASER_RECEIVE_OFF       LOW_POWER

/**
 * laser far control define
 */
#define UP_LASER_CONTROL_LEFT 		(((PTD8<<16) | 0x0000))
#define UP_LASER_CONTROL_RIGHT 		(((PTD7<<16) | 0x0001))

/**
 * laser near control define
 */
#define DOWN_LASER_CONTROL0			(((PTC8<<16) | 0x0000))

/**
 * laser far receive define
 */
#define UP_LASER_RECEIVE0			(((PTC19<<16) | 0x0000))
#define UP_LASER_RECEIVE1			(((PTD6<<16) | 0x0001))
#define UP_LASER_RECEIVE2			(((PTD1<<16) | 0x0002))
#define UP_LASER_RECEIVE3			(((PTD3<<16) | 0x0003))
#define UP_LASER_RECEIVE4			(((PTD0<<16) | 0x0004))
#define UP_LASER_RECEIVE5			(((PTD2<<16) | 0x0005))

/**
 * laser near receive define
 */
#define DOWN_LASER_RECEIVE0			(((PTC13<<16) | 0x0000))
#define DOWN_LASER_RECEIVE1			(((PTC10<<16) | 0x0001))
#define DOWN_LASER_RECEIVE2			(((PTC11<<16) | 0x0002))

/**
 *    @brief   laser address初始化
 */
void laser_address_init(void);

/**
 *    @brief   up laser control初始化
 */
void up_laser_control_init(void);

/**
 *    @brief   down laser control初始化
 */
void down_laser_control_init(void);

/**
 *    @brief   laser far receive初始化
 */
void up_laser_receive_init(void);

/**
 *    @brief   laser near receive初始化
 */
void down_laser_receive_init(void);

/**
 *    @brief   激光发送初始化
 *
 *    @note   初始化地址和控制线
 */
void laser_send_init(void);

/**
 *    @brief   laser激光地址选择函数
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   选择相应的地址
 */
ER laser_address_open(ID laser_address_id);

/**
 *    @brief   打开多个laser address
 *
 *    @param   laser_address_mask  多个激光地址的掩码
 */
void laser_address_open_some(uint8_t laser_address_mask);

/**
 *    @brief   far laser片选选择函数
 *
 *    @param   laser_far_control_id  远处激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   选择相应的译码器
 */
ER up_laser_control_open(ID laser_far_control_id);

/**
 *    @brief   near laser片选选择函数
 *
 *    @param   laser_near_control_id  远处激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   选择相应的译码器
 */
ER down_laser_control_open(ID laser_near_control_id);

/**
 *    @brief   laser激光地址选择函数
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   关闭相应的地址
 */
ER laser_address_close(ID laser_address_id);

/**
 *    @brief   far laser片选关闭函数
 *
 *    @param   laser_far_control_id  远处激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   关闭相应的译码器
 */
ER  up_laser_control_close(ID laser_far_control_id);

/**
 *    @brief   far laser片选关闭函数
 *
 *    @note   关闭2个译码器
 */
void up_laser_control_close_all(void);

/**
 *    @brief   near laser片选关闭函数
 *
 *    @param   laser_near_control_id  远处激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   关闭相应的译码器
 */
ER  down_laser_control_close(ID laser_near_control_id);

/**
 *   	@brief   接收某一接收管信号
 *
 *  	@param laser_far_receive_id
 *
 *  	@return laser_far_receive_temp
 *
 *  	@note 1表示这一接收管收到信号，0表示没有接收到信号
 */
uint16_t up_laser_receive(ID laser_far_receive_id);

/**
 *   	@brief   下排接收某一接收管信号
 *
 *  	@param down_laser_receive_id
 *
 *  	@return down_laser_receive_temp
 *
 *  	@note 1表示这一接收管收到信号，0表示没有接收到信号
 */
uint16_t down_laser_receive(ID down_laser_receive_id);

#endif /* LASER_H_ */
