/* polling method */
#include "system.h"
#include "altera_avalon_pio_regs.h"

void main(void)
{
	int direction = 1;					// up => 1			down => -1
	alt_u8 led_data=0x01;	// led start position at LEDR0
	
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
		usleep( 1505000 - 100000 * IORD_ALTERA_AVALON_PIO_DATA(PIO_1) );	// sleep time controled by SW0 through SW3
	}
}
