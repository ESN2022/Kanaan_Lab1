#include <stdio.h>
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include <priv/alt_legacy_irq.h>
#include "system.h"
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"
#include "unistd.h"

/**************************************polling method**************************************
int main(void)
{
	char direction = 1;		// up => 1  ---	down => -1
	alt_u8 led_data = 0x01; // led start position at LEDR0
	alt_u8 pb_state = 0;	// push button start trigger flag
	alt_u32 sleep_duration; // sleep duration

	while (1)
	{
		while (pb_state == 0)
		{
			if (IORD_ALTERA_AVALON_PIO_DATA(PIO_2_BASE) == 0x01)
			{
				pb_state == 1;
				break;
			}
		}
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, led_data); // turn on led
		sleep_duration = 5000 * (1 + (alt_u8)IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE));
		if (direction == 1) // up direction
		{
			if (led_data == 0x80)
			{
				direction = -1; // LEDR7 reached
			}
			else
			{
				led_data <<= 1;
			}
		}
		else // down direction
		{
			if (led_data == 0x01)
			{
				direction = 1; // LED0 reached
			}
			else
			{
				led_data >>= 1;
			}
		}
		usleep(sleep_duration); // sleep duration controlled by SW0 through SW3
	}
	return 0;
}
*********************************************************************************************/

/**************************************interrupt method**************************************/
volatile alt_u32 time_delay;
volatile alt_u8 pb_flag; // push button start trigger flag

void LED_Chaser_Start(void)
{
	char direction = 1;		// up => 1  ---	down => -1
	alt_u8 led_data = 0x01; // led start position at LEDR0

	while (1)
	{
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, led_data); // turn on led
		if (direction == 1)								   // up direction
		{
			if (led_data == 0x80)
			{
				direction = -1; // LEDR7 reached
			}
			else
			{
				led_data <<= 1;
			}
		}
		else // down direction
		{

			if (led_data == 0x01)
			{
				direction = 1; // LEDR0 reached
			}
			else
			{
				led_data >>= 1;
			}
		}
		usleep(time_delay);
	}
}

static void irq_speed_handler(void *context, alt_u32 id)
{
	time_delay = 5000 * (1 + (alt_u8)IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE)); // sleep time controlled by SW0 through SW3	// sleep time controled by SW0 through SW3

	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x0);
}

static void irq_start_trigger(void *context, alt_u32 id)
{
	pb_flag = (alt_u8)IORD_ALTERA_AVALON_PIO_DATA(PIO_2_BASE); // sleep time controlled by SW0 through SW3	// sleep time controled by SW0 through SW3

	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0x0);
	LED_Chaser_Start();
}

int main(void)
{
	alt_irq_register(PIO_2_IRQ, NULL, (void *)irq_start_trigger); // register the irq
	alt_irq_register(PIO_1_IRQ, NULL, (void *)irq_speed_handler); // register the irq

	while (1)
	{
	}
	return 0;
}
/**********************************************************************************************/
