/*
 * =====================================================================================
 *
 *       Filename:  target_debug.h
 *
 *    Description:  debug configuration for target
 *
 *        Version:  0.1
 *        Created:  2011-4-14 16:28:37
 *
 *         Author:  Ren Wei , renweihust@gmail.com
 *        Company:  HUST-Renesas Lab
 *       Revision:
 * =====================================================================================
 *	@0.1 	Ren Wei	2011-4-14	create orignal file
 * =====================================================================================
 */

#ifndef  TARGET_DEBUG_H
#define  TARGET_DEBUG_H

#define DEBUG
#include <t_stddef.h>
#include <t_syslog.h>  /* TOPPERS/ASP log service */
#include <t_stdlib.h>

#ifdef DEBUG
#define DBG(x, args...)	syslog(LOG_NOTICE, x, ##args)

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}
#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))
#else
#define DBG(x, args...)
#define	SVC_PERROR(expr)	(expr)
#endif




#endif   /* ----- #ifndef TARGET_DEBUG_INC  ----- */

