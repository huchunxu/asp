#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test_serial.h"

/*
 *  日志输出的服务调用错误
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

void	task1(intptr_t exinf)
{
	uint8_t get_char = 0;

	while(1)
	{
		serial_rea_dat(TASK_PORTID, &get_char,1);

		switch(get_char)
		{
			case 'a':
				serial_wri_dat(TASK_PORTID, "1", 1);
			break;
			case 's':
				serial_wri_dat(TASK_PORTID, "2", 1);
			break;
			case 'd':
				serial_wri_dat(TASK_PORTID, "3", 1);
			break;
			case 'f':
				serial_wri_dat(TASK_PORTID, "4", 1);
			break;
			default:
				break;
		}

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
