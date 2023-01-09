/* IRQ method */
#include <stdio.h>
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include "system.h"
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"
#include "unistd.h"
#include <priv/alt_legacy_irq.h>

volatile alt_u32 time_delay;
static void irq_speed_handler(void * context, alt_u32 id)
{ // Instructions to handle IRQ goes here !
    time_delay = 20000 * (alt_u8)IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE);     	// sleep time controlled by SW0 through SW3	// sleep time controled by SW0 through SW3
}

int main(void) 
{
	int direction = 1;					// up => 1			down => -1
	alt_u8 led_data = 0x01;	// led start position at LEDR0
 alt_irq_register( PIO_1_IRQ, NULL, (void*)irq_speed_handler ); // register the irq 
	
	while(1)
	{
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, led_data); // turn on led
		if(direction == 1)	// up direction
		{
			if(led_data == 0x80)
			{
				direction = -1;	// LEDR7 reached
			}
			else
			{
				led_data <<= 1;
			}				
		}
		else						// down direction
		{
			
			if(led_data == 0x01)
			{
				direction = 1;		// LEDR0 reached
			}
			else
			{
				led_data >>= 1;
			}
		}
		usleep(320500 - time_delay);
	}
	return 0;
}
