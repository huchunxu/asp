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
 *  @(#) $Id: net.h,v 1.5 2009/12/24 05:42:40 abe Exp abe $
 */

/*
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _NET_H_
#define _NET_H_

/*
 *  バイトオーダ変換の定義
 */

#ifdef TARGET_KERNEL_ASP

#if defined(SIL_ENDIAN_BIG)

#undef  SIL_ENDIAN_BIG
#define SIL_ENDIAN_LITTLE	0
#define SIL_ENDIAN_BIG		1
#define SIL_ENDIAN		SIL_ENDIAN_BIG

#elif defined(SIL_ENDIAN_LITTLE)

#undef  SIL_ENDIAN_LITTLE
#define SIL_ENDIAN_LITTLE	0
#define SIL_ENDIAN_BIG		1
#define SIL_ENDIAN		SIL_ENDIAN_LITTLE

#endif	/* of #if defined(SIL_ENDIAN_BIG) */

#endif	/* of #ifdef TARGET_KERNEL_ASP */

#if !defined(SIL_ENDIAN)

#error "SIL_ENDIAN expected."

#else	/* of #if !defined(SIL_ENDIAN) */

#if SIL_ENDIAN == SIL_ENDIAN_BIG

#define ntohs(n)		((uint16_t)n)
#define htons(h)		((uint16_t)h)
#define ntohl(n)		((uint32_t)n)
#define htonl(h)		((uint32_t)h)

#define NTOHS(n)
#define HTONS(h)
#define NTOHL(n)
#define HTONL(h)

/*
 *  注意: IPヘッダ以降は、4 バイト境界にアラインされている事を
 *        前提としているが、4 バイト境界でアクセスする
 *        プロセッサで、ネットワーク側のデータが、4 バイト境界にアライン
 *        されていない場合は、ntoahl、ahtonl、nahcmpl を使用すること。
 */

#define ntoahl(h,n)		memcpy((uint8_t*)&(h),(n),4)
#define ahtonl(n,h)		memcpy((n),(uint8_t*)&(h),4)

#define nahcmpl(n,h)		memcmp((n),(uint8_t*)&(h),4)

#elif SIL_ENDIAN == SIL_ENDIAN_LITTLE

#ifndef NET_REV_ENDIAN_HWORD

#define NET_REV_ENDIAN_HWORD(d)	((uint16_t)((((uint16_t)(d)&0xff)<<8)|(((uint16_t)(d)>>8)&0xff)))

#endif	/* of #ifndef NET_REV_ENDIAN_HWORD */

#ifndef NET_REV_ENDIAN_WORD

#define NET_REV_ENDIAN_WORD(d)	((uint32_t)((((uint32_t)(d)&0xff)<<24)|(((uint32_t)(d)&0xff00)<<8)| \
				            (((uint32_t)(d)>>8)&0xff00)|(((uint32_t)(d)>>24)&0xff)))

#endif	/* of #ifndef NET_REV_ENDIAN_WORD */

#define ntohs(n)		NET_REV_ENDIAN_HWORD(n)
#define htons(h)		NET_REV_ENDIAN_HWORD(h)
#define ntohl(n)		NET_REV_ENDIAN_WORD(n)
#define htonl(h)		NET_REV_ENDIAN_WORD(h)

#define NTOHS(n)		((n)=NET_REV_ENDIAN_HWORD(n))
#define HTONS(h)		((h)=NET_REV_ENDIAN_HWORD(h))
#define NTOHL(n)		((n)=NET_REV_ENDIAN_WORD(n))
#define HTONL(h)		((h)=NET_REV_ENDIAN_WORD(h))

/*
 *  注意: IPヘッダ以降は、4 バイト境界にアラインされている事を
 *        前提としているが、4 バイト境界でアクセスする
 *        プロセッサで、ネットワーク側のデータが、4 バイト境界にアライン
 *        されていない場合は、ntoahl、ahtonl、nahcmpl を使用すること。
 */

#ifndef _MACRO_ONLY

extern void rev_memcpy_word (uint8_t *dst, uint8_t *src);
extern int  rev_memcmp_word (uint8_t *data1, uint8_t *data2);

#endif	/* of #ifndef _MACRO_ONLY */

#define ntoahl(h,n)		rev_memcpy_word((uint8_t*)&(h),(n))
#define ahtonl(n,h)		rev_memcpy_word((n),(uint8_t*)&(h))
#define nahcmpl(n,h)		rev_memcmp_word((n),(uint8_t*)&(h))

#endif	/* #if SIL_ENDIAN == SIL_ENDIAN_BIG */

#endif	/* of #if !defined(SIL_ENDIAN) */

/*
 *  プロトコルを識別するフラグに関する定義
 *
 *    ・統計情報
 */

#define PROTO_FLG_PPP_HDLC	ULONG_C(0x00000001)	/* ppp_hdlc.c	*/
#define PROTO_FLG_PPP_FSM	ULONG_C(0x00000002)	/* ppp_fsm.c	*/
#define PROTO_FLG_PPP_AUTH	ULONG_C(0x00000004)	/* ppp_auth.c	*/
#define PROTO_FLG_PPP_LCP	ULONG_C(0x00000008)	/* ppp_lcp.c	*/
#define PROTO_FLG_PPP_IPCP	ULONG_C(0x00000010)	/* ppp_ipcp.c	*/
#define PROTO_FLG_PPP_CCP	ULONG_C(0x00000020)	/* ppp_ccp.c	*/
#define PROTO_FLG_PPP_PAP	ULONG_C(0x00000040)	/* ppp_upap.c	*/
#define PROTO_FLG_PPP_MODEM	ULONG_C(0x00000080)	/* ppp_modem.c	*/
#define PROTO_FLG_PPP		ULONG_C(0x00000100)	/* ppp.c	*/

#define PROTO_FLG_ETHER_NIC	ULONG_C(0x00000001)	/* if_??.c	*/

#define PROTO_FLG_LOOP		ULONG_C(0x00000100)	/* if_loop.c	*/

#define PROTO_FLG_ETHER		ULONG_C(0x00000100)	/* ethernet.c	*/

#define PROTO_FLG_IF		ULONG_C(0x00000800)	/* if.c		*/

#define PROTO_FLG_ARP		ULONG_C(0x00008000)	/* if_ether.c	*/

#define PROTO_FLG_IP4		ULONG_C(0x00010000)	/* ip_*.c	*/
#define PROTO_FLG_ICMP4		ULONG_C(0x00040000)	/* ip_icmp.c	*/

#define PROTO_FLG_IP6		ULONG_C(0x00100000)	/* ip6_*.c	*/
#define PROTO_FLG_ICMP6		ULONG_C(0x00400000)	/* icmp6.c	*/
#define PROTO_FLG_ND6		ULONG_C(0x00800000)	/* nd6_*.c	*/

#define PROTO_FLG_UDP		ULONG_C(0x08000000)	/* udp_*.c	*/

#define PROTO_FLG_TCP		ULONG_C(0x01000000)	/* tcp_*.c	*/

#define PROTO_FLG_NET_BUF	ULONG_C(0x10000000)	/* net_buf.c	*/
#define PROTO_FLG_NET		ULONG_C(0x80000000)	/* route.c, net*.c	*/

/*
 *  アドレス形式の定義
 */

#define AT_ETHER		UINT_C(0x01)		/* イーサネット		*/
#define AT_INET4		UINT_C(0x10)		/* IPv4 アドレス	*/
#define AT_INET6		UINT_C(0x20)		/* IPv6 アドレス	*/

#if !defined(TOPPERS_MACRO_ONLY) && !defined(_MACRO_ONLY)

/*
 *  TINET 独自関数
 */

extern char *mac2str (char *buf, uint8_t *macaddr);

#endif /* #if !defined(TOPPERS_MACRO_ONLY) && !defined(_MACRO_ONLY) */

#endif	/* of #ifndef _NET_H_ */
