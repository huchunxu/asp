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
#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H


/*
 * the definition of k60's resources
 */
#include <sil.h>
#include "arm_m_gcc/arm_m.h"

/*
 *  the definition of clock
 */
#define HSE_CLOCK		(8000000)
#define SYS_CLOCK		((HSE_CLOCK >> 1) * 25)
#define PCLK1_CLOCK		((SYS_CLOCK) >> 1)
#define PCLK2_CLOCK		(SYS_CLOCK)

/*
 *  interrupt handler no. related definition
 */
#define TMAX_INTNO   (16 + 239)   //important for flash protect

/*
 *
 */
#define TBITW_IPRI     4

/*
 *
 */
#define TBITW_SUBIPRI  0

/*
 *
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  ���������������������������
 *  8byte���������
 */
#define DEFAULT_ISTKSZ			(0x1000/8U)		/* 4KByte */

#define PLL_48MHZ   0
#define PLL_50MHZ   1
#define PLL_96MHZ   2
#define PLL_100MHZ  3

#define CORE_CLK_48000    48000    /**< 单位 ：khz */
#define CORE_CLK_50000    50000
#define CORE_CLK_96000    96000
#define CORE_CLK_100000   100000

#define BUS_CLK_48000     48000    /**< 单位 ：khz */
#define BUS_CLK_50000     50000

#define XTAL_2MHZ   0
#define XTAL_4MHZ   1
#define XTAL_6MHZ   2
#define XTAL_8MHZ   3
#define XTAL_10MHZ  4
#define XTAL_12MHZ  5
#define XTAL_14MHZ  6
#define XTAL_16MHZ  7
#define XTAL_18MHZ  8
#define XTAL_20MHZ  9
#define XTAL_22MHZ  10
#define XTAL_24MHZ  11
#define XTAL_26MHZ  12
#define XTAL_28MHZ  13
#define XTAL_30MHZ  14
#define XTAL_32MHZ  15

/*
 *  parameters for SIL_DLY
 */
#define SIL_DLY_TIM1    162
#define SIL_DLY_TIM2    100

/*
 *  sio port ID
 */
#define SIO_PORTID		(1)

/*
 *
 */
#define TOPPERS_CUSTOM_IDEL
#define toppers_asm_custom_idle		\
	msr		basepri, r0;			\
	msr		basepri, r1;


#ifndef TOPPERS_MACRO_ONLY

/*
 *  target initialize
 */
extern void	target_initialize(void);

/*
 * target exit
 */
extern void	target_exit(void) NoReturn;

extern void hardware_init_hook(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  processor dependent(ARM)
 */
#include "arm_m_gcc/prc_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */
