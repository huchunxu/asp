/*
 *  TINET (TCP/IP Protocol Stack)
 * 
 *  Copyright (C) 2001-2009 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次の条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: if_lpc.h,v 1.5 2011/01/18 renwei $
 */


#ifndef _IF_LPC_H_
#define _IF_LPC_H_

/*
 *  NIC macro
 *  the following functions should be implemented
 */
#define IF_ETHER_NIC_GET_SOFTC()	if_lpc_get_softc() /* get the nic dependent data structure */
#define IF_ETHER_NIC_WATCHDOG(i)	if_lpc_watchdog(i) /* wathch dog  time out */
#define IF_ETHER_NIC_PROBE(i)		if_lpc_probe(i)	/* probe the existence of NIC and get info */
#define IF_ETHER_NIC_INIT(i)		if_lpc_open(i)		/* init the nic */
#define IF_ETHER_NIC_READ(i)		if_lpc_read(i)		/* read one fram from data */
#define IF_ETHER_NIC_RESET(i)		if_lpc_reset(i)	/* reset the nic */
#define IF_ETHER_NIC_START(i,o)		if_lpc_write(i,o)	/* start the transmition of one frame */

#define T_IF_ETHER_NIC_SOFTC		struct t_if_lpc_softc

/* IPv6 related */

#define IF_ETHER_NIC_IN6_IFID(i,a)	get_mac6_ifid(i,a)	/*  set interface id        */
#define IF_ETHER_NIC_ADDMULTI(s)	if_lpc_addmulti(s)		/*  add multicase address	*/

#if !defined(TOPPERS_MACRO_ONLY) && !defined(_MACRO_ONLY)


#ifndef T_IF_SOFTC_DEFINED

typedef struct t_if_softc T_IF_SOFTC;

#define T_IF_SOFTC_DEFINED

#endif	/* of #ifndef T_IF_SOFTC_DEFINED */

#ifndef T_NET_BUF_DEFINED

typedef struct t_net_buf T_NET_BUF;

#define T_NET_BUF_DEFINED

#endif	/* of #ifndef T_NET_BUF_DEFINED */

/*
 *  function declaration
 */
extern void if_lpc_initialize(intptr_t exinf);
extern T_IF_SOFTC* if_lpc_get_softc(void);
extern void if_lpc_watchdog(T_IF_SOFTC *ic);
extern void if_lpc_probe(T_IF_SOFTC *ic);
extern void if_lpc_open(T_IF_SOFTC *ic);
extern void if_lpc_reset(T_IF_SOFTC *ic);
extern T_NET_BUF* if_lpc_read(T_IF_SOFTC *ic);
extern void if_lpc_write(T_IF_SOFTC *ic, T_NET_BUF *output);
extern ER if_lpc_addmulti(T_IF_SOFTC *ic);
extern void if_lpc_handler(void);	/* interrupt handler */

#endif /* #if !defined(TOPPERS_MACRO_ONLY) && !defined(_MACRO_ONLY) */

#endif	/* of #ifndef _IF_LPC_H_ */
