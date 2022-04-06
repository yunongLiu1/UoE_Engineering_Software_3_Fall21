/*
*
* File Name: led_display.h
* Project Name: Traffic Light
* Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA)
* Tool Version: Xilinx SDK 2015.2
* Name: Yunong Liu
* Company: University of Edinburgh
* Creation Date and Time: 20/11/2021; 07:23
* Description: This header file contains all function declarations in the source file.
*/

#ifndef LED_DISPLAY_H_
#define LED_DISPLAY_H_

/******************* Include Header Files ******************************/
#include "xil_types.h"		// Added for integer type definitions
#include "gpio_init.h"		// Added for XGpio type definitions
#include "traffic_lights.h" // Added for struct road

/************************** Function Prototypes ******************************/
u16 led_states(struct road rd); //Function defined in led_display.c which used to get the led states(16bits) from road
//Function defined in led_display.c which used to display the state of road on leds.
void display_leds(u8 ped_btn, struct road* roads, u8 numOfRoads,struct pedestrianLight* ped_light);

#endif /* LED_DISPLAY_H_ */
