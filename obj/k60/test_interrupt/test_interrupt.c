#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test_interrupt.h"
#include "light.h"

void task1(intptr_t exinf)
{
	volatile static int flag ;
	while(1)
	{
		flag++;
		light_change(LIGHT0);
		slp_tsk();
		//tslp_tsk(1);

	}
}

void task2(intptr_t exinf)
{
	volatile int i ;
    while(1)
    {
    	i++;
        wup_tsk(TASK1);
    }
}
