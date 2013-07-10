#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test_laser_car.h"
#include "light.h"
#include "switch.h"
#include "speaker.h"
#include "motor.h"
#include "steer.h"
#include "decoder.h"
#include "getLaserInfo.h"


void task1(intptr_t exinf)
{
	uint8_t switch_temp = 0;

	while(1)
	{
		switch_temp = switch_read_8bit(SWITCH_GROUP0);

		light_close_some(0xff);
		light_open_some(switch_temp);

    	motor_output(MOTOR0, switch_temp * 10);
    	steer_output_value(STEER_DIR, switch_temp);
    	steer_output_value(STEER_SERVO, switch_temp);

        slp_tsk();
	}
}

void task2(intptr_t exinf)
{
	volatile uint32_t decoder_temp = 0;

    while(1)
    {
		decoder_temp = decoder_read(DECODER);

		getLaserInfo();

		if((switch_read(SWITCH_START0)) || (switch_read(SWITCH_START1)))
			speaker_open(SPEAKER0);
		else
			speaker_close(SPEAKER0);

		wup_tsk(TASK1);
    }
}
