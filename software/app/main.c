/* polling method */
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "unistd.h"

int main(void)
{
	int time_delay;
    int direction = 1;					// up => 1			down => -1
    alt_u8 led_data = 0x01;	// led start position at LEDR0

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
        else													// down direction
        {

            if(led_data == 0x01)
            {
                direction = 1;		// LED0 reached
            }
            else
			{
				led_data >>= 1;
			}
        }
		
		switch( IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE) )
		{
			case 1 :
				time_delay = 100000;
				break;       
			case 2 :         
				time_delay = 200000;
				break;       
			case 3 :         
				time_delay = 300000;
				break;       
			case 4 :         
				time_delay = 400000;
				break;       
			case 5 :         
				time_delay = 500000;
				break;       
			case 6 :         
				time_delay = 600000;
				break;       
			case 7 :         
				time_delay = 700000;
				break;       
			case 8 :         
				time_delay = 800000;
				break;       
			case 9 :         
				time_delay = 900000;
				break;       
			case 10 :        
				time_delay = 1000000;
				break;       
			case 11 :        
				time_delay = 1100000;
				break;       
			case 12 :        
				time_delay = 1200000;
				break;       
			case 13 :        
				time_delay = 1300000;
				break;       
			case 14 :        
				time_delay = 1400000;
				break;       
			case 15 :        
				time_delay = 1500000;
				break;       
			case 16 :        
				time_delay = 1600000;
				break;
			default: time_delay = 0;
		}
        usleep(1605000 - time_delay);	// sleep time controlled by SW0 through SW3
    }
	return 0;
}
