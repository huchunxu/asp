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
 *  @(#) $Id: $
 */

#ifndef _TINET_TARGET_CONFIG_H_
#define _TINET_TARGET_CONFIG_H_

/*
 *  TCP/IP related definition
 */

/* TCP に関する定義 */

/*
 *  MAX_TCP_SND_SEG: the maximum send segment size
 *
 */

#ifndef MAX_TCP_SND_SEG
#define MAX_TCP_SND_SEG		(IF_MTU - (IP_HDR_SIZE + TCP_HDR_SIZE))
#endif	/* of #ifndef MAX_TCP_SND_SEG */

/*
 *  DEF_TCP_RCV_SEG: 受信セグメントサイズの規定値
 */

#ifndef DEF_TCP_RCV_SEG
#define DEF_TCP_RCV_SEG		(IF_MTU - (IP_HDR_SIZE + TCP_HDR_SIZE))
#endif	/* of #ifndef DEF_TCP_RCV_SEG */

/*
 *  セグメントの順番を入れ替えるときに新たにネットワークバッファを割当てて、
 *  データをコピーするサイズのしきい値
 */
#define MAX_TCP_REALLOC_SIZE	1024

#define TCP_CFG_OPT_MSS		/* コネクション開設時に、セグメントサイズオプションをつけて送信する。*/
#define TCP_CFG_DELAY_ACK	/* ACK を遅らせるときはコメントを外す。			*/
#define TCP_CFG_ALWAYS_KEEP	/* 常にキープアライブする時はコメントを外す。		*/

/* UDP related definition */

#define UDP_CFG_IN_CHECKSUM		/* UDP の入力チェックサムを行う場合はコメントを外す。	*/
#define UDP_CFG_OUT_CHECKSUM	/* UDP の出力チェックサムを行う場合はコメントを外す。	*/

/* ICMPv4/v6 related definition */

#define ICMP_REPLY_ERROR		/* ICMP エラーメッセージを送信する場合はコメントを外す。*/

/* IPv4 に関する定義 */

#define IP4_CFG_FRAGMENT	/* データグラムの分割・再構成行う場合はコメントを外す。	*/
#define NUM_IP4_FRAG_QUEUE	4	/* データグラム再構成キューサイズ			*/
#define IP4_CFG_FRAG_REASSM_SIZE	4096	/* IPv4 再構成バッファサイズ			*/

/*
 *  data link layer(network interface) related definition
 */
/*
 *  lpc emac related definition
 */
#define TMO_IF_LPC_GET_NET_BUF	30	/* [ms]、time out of accquirring receive net buf	*/
									/* [s]、 send timeout			*/
#define TMO_IF_LPC_XMIT		(2*IF_TIMER_HZ)

/*#define IF_LPC_CFG_ACCEPT_ALL		 マルチキャスト、エラーフレームも受信するときはコメントを外す。*/

/*
 *  イーサネット出力時に、NIC で net_buf を開放する場合に指定する。
 *
 *  注意: 以下の指定は、指定例であり、if_ed では、
 *        開放しないので、以下のコメントを外してはならない。
 */

/*#define ETHER_NIC_CFG_RELEASE_NET_BUF*/

#define INHNO_IF_LPC		21			/* ethernet int */
#define INTNO_IF_LPC		21			/* ethernet int */
#define INTATR_IF_LPC	(TA_NULL)	/* interrupt attribute	*/
#define INTPRI_IF_LPC	(-5)		/* interrupt priority	*/

/*
 *  ARP related definition
 */

#define NUM_ARP_ENTRY		10			/* ARP Cache entries*/

/*
 *  DHCP related definition
 *
 *    ・注意: TINET は、DHCP を実装していない。応用プログラムで、
 *            DHCP メッセージを受信するための定義である。
 *            また、現在は IPv4 のみ有効である。
 */

/*#define DHCP_CFG					 DHCP を処理する場合はコメントを外す。*/

/*
 *  Ethernet に関する定義
 */

/*#define ETHER_CFG_ACCEPT_ALL		 マルチキャスト、エラーフレームも受信するときはコメントを外す。	*/
/*#define ETHER_CFG_UNEXP_WARNING	 非サポートフレームの警告を表示するときはコメントを外す。		*/
/*#define ETHER_CFG_802_WARNING		 IEEE 802.3 フレームの警告を表示するときはコメントを外す。		*/
/*#define ETHER_CFG_MCAST_WARNING	 マルチキャストの警告を表示するときはコメントを外す。		*/


/*
 *  common network related definition
 */

/*
 *  network buffer related definition
 */

/* nums of network buffer */
#ifdef SUPPORT_ETHER /* ether related definition */

/*
 *  イーサネットの場合のネットワークバッファ数の割り当て
 */

/*
 *  注意!!
 *
 *  NE2000 互換 NIC のディバイスドライバ（if_ed）の最低割当て長は
 *  60（アラインして 62）オクテットのため IF + IP +TCP よりは
 *  64 オクテットのネットワークバッファの方が最適である。
 */

#ifndef NUM_MPF_NET_BUF_CSEG
#define NUM_MPF_NET_BUF_CSEG	0	/* IF + IP + TCP	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_CSEG */

#ifndef NUM_MPF_NET_BUF_64
#define NUM_MPF_NET_BUF_64	2	/* 64 オクテット	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_64 */

#ifndef NUM_MPF_NET_BUF_128
#define NUM_MPF_NET_BUF_128	0	/* 128 オクテット	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_128 */

#ifndef NUM_MPF_NET_BUF_256
#define NUM_MPF_NET_BUF_256	0	/* 256 オクテット	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_256 */

#ifndef NUM_MPF_NET_BUF_512
#define NUM_MPF_NET_BUF_512	0	/* 512 オクテット	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_512 */

#if defined(SUPPORT_INET4)

#ifndef NUM_MPF_NET_BUF_IP_MSS
#define NUM_MPF_NET_BUF_IP_MSS	0	/* IF + 576 オクテット	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_IP_MSS */

#endif	/* of #if defined(SUPPORT_INET4) */

#ifndef NUM_MPF_NET_BUF_1024
#define NUM_MPF_NET_BUF_1024	0	/* 1024 オクテット	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_1024 */


#ifndef NUM_MPF_NET_BUF_IF_PDU
#define NUM_MPF_NET_BUF_IF_PDU	4	/* IF 最大 PDU サイズ	*/
#endif	/* of #ifndef NUM_MPF_NET_BUF_IF_PDU */

#ifndef NUM_MPF_NET_BUF4_REASSM

#if defined(SUPPORT_INET4) && defined(IP4_CFG_FRAGMENT)
#define NUM_MPF_NET_BUF4_REASSM	NUM_IP4_FRAG_QUEUE	/* IPv4 再構成バッファサイズ	*/
#else
#define NUM_MPF_NET_BUF4_REASSM	0	/* IPv4 再構成バッファサイズ	*/
#endif

#endif	/* of #ifndef NUM_MPF_NET_BUF4_REASSM */

#endif	/* of #ifdef SUPPORT_ETHER */

/*
 *  ネットワーク統計情報の計測
 *
 *  ネットワーク統計情報の計測を行う場合は、tinet/include/net/net.h
 *  で定義されているプロトコル識別フラグを指定する。
 */

#if 1

#ifdef SUPPORT_INET4

#define NET_COUNT_ENABLE	(0			\
				| PROTO_FLG_PPP_HDLC	\
				| PROTO_FLG_PPP_PAP	\
				| PROTO_FLG_PPP_LCP	\
				| PROTO_FLG_PPP_IPCP	\
				| PROTO_FLG_PPP		\
				| PROTO_FLG_LOOP	\
				| PROTO_FLG_ETHER_NIC	\
				| PROTO_FLG_ETHER	\
				| PROTO_FLG_ARP		\
				| PROTO_FLG_IP4		\
				| PROTO_FLG_ICMP4	\
				| PROTO_FLG_UDP		\
				| PROTO_FLG_TCP		\
				| PROTO_FLG_NET_BUF	\
				)

#endif	/* of #ifdef SUPPORT_INET4 */

#else	/* of #if 0 */

#define NET_COUNT_ENABLE	(0			\
				)

#endif	/* of #if 0 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  function declaration
 */

//#include "LPC24xx.h"
#include <sil.h>

extern void if_lpc_bus_init (void);
extern void if_lpc_inter_init (void);

static inline void
if_lpc_inter_clear(void)
{
}


#endif	/* of #ifndef TOPPERS_MACRO_ONLY */

#endif /* _TINET_TARGET_CONFIG_H_ */
