#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test_sd.h"
#include "light.h"
#include "switch.h"
#include "speaker.h"
#include "motor.h"
#include "steer.h"
#include "decoder.h"
#include "getLaserInfo.h"
#include "sd.h"


void task1(intptr_t exinf)
{
	int e = 0;

	sd_init(&Fatfs);                                     /* 初始化SD卡，并创建文件 */

	while(1)
	{
		SD_SendData(1);
		SD_SendData(2);
		SD_SendData(3);
		SD_SendData(4);
		SD_SendData(CRLF);

		for(e = 0; e< 65535; e++);
	}
}

void task2(intptr_t exinf)
{
    while(1)
    {
		wup_tsk(TASK1);
    }
}
