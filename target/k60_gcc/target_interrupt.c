/*
 * target_interrupt.c
 *
 *  Created on: 2012-8-30
 *      Author: hcx
 */

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

#include "MK60N512VMD100.h"
#include "target_interrupt.h"


/**
 * @brief enable exception
 *
 * @param excno exception no
 */
void exc_enable(uint32_t excno)
{
	uint32_t div;
	uint32_t bits;

	if (excno > 15)
	{
		/* 将中断向量号转换为中断请求号 */
		excno -= 16;

		div = excno / 32;
		bits = excno % 32;

		switch (div)
    	{
    		case 0x0:
              NVICICPR0 = 1 << bits;
              NVICISER0 = 1 << bits;
              break;
    		case 0x1:
              NVICICPR1 = 1 << bits;
              NVICISER1 = 1 << bits;
              break;
    		case 0x2:
              NVICICPR2 = 1 << bits;
              NVICISER2 = 1 << bits;
			case 0x3:
			  NVICICPR3 = 1 << bits;
              NVICISER3 = 1 << bits;
              break;
   		}
	}/* end if */
	else
	{
		switch (excno)
		{
			case INT_Reserved4:
				SCB_SHCSR |= SCB_SHCSR_MEMFAULTENA_MASK;
				break;
			case INT_Bus_Fault:
				SCB_SHCSR |= SCB_SHCSR_BUSFAULTENA_MASK;
				break;
			case INT_Usage_Fault:
				SCB_SHCSR |= SCB_SHCSR_USGFAULTENA_MASK;
				break;
		}
	}/* end else */

}


/**
 * @brief disable exception
 *
 * @param excno exception no
 */
void exc_disable(uint32_t excno)
{
	uint32_t div;
	uint32_t bits;

	if (excno > 15) {
	    /* 将中断向量号转换为中断请求号 */
	    excno -= 16;

		div = excno / 32;
		bits = excno % 32;

		switch (div)
    	{
    		case 0x0:
              NVICICER0 = 1 << bits;
              break;
    		case 0x1:
    		  NVICICER1 = 1 << bits;
              break;
    		case 0x2:
    		  NVICICER2 = 1 << bits;
			case 0x3:
			  NVICICER3 = 1 << bits;
              break;
    	}
	}
	else {
		switch (excno) {
			case INT_Reserved4:
				SCB_SHCSR &= ~SCB_SHCSR_MEMFAULTENA_MASK;
				break;
			case INT_Bus_Fault:
				SCB_SHCSR &= ~SCB_SHCSR_BUSFAULTENA_MASK;
				break;
			case INT_Usage_Fault:
				SCB_SHCSR &= ~SCB_SHCSR_USGFAULTENA_MASK;
				break;
		}
	}
}
