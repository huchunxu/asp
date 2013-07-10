/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2008-2009 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *	The above copyright holders grant permission gratis to use,
 *	duplicate, modify, or redistribute (hereafter called use) this
 *	software (including the one made by modifying this software),
 *	provided that the following four conditions (1) through (4) are
 *	satisfied.
 *
 *	(1) When this software is used in the form of source code, the above
 *    	copyright notice, this use conditions, and the disclaimer shown
 *    	below must be retained in the source code without modification.
 *
 *	(2) When this software is redistributed in the forms usable for the
 *    	development of other software, such as in library form, the above
 *    	copyright notice, this use conditions, and the disclaimer shown
 *    	below must be shown without modification in the document provided
 *    	with the redistributed software, such as the user manual.
 *
 *	(3) When this software is redistributed in the forms unusable for the
 *    	development of other software, such as the case when the software
 *    	is embedded in a piece of equipment, either of the following two
 *   	 conditions must be satisfied:
 *
 *  	(a) The above copyright notice, this use conditions, and the
 *         	disclaimer shown below must be shown without modification in
 *     		the document provided with the redistributed software, such as
 *      		the user manual.
 *
 * 		(b) How the software is to be redistributed must be reported to the
 *     		TOPPERS Project according to the procedure described
 *     		separately.
 *
 *	(4) The above copyright holders and the TOPPERS Project are exempt
 *    	from responsibility for any type of damage directly or indirectly
 *   	caused from the use of this software and are indemnified by any
 *    	users or end users of this software from any and all causes of
 *    	action whatsoever.
 *
 *	THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *	THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *	INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *	PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *	TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *	INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 */

/*
 *  serial io driver implementation (k60)
 */

#include <kernel.h>
#include <sil.h>
#include "target_serial.h"
#include "target_syssvc.h"
#include "target_interrupt.h"

/**
 * serial IO init block
 */
typedef struct sio_port_initialization_block
{
    UART_MemMapPtr uart_base; /**< 串口基地址    */
    uint_t intno;             /**< 串口中断异常号 */
}
SIOPINIB;

/**
 *  serial io control block
 */
struct sio_port_control_block
{
    const SIOPINIB  *p_siopinib;    /**< serial io init block */
    intptr_t        exinf;          /**< extern information */
    bool_t    getready;             /**< receive status */
    bool_t    putready;             /**< send status */
    bool_t    intialized;           /**< 是否已经初始化 */
};

/**
 * serial io initi table
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{UART0_BASE_PTR, INT_UART0_RX_TX
	},
    {UART4_BASE_PTR, INT_UART4_RX_TX
    }
};

/*
 *  serial control block table
 */
SIOPCB siopcb_table[TNUM_SIOP];
/**
 * macros for operations of serial control block table
 */
#define INDEX_SIOP(siopid)  ((uint_t)((siopid)))
#define get_siopcb(siopid)  (&(siopcb_table[INDEX_SIOP(siopid)]))
#define get_siopinib(siopid) (&(siopinib_table[INDEX_SIOP(siopid)]))



static UART_MemMapPtr uart_get_base_address(ID uart_no);

Inline bool_t  sio_putready(SIOPCB* p_siopcb)
{
    uint_t time = 0;

    /* 等待发送缓冲区空 */
    while(!(UART_S1_REG(p_siopcb->p_siopinib->uart_base) & UART_S1_TDRE_MASK))
    {
        time++;
        if(time > 0xFFBB)
        {
            /* 发送超时 */
            return (UART_S1_REG(p_siopcb->p_siopinib->uart_base) & UART_S1_TDRE_MASK);
        }
    }

    /* 发送成功 */
    return (UART_S1_REG(p_siopcb->p_siopinib->uart_base) & UART_S1_TDRE_MASK);
}

Inline bool_t sio_getready(SIOPCB* p_siopcb)
{
    uint_t time = 0;

    /* 等待发送缓冲区空 */
    while(!(UART_S1_REG(p_siopcb->p_siopinib->uart_base) & UART_S1_RDRF_MASK))
    {
        time++;
        if(time > 0xFFBB)
        {
            /* 发送超时 */
            return (UART_S1_REG(p_siopcb->p_siopinib->uart_base) & UART_S1_RDRF_MASK);
        }
    }

    /* 发送成功 */
    return (UART_S1_REG(p_siopcb->p_siopinib->uart_base) & UART_S1_RDRF_MASK);
}

/*
 *  uart init
 */
ER target_usart_init(ID uart_no, uint32_t sys_clk, uint32_t baud)
{
    register uint32_t sbr, brfa;
    uint8_t temp;

    /* 配置UART功能的GPIO接口 开启时钟 */
    switch(UART_ID_SET(uart_no))
    {
        case UART0:
            if(UART0_RX==PTA1)
                PORTA_PCR1 = PORT_PCR_MUX(0x2);      /* 在PTA1上使能UART0_RXD */
            else if(UART0_RX==PTA15)
                PORTA_PCR15 = PORT_PCR_MUX(0x3);     /* 在PTA15上使能UART0_RXD */
            else if(UART0_RX==PTB16)
                PORTB_PCR16 = PORT_PCR_MUX(0x3);     /* 在PTB16上使能UART0_RXD */
            else if(UART0_RX==PTD7)
                PORTD_PCR7 = PORT_PCR_MUX(0x3);      /* 在PTD7上使能UART0_RXD */
            else
                return E_OBJ;

            if(UART0_TX==PTA2)
                PORTA_PCR2 = PORT_PCR_MUX(0x2);      /* 在PTA2上使能UART0_RXD */
            else if(UART0_TX==PTA14)
                PORTA_PCR14 = PORT_PCR_MUX(0x3);     /* 在PTA14上使能UART0_RXD */
            else if(UART0_TX==PTB17)
                PORTB_PCR17 = PORT_PCR_MUX(0x3);     /* 在PTB17上使能UART0_RXD */
            else if(UART0_TX==PTD6)
                PORTD_PCR6 = PORT_PCR_MUX(0x3);      /* 在PTD6上使能UART0_RXD */
            else
                return E_OBJ;

            SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;       /* 使能 UARTn 时钟 */

            break;

        case UART1:
            if(UART1_RX==PTC3)
                PORTC_PCR3 = PORT_PCR_MUX(0x3);      /* 在PTC3上使能UART1_RXD */
            else if(UART1_RX==PTE1)
                PORTE_PCR1 = PORT_PCR_MUX(0x3);      /* 在PTE1上使能UART1_RXD */
            else
                return E_OBJ;

            if(UART1_TX==PTC4)
                PORTC_PCR4 = PORT_PCR_MUX(0x3);      /* 在PTC4上使能UART1_RXD */
            else if(UART1_TX==PTE0)
                PORTE_PCR0 = PORT_PCR_MUX(0x3);      /* 在PTE0上使能UART1_RXD */
            else
                return E_OBJ;

            SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

            break;

        case UART2:
            PORTD_PCR3 = PORT_PCR_MUX(0x3);         /* 在PTD3上使能UART2_TXD功能 */
            PORTD_PCR2 = PORT_PCR_MUX(0x3);         /* 在PTD2上使能UART2_RXD */
            SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;

            break;

        case UART3:
            if(UART3_RX==PTB10)
                PORTB_PCR10 = PORT_PCR_MUX(0x3);     /* 在PTB10上使能UART3_RXD */
            else if(UART3_RX==PTC16)
                PORTC_PCR16 = PORT_PCR_MUX(0x3);     /* 在PTC16上使能UART3_RXD */
            else if(UART3_RX==PTE5)
                PORTE_PCR5 = PORT_PCR_MUX(0x3);      /* 在PTE5上使能UART3_RXD */
            else
                return E_OBJ;

            if(UART3_TX==PTB11)
                PORTB_PCR11 = PORT_PCR_MUX(0x3);     /* 在PTB11上使能UART3_RXD */
            else if(UART3_TX==PTC17)
                PORTC_PCR17 = PORT_PCR_MUX(0x3);     /* 在PTC17上使能UART3_RXD */
            else if(UART3_TX==PTE4)
                PORTE_PCR4 = PORT_PCR_MUX(0x3);      /* 在PTE4上使能UART3_RXD */
            else
                return E_OBJ;

            SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;

            break;

        case UART4:
            if(UART4_RX==PTC14)
                PORTC_PCR14 = PORT_PCR_MUX(0x3);     /* 在PTC14上使能UART4_RXD */
            else if(UART4_RX==PTE25)
                PORTE_PCR25 = PORT_PCR_MUX(0x3);     /* 在PTE25上使能UART4_RXD */
            else
                return E_OBJ;

            if(UART4_TX==PTC15)
                PORTC_PCR15 = PORT_PCR_MUX(0x3);     /* 在PTC15上使能UART4_RXD */
            else if(UART4_TX==PTE24)
                PORTE_PCR24 = PORT_PCR_MUX(0x3);     /* 在PTE24上使能UART4_RXD */
            else
                return E_OBJ;

            SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;

            break;

        case UART5:
            if(UART5_RX==PTD8)
                PORTD_PCR8 = PORT_PCR_MUX(0x3);     /* 在PTD8上使能UART5_RXD */
            else if(UART5_RX==PTE9)
                PORTE_PCR9 = PORT_PCR_MUX(0x3);     /* 在PTE9上使能UART5_RXD */
            else
                return E_OBJ;

            if(UART5_TX==PTD9)
                PORTD_PCR9 = PORT_PCR_MUX(0x3);     /* 在PTD9上使能UART5_RXD */
            else if(UART5_TX==PTE8)
                PORTE_PCR8 = PORT_PCR_MUX(0x3);     /* 在PTE8上使能UART5_RXD */
            else
                return E_OBJ;

            SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

            break;
        default:
            return E_PAR;
            break;
    }

    /* 获取串口的基址 */
    UART_MemMapPtr uart_ch = uart_get_base_address(uart_no);

    /* 禁止发送接受 */
    UART_C2_REG(uart_ch) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );

    /*  配置成8位无校验模式
     ** 设置UART数据格式、校验方式和停止位位数。通过设置UART模块控制寄存器C1实现 */
    UART_C1_REG(uart_ch) = 0;

    /* 计算波特率，串口0、1使用内核时钟，其它串口使用外设时钟，系统时钟为外设时钟的2倍 */
    if ((uart_ch == UART0_BASE_PTR) | (uart_ch == UART1_BASE_PTR))
    {
        sys_clk = sys_clk * 2000;    /* 内核时钟 换算单位 --> khz */
    }
    else
    {
        sys_clk = sys_clk * 1000;    /* 内部总线时钟 换算单位 --> khz */
    }

    /* 设置 UART 数据通讯波特率。通过设置 UART 模块的波特率寄存器 */
    sbr = (uint16_t)((sys_clk)/(baud << 4));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(uart_ch) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(uart_ch) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(uart_ch) = (uint8_t)(sbr & UART_BDL_SBR_MASK);

    /* Determine if a fractional divider is needed to get closer to the baud rate
     **brfa = (((sys_clk*32000)/(baud * 16)) - (sbr * 32))  */
    brfa = (((sys_clk << 5)/(baud << 4)) - (sbr << 5));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(uart_ch) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(uart_ch) = temp |  UART_C4_BRFA(brfa);

    /*  设置是否允许接收和发送中断。通过设置 UART 模块的 C2 寄存器的
     ** RIE 和 TIE 位实现。如果使能中断，必须首先实现中断服务程序 */

    return E_OK;
}

/*
 *
 */
void target_usart_term(ID siopid)
{

}
/**
 * @brief get an char from input reg or buffer
 */
Inline uint8_t
uart_getchar(SIOPCB *p_siopcb)
{
    /* Return the 8-bit data from the receiver */
    return UART_D_REG(p_siopcb->p_siopinib->uart_base);
}


/**
 * @brief write an char into the send reg or buffer
 */
Inline void
uart_putchar(SIOPCB *p_siopcb, uint8_t c)
{
    /* 发送数据 */
    UART_D_REG(p_siopcb->p_siopinib->uart_base) = c;
}
/*
 *  SIO initialize
 */
void sio_initialize(intptr_t exinf)
{
    SIOPCB  *p_siopcb;
    uint_t  i;

    /*
     *  init the control block
     */
    for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
        p_siopcb->p_siopinib = &(siopinib_table[i]);
    }

    i = 3;
}

/*
 *  open serail IO
 */
SIOPCB *sio_opn_por(ID siopid, intptr_t exinf)
{
    const SIOPINIB  *p_siopinib;
    SIOPCB  *p_siopcb = get_siopcb(siopid);

    p_siopinib = p_siopcb->p_siopinib;
    p_siopcb->exinf = exinf;

    /* 第一次打开需要初始化 */
    if (!(p_siopcb->intialized)) {
    	target_usart_init(siopid, TARGET_UART_CLK, TARGET_UART_BAUD);
        p_siopcb->intialized = true;
    }

    /**
     *  允许串口发送和接收工作
     */
     /* 获取串口的基址 */
    UART_MemMapPtr uart_ch = uart_get_base_address(siopid);
    UART_C2_REG(uart_ch) |= (UART_C2_TE_MASK | UART_C2_RE_MASK);

    /* 开放UART接收中断 */
    UART_C2_REG(uart_ch)|=UART_C2_RIE_MASK;
    /* 开接收引脚的IRQ中断 */
    exc_enable(IRQ_VECTOR_USART4);

    /* something left to do */
    return(p_siopcb);
}

/*
 *  close serial io
 */
void sio_cls_por(ID portid, SIOPCB *p_siopcb)
{
    /*
     *  device dependent close operation
     */
    const SIOPINIB  *p_siopinib;

    p_siopinib = p_siopcb->p_siopinib;

    /**
     * 禁止串口接收发送工作
     */
     /* 获取串口的基址 */
    UART_MemMapPtr uart_ch = uart_get_base_address(portid);
    UART_C2_REG(uart_ch) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );

    /**
     * 禁止串口中断
     */
    /* 禁止UART接收中断 */
    UART_C2_REG(uart_ch)&=~UART_C2_RIE_MASK;
    /* 关接收引脚的IRQ中断 */

    exc_disable(IRQ_VECTOR_USART4);
}

/*
 *  serial interrupt io handler
 */
void sio_isr(intptr_t exinf)
{
    SIOPCB *p_siopcb = &(siopcb_table[1]);

    /* 关中断 */
    exc_disable(IRQ_VECTOR_USART4);

    sio_irdy_rcv(p_siopcb->exinf);

    /* 开中断 */
    exc_enable(IRQ_VECTOR_USART4);
}

/*
 *  output an char by serial io
 *  callback routine
 */
bool_t sio_snd_chr(SIOPCB *siopcb, char_t c)
{
    if (sio_putready(siopcb)){
        uart_putchar(siopcb, c);
        return(true);
    }
    return(false);
}

/*
 *  input an char by serial io
 *  callback routine
 */
int_t sio_rcv_chr(SIOPCB *siopcb)
{
    if (sio_getready(siopcb)) {
        return((int_t)(uint8_t)uart_getchar(siopcb));
    }
    return (-1);
}

/*
 *  enable serial io callback routine
 */
void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
    const SIOPINIB  *p_siopinib;

    p_siopinib = siopcb->p_siopinib;

    switch (cbrtn) {
        case SIO_RDY_SND:
            /* enable Tx interrupt */
            //sil_wrb_mem(p_siopinib->uart_base+UxIER,sil_reb_mem(p_siopinib->uart_base+UxIER) | 0x02);
            break;
        case SIO_RDY_RCV:
            //sil_wrb_mem(p_siopinib->uart_base+UxIER,sil_reb_mem(p_siopinib->uart_base+UxIER) | 0x01);
            /* enable Rx interrupt */
            break;
    }
}

/*
 *  disable callback routine
 */
void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
    const SIOPINIB  *p_siopinib;

    p_siopinib = siopcb->p_siopinib;

    switch (cbrtn) {
        case SIO_RDY_SND:
            /* disable Tx interrupt */
            //sil_wrb_mem(p_siopinib->uart_base+UxIER,sil_reb_mem(p_siopinib->uart_base+UxIER) & 0xfd);
            break;
        case SIO_RDY_RCV:
            //sil_wrb_mem(p_siopinib->uart_base+UxIER,sil_reb_mem(p_siopinib->uart_base+UxIER) & 0xfe);
            /* disable Rx interrupt */
            break;
    }
}

/*
 *  sio send char
 */
void sio_pol_snd_chr(char_t c, ID siopid)
{
	UART_MemMapPtr uart_ch = uart_get_base_address(siopid);

	/* Wait until space is available in the FIFO */
    while(!(UART_S1_REG(uart_ch) & UART_S1_TDRE_MASK));

    /* Send the character */
    UART_D_REG(uart_ch) = c;
}

/**
 * @brief 获取串口的基址
 *
 * @param uart_no 串口号
 *
 * @return 串口的基址值
 */
static UART_MemMapPtr
uart_get_base_address(ID uart_no)
{
    switch(UART_ID_SET(uart_no))
    {
        case UART0:
            return UART0_BASE_PTR;
            break;
        case UART1:
            return UART1_BASE_PTR;
            break;
        case UART2:
            return UART2_BASE_PTR;
            break;
        case UART3:
            return UART3_BASE_PTR;
            break;
        case UART4:
            return UART4_BASE_PTR;
            break;
        case UART5:
            return UART5_BASE_PTR;
            break;
        default :
            return NULL;
    }
}
