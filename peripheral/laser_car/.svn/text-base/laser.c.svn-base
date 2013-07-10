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

#include "laser.h"

/**
 * 文件内部宏定义
 */
#define   LASER_ADDRESS_NUM                       4            /**< 激光地址的数量 */
#define   UP_LASER_CONTROL_NUM            2            /**< 远处激光片选的数量 */
#define   DOWN_LASER_CONTROL_NUM         1           /**< 近处激光片选的数量 */
#define   UP_LASER_RECEIVE_NUM                6           /**< 远处激光接收管的数量 */
#define   DOWN_LASER_RECEIVE_NUM             3			  /**< 近处激光接收管的数量 */

/**
 *  激光地址控制块
 */
typedef struct laser_address_control_block {
    bool_t  state;
    bool_t  openflag;           /**< 打开标志 */
    bool_t  errorflag;          /**< 错误标志 */
}LSACB;

static LSACB lsacb_table[LASER_ADDRESS_NUM];

/**
 *  上排激光片选控制块
 */
typedef struct up_laser_control_control_block {
    bool_t  state;
    bool_t  openflag;           /**< 打开标志 */
    bool_t  errorflag;          /**< 错误标志 */
}LSUCCB;

static LSUCCB lsuccb_table[UP_LASER_CONTROL_NUM];

/**
 *  下排激光片选控制块
 */
typedef struct down_laser_control_control_block {
    bool_t  state;
    bool_t  openflag;           /**< 打开标志 */
    bool_t  errorflag;          /**< 错误标志 */
}LSDCCB;

static LSDCCB lsdccb_table[DOWN_LASER_CONTROL_NUM];

/**
 *  上排激光接收控制块
 */
typedef struct up_laser_receive_control_block {
    bool_t  state;
    bool_t  openflag;           /**< 打开标志 */
    bool_t  errorflag;          /**< 错误标志 */
}LSURCB;

static LSURCB lsurcb_table[UP_LASER_RECEIVE_NUM];

/**
 *  下排激光接收控制块
 */
typedef struct down_laser_receive_control_block {
    bool_t  state;
    bool_t  openflag;           /**< 打开标志 */
    bool_t  errorflag;          /**< 错误标志 */
}LSDRCB;

static LSDRCB lsdrcb_table[DOWN_LASER_RECEIVE_NUM];

/**
 *  激光地址控制块和io口ID之间的转换
 */
#define  INDEX_LASER_ADDRESS(laser_address_no)  ((uint_t)(laser_address_no))
#define  get_lsacb(laser_address_no)     (&(lsacb_table[INDEX_LASER_ADDRESS(laser_address_no)]))

/**
 *  上排激光片选控制块和io口ID之间的转换
 */
#define  INDEX_UP_LASER_CONTROL(up_laser_control_no)  ((uint_t)(up_laser_control_no))
#define  get_lsuccb(up_laser_control_no)     (&(lsuccb_table[INDEX_UP_LASER_CONTROL(up_laser_control_no)]))

/**
 *  下排激光片选控制块和io口ID之间的转换
 */
#define  INDEX_DOWN_LASER_CONTROL(down_laser_control_no)  ((uint_t)(down_laser_control_no))
#define  get_lsdccb(down_laser_control_no)     (&(lsdccb_table[INDEX_DOWN_LASER_CONTROL(down_laser_control_no)]))

/**
 *  上排激光接收控制块和io口ID之间的转换
 */
#define  INDEX_UP_LASER_RECEIVE(up_laser_receive_no)  ((uint_t)(up_laser_receive_no))
#define  get_lsurcb(up_laser_receive_no)     (&(lsurcb_table[INDEX_UP_LASER_RECEIVE(up_laser_receive_no)]))

/**
 *  下排激光接收控制块和io口ID之间的转换
 */
#define  INDEX_DOWN_LASER_RECEIVE(down_laser_receive_no)  ((uint_t)(down_laser_receive_no))
#define  get_lsdrcb(down_laser_receive_no)     (&(lsdrcb_table[INDEX_DOWN_LASER_RECEIVE(down_laser_receive_no)]))

/**
 * laser address的ID方法定义
 */
#define   LASER_ADDRESS_NO_GET(laser_address_id)    (laser_address_id & 0x0000ffff)
#define   LASER_ADDRESS_PORT_GET(laser_address_id)  (PORT_NO_GET(laser_address_id >> 16))
#define   LASER_ADDRESS_PIN_GET(laser_address_id)   (PIN_NO_GET(laser_address_id >> 16))

/**
 * up laser control的ID方法定义
 */
#define   UP_LASER_CONTROL_NO_GET(up_laser_control_id)    (up_laser_control_id & 0x0000ffff)
#define   UP_LASER_CONTROL_PORT_GET(up_laser_control_id)  (PORT_NO_GET(up_laser_control_id >> 16))
#define   UP_LASER_CONTROL_PIN_GET(up_laser_control_id)   (PIN_NO_GET(up_laser_control_id >> 16))

/**
 * down laser  control的ID方法定义
 */
#define   DOWN_LASER_CONTROL_NO_GET(down_laser_control_id)    (down_laser_control_id & 0x0000ffff)
#define   DOWN_LASER_CONTROL_PORT_GET(down_laser_control_id)  (PORT_NO_GET(down_laser_control_id >> 16))
#define   DOWN_LASER_CONTROL_PIN_GET(down_laser_control_id)   (PIN_NO_GET(down_laser_control_id >> 16))

/**
 * up laser  receive的ID方法定义
 */
#define   UP_LASER_RECEIVE_NO_GET(up_laser_receive_id)    (up_laser_receive_id & 0x0000ffff)
#define   UP_LASER_RECEIVE_PORT_GET(up_laser_receive_id)  (PORT_NO_GET(up_laser_receive_id >> 16))
#define   UP_LASER_RECEIVE_PIN_GET(up_laser_receive_id)   (PIN_NO_GET(up_laser_receive_id >> 16))

/**
 * down laser receive的ID方法定义
 */
#define   DOWN_LASER_RECEIVE_NO_GET(down_laser_receivel_id)    (down_laser_receive_id & 0x0000ffff)
#define   DOWN_LASER_RECEIVE_PORT_GET(down_laser_receive_id)  (PORT_NO_GET(down_laser_receive_id >> 16))
#define   DOWN_LASER_RECEIVE_PIN_GET(down_laser_receive_id)   (PIN_NO_GET(down_laser_receive_id >> 16))

static ER laser_address_open_port(ID laser_address_id);
static ER up_laser_control_open_port(ID up_laser_control_id);
static ER down_laser_control_open_port(ID down_laser_control_id);
static ER up_laser_receive_open_port(ID up_laser_receive_id);
static ER down_laser_receive_open_port(ID down_laser_receive_id);

/**
 *    @brief   laser address初始化
 */
void laser_address_init(void)
{
    uint_t i;
    LSACB *p_lsacb;

    for (p_lsacb = lsacb_table, i = 0; i < LASER_ADDRESS_NUM; p_lsacb++, i++)
    {
        p_lsacb->openflag = false;
        p_lsacb->errorflag = E_SYS;
    }

    /* 打开laser address IO口 */
    laser_address_open_port(LASER_ADDRESS0);
    laser_address_open_port(LASER_ADDRESS1);
    laser_address_open_port(LASER_ADDRESS2);
    laser_address_open_port(LASER_ADDRESS3);
}

/**
 *    @brief   up laser control初始化
 */
void up_laser_control_init(void)
{
    uint_t i;
    LSUCCB *p_lsuccb;

    for (p_lsuccb = lsuccb_table, i = 0; i < UP_LASER_CONTROL_NUM; p_lsuccb++, i++)
    {
        p_lsuccb->openflag = false;
        p_lsuccb->errorflag = E_SYS;
    }

    /* 打开laser far control IO口 */
    up_laser_control_open_port(UP_LASER_CONTROL_LEFT);
    up_laser_control_open_port(UP_LASER_CONTROL_RIGHT);
}

/**
 *    @brief   down laser control初始化
 */
void down_laser_control_init(void)
{
    uint_t i;
    LSDCCB *p_lsdccb;

    for (p_lsdccb = lsdccb_table, i = 0; i < DOWN_LASER_CONTROL_NUM; p_lsdccb++, i++)
    {
        p_lsdccb->openflag = false;
        p_lsdccb->errorflag = E_SYS;
    }

    /* 打开laser near control IO口 */
    down_laser_control_open_port(DOWN_LASER_CONTROL0);
}

/**
 *    @brief   up laser receive初始化
 */
void up_laser_receive_init(void)
{
    uint_t i;
    LSURCB *p_lsurcb;

    for (p_lsurcb = lsurcb_table, i = 0; i < UP_LASER_RECEIVE_NUM; p_lsurcb++, i++)
    {
        p_lsurcb->openflag = false;
        p_lsurcb->errorflag = E_SYS;
    }

    /* 打开laser far receive IO口 */
    up_laser_receive_open_port(UP_LASER_RECEIVE0);
    up_laser_receive_open_port(UP_LASER_RECEIVE1);
    up_laser_receive_open_port(UP_LASER_RECEIVE2);
    up_laser_receive_open_port(UP_LASER_RECEIVE3);
    up_laser_receive_open_port(UP_LASER_RECEIVE4);
    up_laser_receive_open_port(UP_LASER_RECEIVE5);
}

/**
 *    @brief   down laser receive初始化
 */
void down_laser_receive_init(void)
{
    uint_t i;
    LSDRCB *p_lsdrcb;

    for (p_lsdrcb = lsdrcb_table, i = 0; i < DOWN_LASER_RECEIVE_NUM; p_lsdrcb++, i++)
    {
        p_lsdrcb->openflag = false;
        p_lsdrcb->errorflag = E_SYS;
    }

    /* 打开laser near receive IO口 */
    down_laser_receive_open_port(DOWN_LASER_RECEIVE0);
    down_laser_receive_open_port(DOWN_LASER_RECEIVE1);
    down_laser_receive_open_port(DOWN_LASER_RECEIVE2);
}

/**
 *    @brief   激光发送初始化
 *
 *    @note   初始化地址和控制线
 */
void laser_send_init()
{
		laser_address_init();                         /*地址初始化，引脚初始状态为1111*/
		up_laser_control_init();					/*上排激光片选初始化，引脚初始状态为11*/
		down_laser_control_init();              /*下排激光片选初始化，引脚初始状态为1*/
}
/**
 *    @brief   打开laser address端口
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *    @return  ercd 错误码
 */
static ER laser_address_open_port(ID laser_address_id)
{
    LSACB *p_lsacb;
    ER ercd = 0;
    ID laser_address_no;

    laser_address_no = LASER_ADDRESS_NO_GET(laser_address_id);

    if (!(0 <= laser_address_no && laser_address_no < LASER_ADDRESS_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsacb = get_lsacb(laser_address_no);

    if (p_lsacb->openflag)
    { /* 检查是否已经打开 */
        ercd = E_OBJ;
    }
    else
    {
        /**
         *  硬件初始化
         */
        gpio_init(LASER_ADDRESS_PORT_GET(laser_address_id), LASER_ADDRESS_PIN_GET(laser_address_id), OUT_PUT,
                LASER_ADDRESS_OFF);

        p_lsacb->openflag = true;
        p_lsacb->state = LASER_ADDRESS_OFF;

        ercd = E_OK;
    }

    return (ercd);
}

/**
 *    @brief   打开laser far control端口
 *
 *    @param   up_laser_control_id  远处激光片选的ID号
 *
 *    @return  ercd 错误码
 */
static ER up_laser_control_open_port(ID up_laser_control_id)
{
    LSUCCB *p_lsdccb;
    ER ercd = 0;
    ID up_laser_control_no;

    up_laser_control_no = UP_LASER_CONTROL_NO_GET(up_laser_control_id);

    if (!(0 <= up_laser_control_no && up_laser_control_no < UP_LASER_CONTROL_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsdccb = get_lsuccb(up_laser_control_no);

    if (p_lsdccb->openflag)
    { /* 检查是否已经打开 */
        ercd = E_OBJ;
    }
    else
    {
        /**
         *  硬件初始化
         */
        gpio_init(UP_LASER_CONTROL_PORT_GET(up_laser_control_id), UP_LASER_CONTROL_PIN_GET(up_laser_control_id), OUT_PUT,
                UP_LASER_CONTROL_OFF);

        p_lsdccb->openflag = true;
        p_lsdccb->state = UP_LASER_CONTROL_OFF;

        ercd = E_OK;
    }

    return (ercd);
}

/**
 *    @brief   打开laser near control端口
 *
 *    @param   down_laser_control_id  近处激光片选的ID号
 *
 *    @return  ercd 错误码
 */
static ER down_laser_control_open_port(ID down_laser_control_id)
{
    LSDCCB *p_lsdccb;
    ER ercd = 0;
    ID down_laser_control_no;

    down_laser_control_no = DOWN_LASER_CONTROL_NO_GET(down_laser_control_id);

    if (!(0 <= down_laser_control_no && down_laser_control_no < DOWN_LASER_CONTROL_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsdccb = get_lsdccb(down_laser_control_no);

    if (p_lsdccb->openflag)
    { /* 检查是否已经打开 */
        ercd = E_OBJ;
    }
    else
    {
        /**
         *  硬件初始化
         */
        gpio_init(DOWN_LASER_CONTROL_PORT_GET(down_laser_control_id), DOWN_LASER_CONTROL_PIN_GET(down_laser_control_id), OUT_PUT,
                DOWN_LASER_CONTROL_OFF);

        p_lsdccb->openflag = true;
        p_lsdccb->state = DOWN_LASER_CONTROL_OFF;

        ercd = E_OK;
    }

    return (ercd);
}

/**
 *    @brief   打开laser far receive端口
 *
 *    @param   up_laser_receive_id  远处激光接收的ID号
 *
 *    @return  ercd 错误码
 */
static ER up_laser_receive_open_port(ID up_laser_receive_id)
{
    LSURCB *p_lsurcb;
    ER ercd = 0;
    ID up_laser_receive_no;

    up_laser_receive_no = UP_LASER_RECEIVE_NO_GET(up_laser_receive_id);

    if (!(0 <= up_laser_receive_no && up_laser_receive_no < UP_LASER_RECEIVE_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsurcb = get_lsurcb(up_laser_receive_no);

    if (p_lsurcb->openflag)
    { /* 检查是否已经打开 */
        ercd = E_OBJ;
    }
    else
    {
        /**
         *  硬件初始化
         */
        gpio_init(UP_LASER_RECEIVE_PORT_GET(up_laser_receive_id), UP_LASER_RECEIVE_PIN_GET(up_laser_receive_id), IN_PUT,
        		UP_LASER_RECEIVE_OFF);

        p_lsurcb->openflag = true;
        p_lsurcb->state = UP_LASER_RECEIVE_OFF;

        ercd = E_OK;
    }

    return (ercd);
}

/**
 *    @brief   打开down laser receive端口
 *
 *    @param   down_laser_receive_id  远处激光接收的ID号
 *
 *    @return  ercd 错误码
 */
static ER down_laser_receive_open_port(ID down_laser_receive_id)
{
    LSDRCB *p_lsdrcb;
    ER ercd = 0;
    ID down_laser_receive_no;

    down_laser_receive_no = DOWN_LASER_RECEIVE_NO_GET(down_laser_receive_id);

    if (!(0 <= down_laser_receive_no && down_laser_receive_no < DOWN_LASER_RECEIVE_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsdrcb = get_lsdrcb(down_laser_receive_no);

    if (p_lsdrcb->openflag)
    { /* 检查是否已经打开 */
        ercd = E_OBJ;
    }
    else
    {
        /**
         *  硬件初始化
         */
        gpio_init(DOWN_LASER_RECEIVE_PORT_GET(down_laser_receive_id), DOWN_LASER_RECEIVE_PIN_GET(down_laser_receive_id), IN_PUT,
                DOWN_LASER_RECEIVE_OFF);

        p_lsdrcb->openflag = true;
        p_lsdrcb->state = DOWN_LASER_RECEIVE_OFF;

        ercd = E_OK;
    }

    return (ercd);
}

/**
 *    @brief   laser激光地址选择函数
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   选择相应的地址
 */
ER laser_address_open(ID laser_address_id)
{
    LSACB *p_lsacb;
    ER ercd = 0;
    ID laser_address_no;

    laser_address_no = LASER_ADDRESS_NO_GET(laser_address_id);

    if (!(0 <= laser_address_no && laser_address_no < LASER_ADDRESS_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsacb = get_lsacb(laser_address_no);

    if (p_lsacb->openflag)
    { /* 检查是否已经打开 */
        gpio_set(LASER_ADDRESS_PORT_GET(laser_address_id), LASER_ADDRESS_PIN_GET(laser_address_id), LASER_ADDRESS_ON);
        p_lsacb->state = LASER_ADDRESS_ON;

        ercd = E_OK;
    }
    else
    {
        ercd = E_ILUSE;
    }

    return (ercd);
}

/**
 *    @brief   打开多个laser address
 *
 *    @param   laser_address_mask  多个激光地址的掩码
 */
void laser_address_open_some(uint8_t laser_address_mask)
{
    uint8_t laser_address_no = 0;
    uint8_t n = 0;

    for(n = 0;n < LASER_ADDRESS_NUM;n++)
    {
        if(laser_address_mask & 0x01)
        {
            switch(laser_address_no)
            {
                case 0:
                    laser_address_open(LASER_ADDRESS0);
                    break;
                case 1:
                    laser_address_open(LASER_ADDRESS1);
                    break;
                case 2:
                    laser_address_open(LASER_ADDRESS2);
                    break;
                case 3:
                    laser_address_open(LASER_ADDRESS3);
                    break;
                default:
                    break;
            }

        }
        laser_address_mask = laser_address_mask >> 1;
        laser_address_no ++;
    }
}

/**
 *    @brief   up laser片选选择函数
 *
 *    @param   up_laser_control_id  上排激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   选择相应的译码器
 */
ER up_laser_control_open(ID up_laser_control_id)
{
    LSUCCB *p_lsuccb;
    ER ercd = 0;
    ID up_laser_control_no;

    up_laser_control_no = UP_LASER_CONTROL_NO_GET(up_laser_control_id);

    if (!(0 <= up_laser_control_no && up_laser_control_no < UP_LASER_CONTROL_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsuccb = get_lsuccb(up_laser_control_no);

    if (p_lsuccb->openflag)
    { /* 检查是否已经打开 */
        gpio_set(UP_LASER_CONTROL_PORT_GET(up_laser_control_id), UP_LASER_CONTROL_PIN_GET(up_laser_control_id), UP_LASER_CONTROL_ON);
        p_lsuccb->state = UP_LASER_CONTROL_ON;

        ercd = E_OK;
    }
    else
    {
        ercd = E_ILUSE;
    }

    return (ercd);
}

/**
 *    @brief   down laser片选选择函数
 *
 *    @param   down_laser_control_id  下排激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   选择相应的译码器
 */
ER down_laser_control_open(ID down_laser_control_id)
{
    LSDCCB *p_lsdccb;
    ER ercd = 0;
    ID down_laser_control_no;

    down_laser_control_no = DOWN_LASER_CONTROL_NO_GET(down_laser_control_id);

    if (!(0 <= down_laser_control_no && down_laser_control_no < DOWN_LASER_CONTROL_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsdccb = get_lsdccb(down_laser_control_no);

    if (p_lsdccb->openflag)
    { /* 检查是否已经打开 */
        gpio_set(DOWN_LASER_CONTROL_PORT_GET(down_laser_control_id), DOWN_LASER_CONTROL_PIN_GET(down_laser_control_id), DOWN_LASER_CONTROL_ON);
        p_lsdccb->state = DOWN_LASER_CONTROL_ON;

        ercd = E_OK;
    }
    else
    {
        ercd = E_ILUSE;
    }

    return (ercd);
}

/**
 *    @brief   laser激光地址选择函数
 *
 *    @param   laser_address_id  激光地址的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   关闭相应的地址
 */
ER laser_address_close(ID laser_address_id)
{
    LSACB *p_lsacb;
    ER ercd = 0;
    ID laser_address_no;

    laser_address_no = LASER_ADDRESS_NO_GET(laser_address_id);

    if (!(0 <= laser_address_no && laser_address_no < LASER_ADDRESS_NUM))
    {
        return (E_ID); /* ID号错误 */
    }
    p_lsacb = get_lsacb(laser_address_no);

    if (p_lsacb->openflag)
    { /* 检查是否已经打开 */
        gpio_set(LASER_ADDRESS_PORT_GET(laser_address_id), LASER_ADDRESS_PIN_GET(laser_address_id), LASER_ADDRESS_OFF);
        p_lsacb->state = LASER_ADDRESS_OFF;

        ercd = E_OK;
    }
    else
    {
        ercd = E_ILUSE;
    }

    return (ercd);
}

/**
 *    @brief   up  laser片选关闭函数
 *
 *    @param   up_laser_control_id  上排激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   关闭相应的译码器
 */
ER  up_laser_control_close(ID up_laser_control_id)
{
	    LSUCCB *p_lsdccb;
	    ER ercd = 0;
	    ID up_laser_control_no;

	    up_laser_control_no = UP_LASER_CONTROL_NO_GET(up_laser_control_id);

	    if (!(0 <= up_laser_control_no && up_laser_control_no < UP_LASER_CONTROL_NUM))
	    {
	        return (E_ID); /* ID号错误 */
	    }
	    p_lsdccb = get_lsuccb(up_laser_control_no);

	    if (p_lsdccb->openflag)
	    { /* 检查是否已经打开 */
	        gpio_set(UP_LASER_CONTROL_PORT_GET(up_laser_control_id), UP_LASER_CONTROL_PIN_GET(up_laser_control_id), UP_LASER_CONTROL_OFF);
	        p_lsdccb->state = UP_LASER_CONTROL_OFF;

	        ercd = E_OK;
	    }
	    else
	    {
	        ercd = E_ILUSE;
	    }

	    return (ercd);
}

/**
 *    @brief   up laser片选关闭函数
 *
 *    @note   关闭2个译码器
 */
void up_laser_control_close_all(void)
{
		up_laser_control_close(UP_LASER_CONTROL_LEFT);
		up_laser_control_close(UP_LASER_CONTROL_RIGHT);
}
/**
 *    @brief   down laser片选关闭函数
 *
 *    @param   down_laser_control_id  远处激光片选的ID号
 *
 *    @return  ercd 错误码
 *
 *    @note   关闭相应的译码器
 */
ER  down_laser_control_close(ID down_laser_control_id)
{
	    LSDCCB *p_lsdccb;
	    ER ercd = 0;
	    ID down_laser_control_no;

	    down_laser_control_no = DOWN_LASER_CONTROL_NO_GET(down_laser_control_id);

	    if (!(0 <= down_laser_control_no && down_laser_control_no < DOWN_LASER_CONTROL_NUM))
	    {
	        return (E_ID); /* ID号错误 */
	    }
	    p_lsdccb = get_lsdccb(down_laser_control_no);

	    if (p_lsdccb->openflag)
	    { /* 检查是否已经打开 */
	        gpio_set(DOWN_LASER_CONTROL_PORT_GET(down_laser_control_id), DOWN_LASER_CONTROL_PIN_GET(down_laser_control_id), DOWN_LASER_CONTROL_OFF);
	        p_lsdccb->state = DOWN_LASER_CONTROL_OFF;

	        ercd = E_OK;
	    }
	    else
	    {
	        ercd = E_ILUSE;
	    }

	    return (ercd);
}

/**
 *   	@brief   接收某一接收管信号
 *
 *  	@param up_laser_receive_id
 *
 *  	@return up_laser_receive_temp
 *
 *  	@note 1表示这一接收管收到信号，0表示没有接收到信号
 */
uint16_t up_laser_receive(ID up_laser_receive_id)
{
	    uint16_t up_laser_receive_temp;
	    up_laser_receive_temp=0;

	    up_laser_receive_temp=gpio_get(UP_LASER_RECEIVE_PORT_GET(up_laser_receive_id), UP_LASER_RECEIVE_PIN_GET(up_laser_receive_id));
	    return up_laser_receive_temp;
}

/**
 *   	@brief   下排接收某一接收管信号
 *
 *  	@param down_laser_receive_id
 *
 *  	@return down_laser_receive_temp
 *
 *  	@note 1表示这一接收管收到信号，0表示没有接收到信号
 */
uint16_t down_laser_receive(ID down_laser_receive_id)
{
	    uint16_t down_laser_receive_temp;
	    down_laser_receive_temp=0;

	    down_laser_receive_temp=gpio_get(DOWN_LASER_RECEIVE_PORT_GET(down_laser_receive_id), DOWN_LASER_RECEIVE_PIN_GET(down_laser_receive_id));
	    return down_laser_receive_temp;
}






