#include "target_test.h"

/*
 *  各タスクの優先度の定義
 */

#define MAIN_PRIORITY	5		/* メインタスクの優先度 */
								/* HIGH_PRIORITYより高くすること */

#define SPEED_PRIORITY	7		/* 並列に実行されるタスクの優先度 */
#define FOLLOW_PRIORITY		8
#define SENSORS_PRIORITY  9
/*
 *  ターゲットに依存する可能性のある定数の定義
 */

#ifndef TASK_PORTID
#define	TASK_PORTID		1			/* 文字入力するシリアルポートID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */
#endif /* STACK_SIZE */

#ifndef LOOP_REF
#define LOOP_REF		ULONG_C(1000000)	/* 速度計測用のループ回数 */
#endif /* LOOP_REF */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	speed_task(intptr_t exinf);
extern void  follow_task(intptr_t exinf);
extern void	main_task(intptr_t exinf);
extern void	sensors_task(intptr_t exinf);
extern void	cyc_handle(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
