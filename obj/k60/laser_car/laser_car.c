#include <stdio.h>
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "laser_car.h"
#include "light.h"
#include "switch.h"
#include "speaker.h"
#include "motor.h"
#include "steer.h"
#include "decoder.h"
#include "getLaserInfo.h"
#include "FarSignalProcess.h"
#include "steer_control.h"
#include "Follow.h"

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

void main_task(intptr_t exinf)
{
	//串口任务初始化
	ER_UINT	ercd;
	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_NOTICE), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	//初始化其它的任务
	SVC_PERROR(act_tsk(SPEED_TASK));
	SVC_PERROR(act_tsk(FOLLOW_TASK));
	SVC_PERROR(act_tsk(SENSORS_TASK));
	SVC_PERROR(sta_cyc(CYC_HANDLE));
	//结束自己的使命
	ext_tsk();
}

void speed_task(intptr_t exinf)
{
	while(1)
	{
		wai_sem(SPEED_SEM);

		//输出ＰＷＭ
		motor_output(MOTOR0, 15);

		syslog(LOG_NOTICE, "speed_task run!");
	}
}

void follow_task(intptr_t exinf)
{
	while(1)
	{
		wai_sem(FOLLOW_SEM);

		//对传感器进行处理
    	farSignalProcess();

    	//摇头舵机控制
    	setFollowAngle();

    	//转向舵机控制
    	setSteerAngle();

    	//输出ｐｗｍ
    	outputFollowPWM();
    	outputSteerPWM();

		syslog(LOG_NOTICE, "follow_task run!");
	}
}

void sensors_task(intptr_t exinf)
{
	while(1)
	{
		wai_sem(SENSORS_SEM);

		//更新传感器信息
		getLaserInfo();
		syslog(LOG_NOTICE, "sensors_task run!");
		//释放信号量
		sig_sem(FOLLOW_SEM);
	}
}

void cyc_handle(intptr_t exinf)
{
	static unsigned int count_ms = 0;
	count_ms ++;

	//每ms speed_task都需要运行
	isig_sem(SPEED_SEM);

	//每4ms  sensors_task都需要运行
	if (!(count_ms % 4))
		isig_sem(SENSORS_SEM);
}

