# Kanaan_Lab1 | Let there be light
ESN11 Lab1 (Freddy KANAAN)

# A Introduction
This lab serves as an introduction Co-Design. In this lab, we have implemented a Led chaser using a DE10-Lite board.
The Led chaser was implemented using C language and Quartus.
The implemented 8 bit chaser uses the following controls:
- KEY1 : system reset push button;
- KEY0 : Led chaser start trigger push button;
- 4 switches (SW0 through SW3) : These switches enable control of the LED chaser's speed. In total the LED chaser has 16 control speeds;
- 8 LEDs (LEDR0 through LEDR7).

The *main.c* contains two distinct method of implemeting the LED chaser :
* Polling method : this method is the easier one to implement. However, it is inefficient in terms of performance since the program will be periodically checking the start trigger and switches' state.
* Interrupt method : this method is a bit more complex but is far more efficient as the push button and switches' state are triggered by the hardware.

# B System Architecture
## QSYS components and their interaction 
Since the DE10-Lite incorporates a NiosII processor, it should be used alongside a clock source. We also use a JTAG which can be useful for debugging. Moreover, we need some RAM and 3 PIOs.

The PIO modules are used for the LEDs, switches, and push buttons.
In this LAB the PIOs represent the following :
- PIO0 represents the LEDs. 8 Leds means a width of 8 bits. And since leds are controlled to provide visual feedback, PIO0 should be set as an output;
- PIO1 represents the switches. Since there are 4 switches that can be controlled by the user, PIO1 should have a bit width of 4 bits and should be set as input;
- PIO2 represents the push button KEY0. Since this is a push button that can be either be pressed or not pressed, 1 bit is enough. Furthermore, since the state of this push button is directly controlled by the user, this PIO should be set as input.

I should also note a few more things concering this system :
- PIO1 and PIO2 have the 'Generate IRQ' option ticked. This is done to enable interrupt mode for each;
- 4000 KB of RAM were used to make sure system works without runnning out of memory.

The following screenshot shows our qsys design :
![image](https://user-images.githubusercontent.com/114091388/213661577-cde3af3a-39e1-4a04-b114-edb81f0fd566.png)


## Progress and Results
Polling mode work as intended : we have a LED chaser that goes back and forth indefinetly. We were able to trigger the system with KEY0 and reset it with KEY1. In addition, were able to have the 16 intended speeds that control the LED chaser.

https://drive.google.com/file/d/1GnVSLV1S4aq_zYw0Yclakiim9D6D1Ger/view?usp=share_link

Interrupt method is not working. 

# C Conclusion: 
## What we learned
This lab tough me how to implement the concept of the LED chaser using *C* instead of *VHDL*.
I also learned how to use *Platform Desginer* aswell as *Pin Planner* and generate HAL and BSP. In addition, I learned how to use HAL functions to changes the state some of the PIOs. Furthermore, I learned how to implement ISRs.

## Comment Progress
The implementation of this was once step at a time : first we learned how to take our design on paper to simply toggle a single LED.
As simple as this may be on paper, it remains a challenge as I was not familiar with *Platform Desginer* nor *Pin Planner*.
However, once that technical aspect was mastered, it felt quite simple to go on and implement an 8 bit LED chaser, and then go on to code functionalities such as speed control and start trigger in polling, and interrupt mode in *C*. 

## Pitfalls & Solutions
Pitfalls included simple things such as :
- Setting RAM amount to 4 KB instead of 40 KB (the system will not work), and then testing with large values such as 40 MB (compilation will take a very long time to finish, not to mention that this amount far exceeds our need);
- Forgetting to export the needed pins (which will make pin planner not show any exported pins);
- Generating app, HAL, and BSP files in the same directory which is a bad practice as it can squash your files;
- Forgetting to set some variable to *volatile* type in *main.c*. This will make interrupts not work.
