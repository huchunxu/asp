/*
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *//**        
 *        @file     motor.c
 *
 *        @brief    电机硬件抽象层函数
 *
 *        @version  0.1
 *        @date     2012/4/6  21:16
 *
 *        @author   Hu Chunxu , hcx196@gmail.com
 *//* ==================================================================================
 *  @0.1    Hu Chunxu   2012/4/6    create orignal file
 * =====================================================================================
 */
#include "motor.h"
#include "target_gpio.h"

/**
 * 文件内部宏定义
 */
#define   MOTOR_NUM    1       /**< 电机的数量 */

/**
 * 电机类型定义
 */
#define    MOTOR_DC    0x00    /**< 直流电机 */
#define    MOTOR_AC    0x80    /**< 交流电机 */

/**
 *  灯控制块
 */
typedef struct motor_control_block {
    ID        id;               /**< 电机的ID号 */
    uint8_t   type;             /**< 电机的类型 */
    int32_t   speed;            /**< 电机的转速 */    
    int32_t   voltage;          /**< 电机的电压值（占空比） */
    int32_t   current;          /**< 电机的电流值 */
    bool_t    openflag;         /**< 打开标志 */
    bool_t    errorflag;        /**< 错误标志 */
}MTCB;

static MTCB mtcb_table[MOTOR_NUM];

/**
 *  电机控制块和io口ID之间的转换
 */
#define  INDEX_MOTOR(motor_no)  ((uint_t)(motor_no))
#define  get_mtcb(motor_no)     (&(mtcb_table[INDEX_MOTOR(motor_no)]))


/** 
 * 电机的ID方法定义
 */
#define   MOTOR_NO_GET(motorid)    (motorid & 0x0000ffff)
#define   MOTOR_MOD_GET(motorid)   (FTM_MOD_GET((motorid & 0xffff0000) >> 16))
#define   MOTOR_CH_GET(motorid)    (FTM_CH_GET((motorid & 0xffff0000) >> 16))

static ER motor_open_port(ID motorid);

/**
 *    @brief   motor初始化
 */
void motor_init(intptr_t exinf)
{
    uint_t i;
    MTCB *p_mtcb;

    for (p_mtcb = mtcb_table, i = 0; i < MOTOR_NUM; p_mtcb++, i++)
    {
        p_mtcb->type = MOTOR_DC;
        p_mtcb->speed = 0;
        p_mtcb->voltage = 0;
        p_mtcb->current = 0;
        p_mtcb->openflag = false;
        p_mtcb->errorflag = E_SYS;
    }
    
    /* 打开电机端口 */
    motor_open_port(MOTOR0);
    
    gpio_init(PORT_NO_GET(MOTOR0_DIR_PIN), PIN_NO_GET(MOTOR0_DIR_PIN), 
              OUT_PUT, MOTOR0_FORWARD);
    
    motor_output(MOTOR0, 0);
}

/**
 *    @brief   打开motor端口
 *
 *    @param   motorid  电机的ID号
 *
 *    @return  ercd 错误码
 */
static ER motor_open_port(ID motorid)
{
    MTCB *p_mtcb;
    ER ercd;
    ID motor_no;

    motor_no = MOTOR_NO_GET(motorid);

    if (!((0 <= motor_no) && (motor_no < MOTOR_NUM)))
    {
        return (E_ID); /* ID号错误 */
    }
    p_mtcb = get_mtcb(motor_no);

    if (p_mtcb->openflag)
    { /* 检查是否已经打开 */
        ercd = E_OBJ;
    }
    else
    {
        /**
         *  硬件初始化
         */
        ftm_pwm_init(MOTOR_MOD_GET(motorid), MOTOR_CH_GET(motorid));

        p_mtcb->openflag = true;

        ercd = E_OK;
    }

    return (ercd);
}

/**
 *    @brief   motor输出
 *
 *    @param   motorid    电机的ID号
 *    @param   motor_duty 电机占空比
 *
 *    @return  ercd 错误码
 */
ER motor_output(ID motorid, int32_t motor_duty)
{
    MTCB *p_mtcb;
    ER ercd = 0;
    ID motor_no;
    int32_t motor_value;

    motor_no = MOTOR_NO_GET(motorid);

    if (!(0 <= motor_no && motor_no < MOTOR_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    
    /* 占空比范围保护 */
    if(motor_duty > MOTOR_MAX_VAL)
    {
        motor_duty = MOTOR_MAX_VAL;
    }
    else if (motor_duty < MOTOR_MIN_VAL)
    {
        motor_duty = MOTOR_MIN_VAL;
    }
    
    p_mtcb = get_mtcb(motor_no);

    if (p_mtcb->openflag)
    { /* 检查是否已经打开 */
        
        /* for "H" Bridge Drive,there is an "IO" Port to Control Motor Direction */
        if (motor_duty >= 0)
        {
            /*  If PWM is Positive Number,Motor Running Forward. */
            motor_output_dir_fw(motorid);
            //motor_duty = 100 - motor_duty;     /* 电磁组电机默认高点平，需要取反 */
        }
        else
        {
            /*  Else,Motor Running Backward.*/
            motor_output_dir_bw(motorid);
            motor_duty = -motor_duty;
            //motor_duty = 100 + motor_duty;  /* 电磁组电机默认高点平，需要取反 */
        }
        motor_value = motor_duty * MOTOR_PWM_PRESCALE;
        ftm_pwm_output(MOTOR_MOD_GET(motorid), MOTOR_CH_GET(motorid), motor_value);
    
        ercd = E_OK;
    }
    else
    {
        ercd = E_ILUSE;
    }

    return (ercd);
}

/**
 *    @brief   motor前进方向设置
 *
 *    @param   motorid    电机的ID号
 */
void motor_output_dir_fw(ID motorid)
{
    gpio_set(PORT_NO_GET(MOTOR0_DIR_PIN), PIN_NO_GET(MOTOR0_DIR_PIN), 
                 MOTOR0_FORWARD);
}

/**
 *    @brief   motor后退方向设置
 *
 *    @param   motorid    电机的ID号
 */
void motor_output_dir_bw(ID motorid)
{
    gpio_set(PORT_NO_GET(MOTOR0_DIR_PIN), PIN_NO_GET(MOTOR0_DIR_PIN), 
            MOTOR0_BACKWARD);
}
