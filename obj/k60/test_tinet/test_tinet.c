#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test_tinet.h"

void	task1(intptr_t exinf)
{
	while(1)
	{
        slp_tsk();
	}
}

void task2(intptr_t exinf)
{
    while(1)
    {
        wup_tsk(TASK1);
    }
}
