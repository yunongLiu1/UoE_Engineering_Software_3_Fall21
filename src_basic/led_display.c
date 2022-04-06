/*
*
* File Name: led_display.c
* Project Name: Traffic Light
* Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA)
* Tool Version: Xilinx SDK 2015.2
* Name: Yunong Liu
* Company: University of Edinburgh
* Creation Date and Time: 20/11/2021; 07:23
* Description: This source file contains functions used to display LEDs, the led_states() function used
*  to make all address of leds together to get the final led address (Using logic OR) and the other one
*  used to display the leds of current states store in the road structure. All these functions are declared
*  in the led_display.h header file.
*/

#include "led_display.h" //Added for definitions of variable and constants, and function prototypes.
#include "vga_display.h"	//Added for definiton of the character of colours and corresponding colour codes, 

/*
 * This function is used to display the leds of current states
 * store in the road structure.
 * */
void display_leds(u8 ped_btn, struct road* roads, u8 numOfRoads,struct pedestrianLight* ped_light){
	u16 led_out = 0;
	u8 i = 0;
	for ( i= 0; i < numOfRoads ; i++){
		led_out = led_out | led_states(roads[i]); //Get led address for all roads
	}
	if (ped_btn) {
		led_out = led_out | ped_light->light_led_address;//Get led address for pedestrian light
	}

	XGpio_DiscreteWrite(&LEDS_OUT, 1, led_out); //Write LEDs

}

/*
 * This function is a helper function to display_leds() which
 * used to make all address of leds together to get the final
 * led address (Using logic OR)
 * */
u16 led_states(struct road rd) {
	u16 states = 0;
	if (rd.states_sequence[rd.curr_state_index].yellow_light_state == YELLOW) { //If the yellow light is active
		states = states | rd.yellow_light_led_address;
	}
	if (rd.states_sequence[rd.curr_state_index].red_light_state == RED) { //If the red light is active
		states = states | rd.red_light_led_address;
	}
	if (rd.states_sequence[rd.curr_state_index].green_light_state == GREEN) { //If the green light is active
		states = states | rd.green_light_led_address;
	}
	return states;
}
