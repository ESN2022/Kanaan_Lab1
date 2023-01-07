/* IRQ method */
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include "system.h"
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"

static void irq_speed_handler(void * context, alt_u32 id)
{ // Instructions to handle IRQ goes here !
	usleep( 1505000 - 100000 * IORD_ALTERA_AVALON_PIO_DATA(PIO_1) );	// sleep time controled by SW0 through SW3
}

void main(void) 
{
	int direction = 1;					// up => 1			down => -1
	alt_u8 led_data=0x01;	// led start position at LEDR0
 alt_irq_register( PIO_1_IRQ, NULL, (void*)irq_speed_handler ); // register the irq 
	
	while(1)
	{
		IORD_ALTERA_AVALON_PIO_DATA(PIO_0, led_data); // turn on led
		if(direction = 1)	// up direction
		{
			if(led_data == 0x80) direction = -1;	// LEDR7 reached
			else 																led_data <<= 1;	
		}
		else													// down direction
		{
			
			if(led_data == 0x01) direction = 1;		// LED0 reached
			else																	led_data >>= 1;
		}
	}
}
