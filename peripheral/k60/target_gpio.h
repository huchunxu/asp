/*
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *//**        
 *        @file     gpio.h
 *
 *        @brief    gpio header file
 *
 *        @version  0.1
 *        @date     2012/2/14 15:35:40
 *
 *        @author   Hu Chunxu, hcx196@gmail.com
 *//* ==================================================================================
 *  @0.1    Hu Chunxu   2012/2/14   create orignal file
 * =====================================================================================
 */

#ifndef _GPIO_H
#define _GPIO_H

/**
 * 头文件包含
 */
#include "MK60N512VMD100.h"   /* 寄存器映像头文件 */
#include "t_stddef.h"
#include "t_soudef.h"
	
/**
 * 宏定义
 */
typedef int_t          PIN;        /**< 引脚 */
typedef int_t          PORT;       /**< 端口 */

#define PORT_MAX_NUM 5

#define HIGH_POWER      1          /**< 高电平 */
#define LOW_POWER       0          /**< 低电平 */

#define OUT_PUT         1           /**< 输出 */
#define IN_PUT          0           /**< 输入 */

#define BSET(bit,Register)  ((Register)|= (1<<(bit)))    /**< 设置寄存器中某一位为1 */
#define BCLR(bit,Register)  ((Register) &= ~(1<<(bit)))  /**< 设置寄存器中某一位为0 */
#define BGET(bit,Register)  (((Register) >> (bit)) & 1)  /**< 得到寄存器中某一位状态 */

/**
 * 函数声明
 */
PORT_MemMapPtr gpio_get_port_addr (PORT port);
GPIO_MemMapPtr gpio_get_pt_addr(PORT port);
ER             gpio_init(PORT port, PIN pin, uint8_t dir, uint8_t state);
bool_t         gpio_get(PORT port, PIN pin);
void           gpio_set(PORT port, PIN pin, uint8_t state);
void           gpio_reverse(PORT port, PIN pin);

#endif
