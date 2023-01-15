#include <stdio.h>
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include <priv/alt_legacy_irq.h>
#include "system.h"
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"
#include "unistd.h"

/**global variables**/
alt_u32 sleep_duration; // sleep duration
alt_u8 led_data = 0x01; // led start position at LEDR0
char direction = 1;		// up => 1  ---	down => -1
char pb_flag = 0;		// push button start trigger flag.
// 0 -> KEY0 was never not pressed -- 1 -> KEY0 was pressed
volatile alt_u8 switchBits;

/************************************** polling method **************************************
int main(void)
{
	while (1)
	{
		while (pb_flag == 0)
		{
			if (!IORD_ALTERA_AVALON_PIO_DATA(PIO_2_BASE)) // checks whether the KEY0 was ever pressed
			{
				pb_flag = 1; // flag indicating KEY0 was pressed
				break;
			}
		}
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, led_data); // turn on led
		switchBits = (alt_u8)~IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE);
		switchBits &= 0x0F;
		sleep_duration = 5000 * (1 + switchBits);
		usleep(sleep_duration); // sleep duration controlled by SW0 through SW3
		if (direction == 1)		// up direction
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
	}
	return 0;
}
/***********************************************************************************************/

/************************************** Interrupt method **************************************/
void LED_Chaser_Start(void)
{
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, led_data); // turn on led
	usleep(sleep_duration);
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
			direction = 1; // LEDR0 reached
		}
		else
		{
			led_data >>= 1;
		}
	}
}

static void irq_start_trigger(void *context, alt_u32 id)
{
	switchBits = (alt_u8)~IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE);
	switchBits &= 0x0F;
	sleep_duration = 5000 * (1 + switchBits);
	pb_flag = 1;
	// Reset the edge capture register //
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0);
}

static void irq_speed_handler(void *context, alt_u32 id)
{
	switchBits = (alt_u8)~IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE);
	switchBits &= 0x0F;
	sleep_duration = 5000 * (1 + switchBits);

	// Reset the edge capture register //
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0);
}

void IRQs_init(void)
{
	// IRQ masks and EDGE CAP //
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_2_BASE, 0x01);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0x0F);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0);

	alt_irq_register(PIO_2_IRQ, NULL, (void *)irq_start_trigger); // register the irq
	alt_irq_register(PIO_1_IRQ, NULL, (void *)irq_speed_handler); // register the irq
}

int main(void)
{
	IRQs_init();
	while (1)
	{
		if (pb_flag)
		{
			LED_Chaser_Start();
		}
	}
	return 0;
}
/**********************************************************************************************/
