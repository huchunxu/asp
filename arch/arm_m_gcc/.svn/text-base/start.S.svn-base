/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  惧淡螟侯涪荚は・笆布の(1)×(4)の掘凤を塔たす眷圭に嘎り・塑ソフトウェ
 *  ア∈塑ソフトウェアを猖恃したものを崔むˉ笆布票じ∷を蝗脱ˇ剩澜ˇ猖
 *  恃ˇ浩芹邵∈笆布・网脱と钙ぶ∷することを痰浸で钓满するˉ
 *  (1) 塑ソフトウェアをソ〖スコ〖ドの妨で网脱する眷圭には・惧淡の螟侯
 *      涪山绩・この网脱掘凤および布淡の痰瘦沮惮年が・そのままの妨でソ〖
 *      スコ〖ド面に崔まれていることˉ
 *  (2) 塑ソフトウェアを・ライブラリ妨及など・戮のソフトウェア倡券に蝗
 *      脱できる妨で浩芹邵する眷圭には・浩芹邵に燃うドキュメント∈网脱
 *      荚マニュアルなど∷に・惧淡の螟侯涪山绩・この网脱掘凤および布淡
 *      の痰瘦沮惮年を非很することˉ
 *  (3) 塑ソフトウェアを・怠达に寥み哈むなど・戮のソフトウェア倡券に蝗
 *      脱できない妨で浩芹邵する眷圭には・肌のいずれかの掘凤を塔たすこ
 *      とˉ
 *    (a) 浩芹邵に燃うドキュメント∈网脱荚マニュアルなど∷に・惧淡の螟
 *        侯涪山绩・この网脱掘凤および布淡の痰瘦沮惮年を非很することˉ
 *    (b) 浩芹邵の妨轮を・侍に年める数恕によって・TOPPERSプロジェクトに
 *        鼠桂することˉ
 *  (4) 塑ソフトウェアの网脱により木儡弄または粗儡弄に栏じるいかなる禄
 *      巢からも・惧淡螟侯涪荚およびTOPPERSプロジェクトを倘勒することˉ
 *      また・塑ソフトウェアのユ〖ザまたはエンドユ〖ザからのいかなる妄
 *      统に答づく懒滇からも・惧淡螟侯涪荚およびTOPPERSプロジェクトを
 *      倘勒することˉ
 *
 *  塑ソフトウェアは・痰瘦沮で捏丁されているものであるˉ惧淡螟侯涪荚お
 *  よびTOPPERSプロジェクトは・塑ソフトウェアに簇して・泼年の蝗脱誊弄
 *  に滦する努圭拉も崔めて・いかなる瘦沮も乖わないˉまた・塑ソフトウェ
 *  アの网脱により木儡弄または粗儡弄に栏じたいかなる禄巢に簇しても・そ
 *  の勒扦を砷わないˉ
 *
 *  @(#) $Id: start.S 1304 2008-08-27 07:28:36Z ertl-honda $
 */

/*
 *   カ〖ネル脱のスタ〖トアップモジュ〖ル(ARM-M脱)
 */
#define TOPPERS_MACRO_ONLY
#define INIT_MSP
#define UINT_C(val)		(val)		/* uint_t房の年眶を侯るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t房の年眶を侯るマクロ */
#define CAST(type, val)	(val)		/* 房キャストを乖うマクロ */

#include "kernel_impl.h"
#include "arm_m.h"

    .section .flash_protect, "a"
    .global init_vector
    .code 16
    .syntax unified
 init_vector:
    .word   0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE

/*
 *  スタ〖トアップル〖チン
 *
 *  Threadモ〖ドで钙び叫されることを涟捏としてるˉ
 */
	.text
	.align 2
	.code 16
	.syntax unified
	.globl _start
	.type _start, function
_start:
	cpsid f                 /* 充哈みロック觉轮へ */

#ifdef INIT_MSP
	/*
	 * MSPの肋年
	 */
	mov   r0, #CONTROL_MSP  /* MSPを铜跟に  */
	msr   control, r0
	isb                     /* control の拎侯稿に涩妥 */
	ldr  r0,=_kernel_istkpt
	ldr  r1,[r0]
	msr  msp, r1
#endif /* INIT_MSP */

	/*
	 * hardware_init_hookの钙び叫し ∈0でない眷圭∷
	 *
	 * タ〖ゲットハ〖ドウェアに巴赂して涩妥な介袋步借妄がある眷圭
	 * は・hardware_init_hook という簇眶を脱罢する
	 */
	ldr  r0, =hardware_init_hook
	cbz  r0, start_1
	blx  r0

start_1:
#ifndef OMIT_BSS_INIT
	/*
	 *  BSSセクション介袋步
	 */
	mov  r0, #0
	ldr  r1, =__bss_start
	ldr  r2, =__bss_end
	cmp  r1, r2
	bhs  start_3
start_2:
	str  r0, [r1]
	add  r1,r1, #4
	cmp  r1, r2
	blo  start_2
#endif /* OMIT_BSS_INIT */


start_3:

#ifndef OMIT_DATA_INIT
	/*
	 *  DATAセクション介袋步
	 *  __idata_start から __idata_end までを・__data_start 笆惯に
	 *  コピ〖する
	 */
	ldr  r1, =__idata_start
	ldr  r2, =__idata_end
	cmp  r1, r2
	bhs  start_5
	ldr  r3, =__data_start
start_4:
	ldr  r0, [r1]
	str  r0, [r3]
	add  r1, #4
	add  r3, #4
	cmp  r1, r2
	blo  start_4
#endif /* OMIT_DATA_INIT */

	/*
	 *  software_init_hook を钙叫し∈0 でない眷圭∷
	 *
	 *  ソフトウェア茨董∈泼にライブラリ∷に巴赂して涩妥な介袋步借
	 *  妄がある眷圭は・software_init_hook という簇眶を脱罢すれば
	 *  よいˉ
	 */
start_5:
	ldr  r0, =software_init_hook
	cbz  r0, start_6
	blx  r0

start_6:
	bl   sta_ker

l_end:
	b   l_end
