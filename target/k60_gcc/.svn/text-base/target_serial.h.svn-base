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
 *  SIO driver(k60)
 */

#ifndef TOPPERS_TARGET_SERIAL_H
#define TOPPERS_TARGET_SERIAL_H

#include "MK60N512VMD100.h"
#include "t_soudef.h"

#define IRQ_VECTOR_USART4		INT_UART4_RX_TX

/*
 *  the sio interrupt used by system log
 */
#if (SIO_PORTID == 1)
#define INHNO_SIO		IRQ_VECTOR_USART4
#define INTNO_SIO		IRQ_VECTOR_USART4
#elif (SIO_PORID == 2)
#define INHNO_SIO		IRQ_VECTOR_USART2
#define INTNO_SIO		IRQ_VECTOR_USART2
#endif
#define INTPRI_SIO		-3				/* interrupt priority */
#define INTATR_SIO		0				/* interrupt attribute */

/*
 *  callback routine no
 */
#define SIO_RDY_SND    1U        /* ready to send no */
#define SIO_RDY_RCV    2U        /* ready to receive no */

/**     模块通道      端口          可选范围                             建议               */
#define UART0_RX    PTD6     /**< PTA1、PTA15、PTB16、PTD6          PTA1不要用（与Jtag冲突）*/
#define UART0_TX    PTD7     /**< PTA2、PTA14、PTB17、PTD7          PTA2不要用（与Jtag冲突）*/

#define UART1_RX    PTE1     /**< PTC3、PTE1 */
#define UART1_TX    PTE0     /**< PTC4、PTE0 */

#define UART2_RX    PTD2     /**< PTD2  */
#define UART2_TX    PTD3     /**< PTD3  */

#define UART3_RX    PTC16    /**< PTB10、PTC16、PTE5 */
#define UART3_TX    PTC17    /**< PTB11、PTC17、PTE4 */

#define UART4_RX    PTE25    /**< PTC14、PTE25 */
#define UART4_TX    PTE24    /**< PTC15、PTE24 */

#define UART5_RX    PTE9     /**< PTD8、PTE9 */
#define UART5_TX    PTE8     /**< PTD9、PTE8 */


#define  UART_BAUD_9600      9600
#define  UART_BAUD_19200     19200
#define  UART_BAUD_57600     57600
#define  UART_BAUD_115200    115200

#define  TARGET_UART_BAUD     UART_BAUD_57600
#define  TARGET_UART_CLK      50000


#ifndef TOPPERS_MACRO_ONLY

/**
 *  sio control block definition
 */
typedef struct sio_port_control_block   SIOPCB;

/*
 *   SIO initialize
 */
extern void sio_initialize(intptr_t exinf);

/*
 *   SIO open
 */
extern SIOPCB *sio_opn_por(ID siopid, intptr_t exinf);

/*
 *  SIO close
 */
extern void sio_cls_por(ID portid, SIOPCB *p_siopcb);

/*
 *  sio interrupt handler
 */
extern void sio_isr(intptr_t exinf);

/*
 *  sio sendchar
 */
extern bool_t sio_snd_chr(SIOPCB *siopcb, char_t c);

/*
 *  sio receive char
 */
extern int_t sio_rcv_chr(SIOPCB *siopcb);

/*
 *  enable call back
 */
extern void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn);

/* 
 *  disable call back routine
 */
extern void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  sio interrupt send
 */
extern void sio_irdy_snd(intptr_t exinf);

/*
 *  sio interrupt receive
 */
extern void sio_irdy_rcv(intptr_t exinf);

/*
 *  sio send char
 */
extern void sio_pol_snd_chr(char_t c, ID siopid);

/*
 *  target initialize
 */
extern ER target_usart_init(ID uart_no, uint32_t sys_clk, uint32_t baud);


#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_SERIAL_H */
