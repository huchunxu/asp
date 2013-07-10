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
 *  @(#) $Id: if_lpc.c,v 1.5 2010/01/18 renwei $
 */

/*
 * Copyright (c) 1995, David Greenman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/i386/isa/if_ed.c,v 1.148.2.4 1999/09/25 13:08:18 nyan Exp $
 */


#ifdef TARGET_KERNEL_ASP

#include <kernel.h>
#include <sil.h>
#include <t_syslog.h>
#include "kernel_cfg.h"
#include "target_config.h"
#include "target_debug.h"

#endif	/* of #ifdef TARGET_KERNEL_ASP */

#ifdef TARGET_KERNEL_JSP

#include <s_services.h>
#include <t_services.h>
#include "kernel_id.h"

#endif	/* of #ifdef TARGET_KERNEL_JSP */

#include <tinet_defs.h>
#include <tinet_config.h>

#include <net/if.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <net/net.h>
#include <net/net_timer.h>
#include <net/net_count.h>
#include <net/net_buf.h>

#include "if_lpcreg.h"
#include "dmm_info.h"


/*
 *  NIC information(hardware dependent)
 */

/* the LPC target data structure */
typedef struct t_if_lpc_softc {
	uint32_t	mac_base;			/* emac base address	  */
	uint16_t	isq;				/* interrupt status queue */
	bool_t		rx_empty;			/* rx empty flag */
	bool_t		tx_full;			/* tx full flag */
} T_IF_LPC_SOFTC;



static T_IF_LPC_SOFTC if_lpc_softc = {
	MAC_BASE_ADDR,					/* mac base address	*/
	0,								/* IRQ 0 connected to host */
	true,
	false
	};

/* */
T_IF_SOFTC if_softc = {
	{{0x00,0x14,0x97,0x0F,0xf4,0x8F}},/* ethernet mac address	*/
	0,								/* send timeout			*/
	&if_lpc_softc,						/* nic info	*/
	SEM_IF_LPC_SBUF_READY,			/* send semaphore		*/
	SEM_IF_LPC_RBUF_READY,			/* receive semaphore	*/

#ifdef SUPPORT_INET6
	IF_MADDR_INIT,					/* multicast address list	*/
#endif	/* of #ifdef SUPPORT_INET6 */
	};




static void lpc_hardware_close(T_IF_LPC_SOFTC *lpc);		/* close the lpc ethernet block */
static void lpc_hardware_open(T_IF_SOFTC *ic); 			/* open  the lpc ethernet block */
static void write_phy(uint8_t phyreg, uint16_t value);	/* write data to the phy 	 */
static uint16_t read_phy(uint8_t phyreg);				/* read data from phy		 */
static void rx_descr_init(void);						/* init rx descriptors array */
static void tx_descr_init(void);						/* init tx descriptors array */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  write_phy
 *  Description:  write data to phy
 *	Parameters:
 *			phyreg	phy register
 *			value	date to be written
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static void
write_phy(uint8_t phyreg, uint16_t value)
{
  uint32_t tout;

  sil_wrw_mem((void *)MAC_MADR,  DP83848C_DEF_ADR | phyreg);
  sil_wrw_mem((void *)MAC_MWTD,  value);

  /* Wait utill operation completed */
  for (tout = 0; tout < MII_WR_TOUT; tout++) {
    if ((sil_rew_mem((void *)MAC_MIND) & MIND_BUSY) == 0) {
      break;
    }
  }
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  read_phy
 *  Description:  read data from phy
 *	Parameters:
 *			phyreg	phyregister
 * 	return value:
 * 			data to read
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static uint16_t
read_phy(uint8_t phyreg)
{
	unsigned int tout;

    sil_wrw_mem((void *)MAC_MADR, DP83848C_DEF_ADR | phyreg);
	sil_wrw_mem((void *)MAC_MCMD, MCMD_READ);

  /* Wait until operation completed */
  for (tout = 0; tout < MII_RD_TOUT; tout++) {
    if ((sil_rew_mem((void *)MAC_MIND) & MIND_BUSY) == 0) {
      break;
    }
  }
  sil_wrw_mem((void *)MAC_MCMD, 0);
  return (sil_rew_mem((void *)MAC_MRDD));
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  rx_descr_init
 *  Description:  initialize rx descriptors array. one frame = one fragment
 *	Parameters:	
 *			none
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static void
rx_descr_init(void)
{
  uint32_t i;

  for (i = 0; i < NUM_RX_FRAG; i++) {
    RX_DESC_PACKET(i)  = RX_BUF(i) + 2;
    RX_DESC_CTRL(i)    = RCTRL_INT | (ETH_FRAG_SIZE-1);
    RX_STAT_INFO(i)    = 0;
    RX_STAT_HASHCRC(i) = 0;
  }

  /* Set EMAC Receive Descriptor Registers. */

  sil_wrw_mem((void *)MAC_RXDESCRIPTOR, RX_DESC_BASE);
  sil_wrw_mem((void *)MAC_RXSTATUS, RX_STAT_BASE);
  sil_wrw_mem((void *)MAC_RXDESCRIPTORNUM, NUM_RX_FRAG-1);

  /* Rx Descriptors Pointer to 0 */
  sil_wrw_mem((void *)MAC_RXCONSUMEINDEX, 0);
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  tx_descr_init
 *  Description:  initialize tx descriptors array. one frame = one framgment
 *	Parameters:
 *			none
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static void
tx_descr_init(void)
{
  uint32_t i;

  for (i = 0; i < NUM_TX_FRAG; i++) {
    TX_DESC_PACKET(i) = TX_BUF(i) + 2;
    TX_DESC_CTRL(i)   = 0;
    TX_STAT_INFO(i)   = 0;
  }

  /* Set EMAC Transmit Descriptor Registers. */
  sil_wrw_mem((void *)MAC_TXDESCRIPTOR, TX_DESC_BASE);
  sil_wrw_mem((void *)MAC_TXSTATUS, TX_STAT_BASE);
  sil_wrw_mem((void *)MAC_TXDESCRIPTORNUM, NUM_TX_FRAG-1);

  /* Tx Descriptors Point to 0 */
  sil_wrw_mem((void *)MAC_TXPRODUCEINDEX, 0);
}


#ifdef SUPPORT_INET6

static uint32_t ds_crc (uint8_t *addr);
static void ds_getmcaf (T_IF_SOFTC *ic, uint32_t *mcaf);


/*
 *  ds_crc -- caculate the CRC of Destination address
 */

#define POLYNOMIAL	0x04c11db6 /* CRC32 for Net */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  ds_crc
 *  Description:  calculate the crc of destination address.
 *	Parameters:
 *			addr, pointer to address
 * 	return value:
 * 			crc value
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static uint32_t
ds_crc(uint8_t *addr)
{
	uint32_t	crc = ULONG_C(0xffffffff);
	int32_t		carry, len, bit;
	uint8_t		byte;

	for (len = ETHER_ADDR_LEN; len -- > 0; ) {
		byte = *addr ++;
		for (bit = 8; bit -- > 0; ) {
			carry   = ((crc & ULONG_C(0x80000000)) ? 1 : 0) ^ (byte & UINT_C(0x01));
			crc   <<= 1;
			byte   >>= 1;
			if (carry)
				crc = (crc ^ POLYNOMIAL) | carry;
			}
		}
	return crc;
}

#undef POLYNOMIAL

/*
 *  ds_getmcaf --
 *
 */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  ds_getmcaf
 *  Description:  caculate the hash address filter according to the
 * 				  multicast address list
 *	Parameters:
 *			ic,	pointer to nic data structure
 *			pointer to multicast address hash filter
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static void
ds_getmcaf (T_IF_SOFTC *ic, uint32_t *mcaf)
{
	uint32_t	count, index;
	uint8_t		*af = (uint8_t*)mcaf;

	mcaf[0] = mcaf[1] = 0;

	for (count = MAX_IF_MADDR_CNT; count -- > 0; )
   	{
		index = (ds_crc(ic->maddrs[count].lladdr) >> 22) & 0x03f; /* get bit 23-28 to form the index */
		af[index >> 3] |= 1 << (index & 7);
	}
}



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  lpc_setmulti
 *  Description:  set multi cast hash filter
 *	Parameters:
 *			ic,	pointer to nic data structure
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static void
lpc_setmulti(T_IF_SOFTC *ic)
{
	T_IF_LPC_SOFTC	*lpc = ic->sc;
	uint32_t	mcaf[2];

	ds_getmcaf(ic, mcaf); /* get the hash address filter */

	 /* set the hash address filter */
	sil_wrw_mem((void *)(MAC_HASHFILTERL), mcaf[0]);
	sil_wrw_mem((void *)(MAC_HASHFILTERH), mcaf[1]);

	 /* enable multicast and unicast */

	  sil_wrw_mem((void *)MAC_RXFILTERCTRL, RFC_UCAST_EN |
					 		 RFC_MCAST_EN | 		/* accept multicast */
							 RFC_PERFECT_EN |		/* only accept my frames */
							 RFC_MCAST_HASH_EN);	/* multicast hash filter enable */
	 /* for IPV6, broadcast is disabled */
}


/*-----------------------------------------------------------------------------
 *  public functions called by TINET when IPv6 enable
 *-----------------------------------------------------------------------------*/

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  lpc_addmulti
 *  Description:  add multicast address to the nic
 *	Parameters:
 *			ic, pointer to nic data structure
 * 	return value:
 * 			error code
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
ER
lpc_addmulti(T_IF_SOFTC *ic)
{
	lpc_setmulti(ic);
	return E_OK;
}

#endif	/* of #ifdef SUPPORT_INET6 */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  lpc_hardware_close
 *  Description:  stop the nic, disable transmit and receive, disable interrupt
 *	Parameters:
 *			lpc,  pointer to lpc emac data structure
 * 	return value:
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static void
lpc_hardware_close(T_IF_LPC_SOFTC *lpc)
{
	sil_wrw_mem((void *)MAC_COMMAND, ~(CR_RX_EN | CR_TX_EN));
	sil_wrw_mem((void *)MAC_INTENABLE, 0); /* disable all ints */
	/* Power off the EMAC controller. */
  	sil_wrw_mem((void *)PCONP, sil_rew_mem((void *)PCONP) & ~(0x40000000));
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  lpc_hardware_open
 *  Description:  open the hardware of lpc emac
 *	Parameters:
 *		ic,	pointer to nic data structure
 * 	return value:
 * 		none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
static void
lpc_hardware_open(T_IF_SOFTC *ic)
{
/* Initializes the EMAC ethernet controller */
 	uint32_t regv = 0;
 	volatile uint32_t tout;
  	uint32_t id1;
  	uint32_t id2;

  	ic->timer = 0;

  /* Power Up the EMAC controller. */
  sil_wrw_mem((void *)PCONP, sil_rew_mem((void *)PCONP) | 0x40000000);

  /* Reset all EMAC internal modules. */
  sil_wrw_mem((void *)MAC_MAC1, MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX |
             MAC1_SIM_RES | MAC1_SOFT_RES);
  /* reset data path */
  sil_wrw_mem((void *)MAC_COMMAND, CR_REG_RES | CR_TX_RES | CR_RX_RES);

  /* A short delay after reset. */
  for (tout = 100; tout; tout--);

  /* Initialize MAC control registers. */
  sil_wrw_mem((void *)MAC_MAC1, MAC1_PASS_ALL); /* pass data & control frames */
  sil_wrw_mem((void *)MAC_MAC2, MAC2_CRC_EN | MAC2_PAD_EN); /* enable padding and crc */
  sil_wrw_mem((void *)MAC_MAXF, ETH_MAX_FLEN);
  sil_wrw_mem((void *)MAC_CLRT, CLRT_DEF);
  sil_wrw_mem((void *)MAC_IPGR, IPGR_DEF);

  /* Enable Reduced MII interface.  no runt frames*/
  sil_wrw_mem((void *)MAC_COMMAND, CR_RMII);

  /* Reset Reduced MII Logic. */
  sil_wrw_mem((void *)MAC_SUPP, SUPP_RES_RMII);

  for (tout = 100; tout; tout--);

  sil_wrw_mem((void *)MAC_SUPP, 0);

  /* Put the DP83848C in reset mode */
  write_phy(PHY_REG_BMCR, 0x8000);

  /* Wait for hardware reset to end. */
  for (tout = 0; tout < 0x100000; tout++) {
    regv = read_phy (PHY_REG_BMCR);
    if (!(regv & 0x8800)) {
      /* Reset complete */
      break;
    }
  }

  /* Check if this is a DP83848C PHY. */
  id1 = read_phy(PHY_REG_IDR1);
  id2 = read_phy(PHY_REG_IDR2);
  if (((id1 << 16) | (id2 & 0xFFF0)) == DP83848C_ID) {
    /* Configure the PHY device */

    /* Use autonegotiation about the link speed. 10/100M */
    write_phy(PHY_REG_BMCR, PHY_AUTO_NEG);
    /* Wait to complete Auto_Negotiation. */
    for (tout = 0; tout < 0x100000; tout++) {
      regv = read_phy(PHY_REG_BMSR);
      if (regv & 0x0020) {
        /* Autonegotiation Complete. */
        break;
      }
    }
  }

  /* Check the link status. */
  for (tout = 0; tout < 0x10000; tout++) {
    regv = read_phy(PHY_REG_STS);
    if (regv & 0x0001) {
      /* Link is on. */
      break;
    }
  }

  /* Configure Full/Half Duplex mode. */
  if (regv & 0x0004){
    /* Full duplex is enabled. */
	/* DBG("full duplex"); */
    sil_wrw_mem((void *)MAC_MAC2, sil_rew_mem((void *)MAC_MAC2) | MAC2_FULL_DUP);
    sil_wrw_mem((void *)MAC_COMMAND, sil_rew_mem((void *)MAC_COMMAND) | CR_FULL_DUP);
    sil_wrw_mem((void *)MAC_IPGT, IPGT_FULL_DUP);
  }
  else {
    /* Half duplex mode. */
    sil_wrw_mem((void *)MAC_IPGT, IPGT_HALF_DUP);
  }

  /* Configure 100MBit/10MBit mode. */
  if (regv & 0x0002) {
    /* 10MBit mode. */
    sil_wrw_mem((void *)MAC_SUPP, 0);
  }
  else {
	/* DBG("100M bit mode"); */
    /* 100MBit mode. */
    sil_wrw_mem((void *)MAC_SUPP, SUPP_SPEED);
  }

  /* Set the Ethernet MAC Address registers */
  sil_wrw_mem((void *)MAC_SA0, (ic->ifaddr.lladdr[0] << 8) | ic->ifaddr.lladdr[1]);
  sil_wrw_mem((void *)MAC_SA1, (ic->ifaddr.lladdr[2] << 8) | ic->ifaddr.lladdr[3]);
  sil_wrw_mem((void *)MAC_SA2, (ic->ifaddr.lladdr[4] << 8) | ic->ifaddr.lladdr[5]);

  /* Initialize Tx and Rx DMA Descriptors */
  rx_descr_init();
  tx_descr_init();

  /* Receive Broadcast and Perfect Match Packets */
  sil_wrw_mem((void *)MAC_RXFILTERCTRL, RFC_UCAST_EN | RFC_BCAST_EN | RFC_PERFECT_EN);

  /* Enable EMAC interrupts. */
  sil_wrw_mem((void *)MAC_INTENABLE, INT_RX_OVERRUN |
				  					 INT_RX_ERR		|
				  					 INT_RX_DONE 	|
									 INT_TX_UNDERRUN |
									 INT_TX_ERR		|
									 INT_TX_DONE);

  /* Reset all interrupts */
  sil_wrw_mem((void *)MAC_INTCLEAR, 0xFFFF);

  /* Enable receive and transmit mode of MAC Ethernet core */
  sil_wrw_mem((void *)MAC_COMMAND, sil_rew_mem((void *)MAC_COMMAND) | (CR_RX_EN | CR_TX_EN));
  sil_wrw_mem((void *)MAC_MAC1, sil_rew_mem((void *)MAC_MAC1) | MAC1_REC_EN);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_initialize
 *  Description:  lpc ether net hardware initialize routine.
 *  			  releated I/O pins are initialized here
 *	Parameters:
 *			exinf, parameter for this routine
 * 	return value:
 * 			none
 *  Created:  2012/4/13 by Ren Wei
 * =====================================================================================
 */
void if_lpc_initialize(intptr_t exinf) 
{
  /* Enable P1 Ethernet Pins. */
  /*
   * p1.0: ENET_TXD0, p1.1: ENET_TXD 1, p1.4:ENET_TX_EN
   * p1.8: ENET_CRS_DV, p1.9: ENET_RXD0, p1.10: ENET_RXD1,
   * p1.15: ENET_REF_CLK,  p1.14: ENET_RX_ER
   * p1.16: ENET_MDC, p1.17: ENET_MDIO
   */
  sil_wrw_mem((void *)PINSEL2, sil_rew_mem((void *)PINSEL2) | 0x50150105);
  sil_wrw_mem((void *)PINSEL3, sil_rew_mem((void *)PINSEL3) | 0x00000005);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_reset
 *  Description:  reset the lpc nic
 *	Parameters:
 *			ic,	pointer to nic data structure
 * 	return value:
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
void
if_lpc_reset(T_IF_SOFTC *ic)
{
#ifdef TARGET_KERNEL_JSP
#error "no support for JSP kernel"
#endif

#ifdef TARGET_KERNEL_ASP
	syscall(dis_int(INTNO_IF_LPC));
#endif
	DBG("lpc emac reset");
	NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_RESETS], 1);
	lpc_hardware_close(ic->sc); /* stop nic */
	lpc_hardware_open(ic); /* init nic */
	ic->sc->rx_empty = true;
  	ic->sc->tx_full= false;
  	ini_sem(ic->semid_txb_ready);
  	ini_sem(ic->semid_rxb_ready);
	/* enable interrupt from nic */
#ifdef TARGET_KERNEL_ASP
	syscall(ena_int(INTNO_IF_LPC));
#endif
}



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_get_softc
 *  Description:  get the lpc emc data structure
 *	Parameters:
 * 	return value:
 * 		pointer to the lpc emc data structure
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
T_IF_SOFTC *
if_lpc_get_softc(void)
{
	return &if_softc;
}



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_watchdog
 *  Description:  reset the lpc emc when timeout
 *	Parameters:
 *			ic,	  pointer to nic data structure
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
void
if_lpc_watchdog(T_IF_SOFTC *ic)
{
	DBG("NIC watchdog timeout");
	NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_OUT_ERR_PACKETS], 1);
	NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_TIMEOUTS], 1);
	if_lpc_reset(ic);

}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_probe
 *  Description:  probe the lpc emc,
 *  			  get info about lpc
 *  			  get the mac address of nic
 *				  if no eeprom present, set mac address with user's mac address
 *	Parameters:
 *			ic,	pointer to nic data structure
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
void
if_lpc_probe(T_IF_SOFTC *ic)
{

//	T_IF_LPC_SOFTC	*lpc = ic->sc;
#if defined(TARGET_KERNEL_ASP)
	/* init bus */
	if_lpc_bus_init();
	/* load mac address from DMM_INFO_BLOCK */
	if (DMM_INFO_PTR->mac_addr[0] != 0x00 || DMM_INFO_PTR->mac_addr[1] != 0x00 
		|| DMM_INFO_PTR->mac_addr[2] != 0x64) {
		DBG("mac address error:%x, %x, %x,", DMM_INFO_PTR->mac_addr[0],DMM_INFO_PTR->mac_addr[1],
				DMM_INFO_PTR->mac_addr[2]);
		return;
	}	
	ic->ifaddr.lladdr[0] = DMM_INFO_PTR->mac_addr[0];
	ic->ifaddr.lladdr[1] = DMM_INFO_PTR->mac_addr[1];
	ic->ifaddr.lladdr[2] = DMM_INFO_PTR->mac_addr[2];
	ic->ifaddr.lladdr[3] = DMM_INFO_PTR->mac_addr[3];
	ic->ifaddr.lladdr[4] = DMM_INFO_PTR->mac_addr[4];
	ic->ifaddr.lladdr[5] = DMM_INFO_PTR->mac_addr[5];
#endif	/* of #if defined(TARGET_KERNEL_ASP) */

}



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  iflpc_open
 *  Description:  open the lpc ethernet
 *	Parameters:
 *			ic, pointer to nic data structure
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
void
if_lpc_open(T_IF_SOFTC *ic)
{
#ifdef TARGET_KERNEL_JSP
#error "no support for JSP kernel"
#endif

#ifdef TARGET_KERNEL_ASP
	syscall(dis_int(INTNO_IF_LPC));
#endif

/* call the init functions of LPC*/
	lpc_hardware_open(ic);

#ifdef TARGET_KERNEL_ASP
	syscall(ena_int(INTNO_IF_LPC));
#endif
}

extern void net_rcv_overflow_hook(void) __attribute__((weak));
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  net_rcv_overflow_hook
 *  Description:  receive overflow hook for net.
 *  		when there is no net_buf for net receive, this hook
 *  		will be called. This hook is weak function
 *	Parameters:
 *		none
 * 	return value:
 * 		none
 *  Created:  2011-12-19 by Ren Wei
 * =====================================================================================
 */
void net_rcv_overflow_hook(void) {
	DBG("no buffer for net receive");
}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_read
 *  Description:  read frame from ethernet ram to tinet
 *	Parameters:
 *			ic,  pointer to nic data structure
 * 	return value:
 * 			pointer to the tinet buf
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
T_NET_BUF *
if_lpc_read(T_IF_SOFTC *ic)
{
	T_NET_BUF	*input = NULL;
	uint32_t	len;
	uint16_t	*dst;
	uint32_t 	*dst1;
	uint16_t 	*rptr;
	uint32_t	*rptr1;
	uint32_t 	idx;
	uint_t 	align;
	ER		ercd;

	idx = sil_rew_mem((void *)MAC_RXCONSUMEINDEX);
	len = (RX_STAT_INFO(idx) & RINFO_SIZE);
	rptr = (uint16_t *)RX_DESC_PACKET(idx);

	/* check whether data exists and valid */
	if (idx == sil_rew_mem((void *)MAC_RXPRODUCEINDEX) ||
		len == 0 || len > ETH_FRAG_SIZE) {
		DBG("invalid read");
		return NULL;
	}
	len -= 3; /*  -3, means no crc, 1 means incluing FCS */
	/* check RX status */
	if(RX_STAT_INFO(idx) & (RINFO_ALIGN_ERR | RINFO_SYM_ERR |  RINFO_LEN_ERR | RINFO_OVERRUN | RINFO_CRC_ERR))
	{
		DBG("Packet Error:%x", RX_STAT_INFO(idx));
		NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_IN_ERR_PACKETS], 1);
	}
	else {
		//DBG("l:%d,c:%d,p:%d",len,idx,sil_rew_mem((void *)MAC_RXPRODUCEINDEX));
		align = ((((len - sizeof(T_IF_HDR)) + 3 + IF_ETHER_NIC_HDR_ALIGN) >> 2) << 2) + sizeof(T_IF_HDR);
		/* try to get buf from NET_BUF */
		if ((ercd = tget_net_buf(&input, align, TMO_IF_LPC_GET_NET_BUF)) == E_OK && input != NULL) {
			NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_IN_PACKETS], 1);
			NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_IN_OCTETS],  len);
			dst = (uint16_t *)(input->buf + IF_ETHER_NIC_HDR_ALIGN);
			
			/* copy first 2 bytes, then address is 4 bytes align */
			*dst++ = *rptr++;
			len -=2;
			
			/* copy in four bytes */
			rptr1 = (uint32_t *)(rptr);
			dst1 = (uint32_t *)(dst);
			while (len > 3) {
				*dst1++ = *rptr1++;
				len -= 4;
			}
			/* copy last bytes */
			rptr = (uint16_t *)(rptr1);
			dst = (uint16_t *)(dst1);

			while (len > 1) {
				*dst++ = *rptr++;
				len -=2;
			}

			if (len) {
				*(uint8_t *)dst = (uint8_t)(*rptr++);
			}
		}
		else {
			NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_IN_ERR_PACKETS], 1);
			NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_NO_BUFS], 1);
			net_rcv_overflow_hook();
		}
	}

	if (++idx == NUM_RX_FRAG) {
		idx = 0; /* wrap */
	}
	sil_wrw_mem((void *)MAC_RXCONSUMEINDEX, idx);
	if (idx != sil_rew_mem((void *)MAC_RXPRODUCEINDEX)) {
		/* Rx buffer is not empty */
		ercd = sig_sem(ic->semid_rxb_ready);	
		if ( ercd != E_OK) {
			DBG("lpc_read rx sig_sem error:%s", itron_strerror(ercd));
			NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_RXB_QOVRS], 1);
		}
	} else {
		ic->sc->rx_empty = true;
	}

	return input;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_write
 *  Description:  send ethernet frame from tinet's net buf  to ethernet ram
 *	Parameters:
 *			ic,	pointer to nic data structure
 *			output, pointer to tinet net's buf
 * 	return value:
 * 			none
 *  Created:  2011-4-25 by Ren Wei
 * =====================================================================================
 */
void
if_lpc_write(T_IF_SOFTC *ic, T_NET_BUF *output)
{
	uint32_t idx;
	uint16_t *tsrc;
	uint32_t *tsrc1;
	uint16_t *tptr;
	uint32_t *tptr1;
	uint32_t len;
	ER ercd;


	NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_OUT_PACKETS], 1);
	NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_OUT_OCTETS],  output->len);

	/* send frame from buffer to lpc */
	tsrc = (uint16_t *)(output->buf + IF_ETHER_NIC_HDR_ALIGN);
	len = output->len - IF_ETHER_NIC_HDR_ALIGN;

	idx = sil_rew_mem((void *)MAC_TXPRODUCEINDEX) & 0x0000ffff;
	tptr = (uint16_t *)TX_DESC_PACKET(idx);
	/* add crc, enable padding, generate int when finished */
  	TX_DESC_CTRL(idx) = (len-1) | TCTRL_LAST | TCTRL_CRC | TCTRL_PAD | TCTRL_INT;
	
	/* copy first 2bytes, then address is align */	
	*tptr++ = *tsrc++;
	/* copy left bytes  */
	tptr1 = (uint32_t *)(tptr);
	tsrc1 = (uint32_t *)(tsrc);
	len -= 2;
	len = (len + 3) & 0xFFFC;
	while (len > 3) {
		*tptr1++ = *tsrc1++;
		len -= 4;
	}

	if (++idx == NUM_TX_FRAG) {
		idx = 0;
	}

	if (idx != sil_rew_mem((void *)MAC_TXCONSUMEINDEX)) {
		ercd = sig_sem(ic->semid_txb_ready);	/* Tx buffer is not full */
		if (ercd != E_OK) {
			DBG("lpc_start tx sig_sem error:%s!",itron_strerror(ercd));
			NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_TXB_QOVRS], 1);
		}
		sil_wrw_mem((void *)MAC_TXPRODUCEINDEX, idx);
	} else {
		ic->sc->tx_full = true;
		DBG("tx buffer is full");
		/* MAC_TXPRODUCEINDEX should not be updated here, or it will
		 * stop the transfer */
	}
	//ic->timer = TMO_IF_LPC_XMIT;	/* when timeout, lpc_watchdog will be called */

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  if_lpc_handler
 *  Description:  emac interrupt handler
 *	Parameters: 
 *			none
 * 	return value: 
 * 			none
 *  Created:  2011-8-6 by Ren Wei
 * =====================================================================================
 */
void
if_lpc_handler(void)
{
	T_IF_LPC_SOFTC	*lpc;
	T_IF_SOFTC	*ic;
	uint16_t int_status;
	ER	ercd;
	uint_t idx;
	ic = &if_softc;
	lpc = ic->sc;


	/* read the isq, then interrupt will be cleared */
	int_status = sil_rew_mem((void *)MAC_INTSTATUS);


	if (int_status & (INT_RX_DONE | INT_RX_ERR)) {
        sil_wrw_mem((void *)MAC_INTCLEAR, (INT_RX_DONE | INT_RX_ERR)); /* clear RX interrupt */
		/* no matter done or error, signal the receive function */
		if (lpc->rx_empty) {
			/* buffer is not empty */
			lpc->rx_empty = false;
			ercd = isig_sem(ic->semid_rxb_ready);	
			if ( ercd != E_OK) {
				DBG("c:%d,p:%d",sil_rew_mem((void *)MAC_RXCONSUMEINDEX),sil_rew_mem((void *)MAC_RXPRODUCEINDEX));
				DBG("eth_int rx sig_em error:%s", itron_strerror(ercd));
				NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_RXB_QOVRS], 1);
			}
		}
	}

	if (int_status & (INT_TX_DONE | INT_TX_ERR)) {
		sil_wrw_mem((void *)MAC_INTCLEAR, INT_TX_DONE | INT_TX_ERR); /* clear TX interrupt */
		/* no matter done or error, signal the send function */
		/* the status of last sent packet should be check */
		if (lpc->tx_full) {
			//DBG("tx buffer is not full:%x", int_status);
			idx = sil_rew_mem((void *)MAC_TXPRODUCEINDEX);
			if (++idx == NUM_TX_FRAG) {
				idx = 0;
			}
			sil_wrw_mem((void *)MAC_TXPRODUCEINDEX, idx);
			lpc->tx_full = false;
			ercd = isig_sem(ic->semid_txb_ready);	/* Tx buffer is not full */
			if (ercd != E_OK) {
				DBG("eth_int tx sig_sem error:%s!",itron_strerror(ercd));
				NET_COUNT_ETHER_NIC(net_count_ether_nic[NC_ETHER_NIC_TXB_QOVRS], 1);
			}
		}
		//ic->timer = 0;	/* reset watchdog */
	}

	if (int_status & INT_RX_OVERRUN) {
		sil_wrw_mem((void *)MAC_INTCLEAR, INT_RX_OVERRUN);
		DBG("RX is overrun");
		/* soft rest RX */
		sil_wrw_mem((void *)MAC_COMMAND, sil_rew_mem((void *)MAC_COMMAND) | CR_RX_RES);
		sil_wrw_mem((void *)MAC_COMMAND, sil_rew_mem((void *)MAC_COMMAND) | CR_RX_EN);
		sil_wrw_mem((void *)MAC_MAC1, sil_rew_mem((void *)MAC_MAC1) | MAC1_REC_EN);
	}

	if (int_status & INT_TX_UNDERRUN) {
		sil_wrw_mem((void *)MAC_INTCLEAR, INT_TX_UNDERRUN);
		DBG("TX is underrun");
		/* soft reset Tx */
		sil_wrw_mem((void *)MAC_COMMAND, sil_rew_mem((void *)MAC_COMMAND) | CR_TX_RES);
		sil_wrw_mem((void *)MAC_COMMAND, sil_rew_mem((void *)MAC_COMMAND) | CR_TX_EN);
	}

}


