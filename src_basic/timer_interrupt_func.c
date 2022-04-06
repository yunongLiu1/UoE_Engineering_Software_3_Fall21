/*
*
* File Name: timer_interrupt_func.h
* Project Name: Traffic Light
* Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA)
* Tool Version: Xilinx SDK 2015.2
* Name: Yunong Liu
* Company: University of Edinburgh
* Creation Date and Time: 20/11/2021; 07:23
* Description: This source file contains functions used to
* All these functions are declared in the header file.
*/
#include "seg7_display.h"	//Added for displayDigit() used to display number at interrupt time
#include "timer_interrupt_func.h" //Added for definitions of variable and constants, and function prototypes.


void hwTimerISR(void *CallbackRef) {
	timer();
	displayDigit(); //Defined in seg7-display.c
}

/*
 * This function is used to count time, will be called by hwTimerISR(void *CallbackRef) at each interrupt*/
void timer(){
	interruptCounter++;
	if (interruptCounter == REQUIRED_TIME/INTERRUPT_SYSTEM_TIME) { //0.1s
		interruptCounter = 0; //Reset counter
		timer_served = TRUE; //contain information of timer served or not
	}

}
