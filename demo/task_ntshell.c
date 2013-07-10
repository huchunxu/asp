/*
 * =====================================================================================
 *
 *       Filename:  task_ntshell.c
 *
 *    Description:  task for ntshell
 *
 *        Version:  0.1
 *        Created:  2011/11/14 16:11:49
 *
 *         Author:  Ren Wei , renweihust@gmail.com
 *        Company:  HUST-Renesas Lab
 *       Revision:
 * =====================================================================================
 *	@0.1 	Ren Wei	2011/11/14	create orignal file
 * =====================================================================================
 */

#include <kernel.h>
#include <t_syslog.h>
#include "target_syssvc.h"
#include <syssvc/serial.h>
#include <t_stddef.h>
#include <t_stdlib.h>
#include <stdio.h>
#include "target_debug.h"


#include "kernel_cfg.h"
#include "task_ntshell.h"
#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"
#include "demo_light.h"


NTSHELL_CALL	ntshell_call_table[TNUM_NTSHELL_CALL];
static ntshell_t ntshell;


/**
 *    @brief   printf依赖读函数
 *
 *    @param   file
 *    @param   ptr   缓存指针
 *    @param   len   数据长度
 *
 *    @return  res   读状态
 */
int _read(int file ,char *ptr, int len)
{
	int res = 0;

	if (file < 3)
	{
		/* read from serial port */
		res = serial_rea_dat(NTSHELL_PORTID, (char_t *)ptr, (uint_t)len);
	}
	return res;
}

/**
 *    @brief   printf依赖写函数
 *
 *    @param   file
 *    @param   ptr   缓存指针
 *    @param   len   数据长度
 *
 *    @return  res   写状态
 */
int _write(int file, char *ptr, int len)
{

	int res = 0;

	if (file < 3)
	{
		res = serial_wri_dat(NTSHELL_PORTID, (const char_t *)ptr, (uint_t)len);
	}
	return res;
}

static int func_read(char *buf, int cnt)
{
    return serial_rea_dat(NTSHELL_PORTID, (char_t *)buf, cnt);
}

static int func_write(const char *buf, int cnt)
{
    return serial_wri_dat(NTSHELL_PORTID, (const char_t *)buf, cnt);
}

static int func_cb(const char *text)
{
    /* Here is the user defined command interfaces */
	int i;
	char buf[NTOPT_MAXLEN_ARGV];
	ER	ercd;

	if (ntopt_get_text(text, 0, buf, sizeof(buf)) != 0)
	{
		for ( i = 0; i < TNUM_NTSHELL_CALL; i++) {
			/* serach call's name in ntshell_call_table */
			if (ntlibc_strcmp(ntshell_call_table[i].name, buf) == 0) {
				ercd = wai_sem(NTSHELL_CALL_SEM);
				if (ercd == E_OK) {
					ntopt_parse(text, ntshell_call_table[i].func);
				} else {
					printf("ntshell wai_sem error:%s\n", itron_strerror(ercd));
					return 0;
				}
				ercd = sig_sem(NTSHELL_CALL_SEM);
				if (ercd != E_OK) {
					printf("ntshell sig_sem error:%s\n", itron_strerror(ercd));
				}
				return 0;
			}
		}
		puts("no such a command\n");
	}
    return 0;
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  ntshell_register
 *  Description:  register a function call in ntshell
 *	Parameters:
 *			name,  function call's name
 *			call,  function call's address
 * 	return value:
 * 			call number(>=0)
 *			-1	fail
 *  Created:  2011/11/20 by Ren Wei
 * =====================================================================================
 */
int ntshell_register(const char *name, ntshell_call_func call)
{
	int i;

	for ( i = 0; i < TNUM_NTSHELL_CALL; i++) {
		if (ntshell_call_table[i].name == 0 && ntshell_call_table[i].func == 0) {
			ntshell_call_table[i].name = name;
			ntshell_call_table[i].func = call;
			return i;
		}
	}

	return -1; /* there is no space in ntshell_call_table */
}

void task_ntshell(intptr_t exinf)
{
	ER ercd;

	ercd = serial_opn_por(NTSHELL_PORTID);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	serial_ctl_por(NTSHELL_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV));
    ntshell_execute(&ntshell,func_read, func_write, func_cb);
}
