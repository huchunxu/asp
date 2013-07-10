#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test_mailbox.h"

typedef struct str_msg{
	T_MSG msg_head;
	int a;
	char b;
	int c[2];
}STR_MSG;

void	task1(intptr_t exinf)
{
	STR_MSG  msg1;

	msg1.a = 31;
	msg1.b = 32;
	msg1.c[0] = 33;
	msg1.c[1] = 34;

	while(1)
	{
		snd_mbx(MBX1, (T_MSG *)(&msg1));

        slp_tsk();
	}
}

void task2(intptr_t exinf)
{
	STR_MSG * p_msg;

    while(1)
    {
    	rcv_mbx(MBX1, (T_MSG **)&p_msg);

    	//while(1);

        //wup_tsk(TASK1);
    }
}
