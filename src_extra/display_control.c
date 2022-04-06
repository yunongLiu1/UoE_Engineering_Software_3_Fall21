/*
* File Name: vga_display.h
* Project Name: Traffic Light
* Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA)
* Tool Version: Xilinx SDK 2015.2
* Name: Yunong Liu
* Company: University of Edinburgh
* Creation Date and Time: 20/11/2021; 07:23
* Description: This source file contains functions used to drive the 7 segment display. 
* Therearethreefunctionsinthisfile. ThedisplayNumber()functionreceivesan unsigned16-bitinteger. 
* Itisusedtoassignthedigitnumberandthevaluetobedisplayed perdigitwhenthetimerinterruptoccurs. 
* ThecalculateDigits()functionisusedtoextract the digits (of which a maximum of four can be displayed) 
* from the number to be displayed. The hwTimerISR() (which is defined in the xinterruptES3.c file) method 
* is used to call the displayDigit() method, which selects the segments and displays the digits on the 
* 7-segment display. All these functions are declared in the seg7_display.h header file.
*/

#include "display_control.h" //Added for definitions of variable and function prototypes
#include "gpio_init.h" // Added for XGpio type definitions
#include "seg7_display.h"	//Added for displayRoadNumberAndTimer() used to display time
#include "vga_display.h"	//Added for definiton of the character of colours and corresponding colour codes, 
							//region_select, colour_select, blinkingGreenLight	

#include "led_display.h"
#include "timer_interrupt_func.h"
#include "traffic_lights.h"

/****************Extra features:******************/
void checkYellowFlashing(u8* light_var_ct,u8* pd_var_ct,struct road* roads,struct pedestrianLight* ped_light,u8 ped_btn);
void checkPause(u8 rd_num,s8 ms);
void checkAllRed(struct road* roads,struct pedestrianLight* ped_light);
void checkChangeNextSpeed(u8* light_var_ct,u8* pd_var_ct);
/**************************************************/

/*
 * VGA, 7seg and led display when pedestrian light
 * is active.
 *
 * */
void pd_cross(u8* light_var_ct,u8* pd_var_ct,struct road* roads, struct pedestrianLight* ped_light) {
	s8 	ms = pd_counter * 10; // multiply by 10 to get counter in ms
	XGpio region= region_select(ped_light->light_vga_address);//get the vga region of pedestrian light
	char colour = GREEN; //initialise pedestrain colour to green
	// Change VGA display to the pedestrain mode, which display red to all roads and green to pedestrian light
	XGpio_DiscreteWrite(&LEDS_OUT, 1, PED_CROSS_LEDS);
	/* Display counter on the FPGA board in ms, and refresh pedestrian light(served for blinking)*/
	for (ms = pd_counter * 10; ms > 0; ms--) {
		XGpio_DiscreteWrite(&region, 1, colour_select(colour));//refresh pedestrian light(served for blinking)
		while (!timer_served) {
			//Display counter on the FPGA board in ms with decimal point
			displayRoadNumberAndTimer('p', pd_counter * 10 + ms % 10, TRUE);

			/****************Extra features:******************/
			checkPause('p',pd_counter * 10 + ms % 10);
			checkAllRed(roads, ped_light);
			checkChangeNextSpeed(light_var_ct,pd_var_ct);

			/************************************************/
		}
		timer_served = FALSE;//Reset to count for the next 0.1 second
		//If 1s passed, decrease the counter of pedestrian light remain seconds
		if (ms % 10 == 0) {pd_counter--;}
		//If remain time <= 2 second, blinking
		if (ms % 2 == 0 && ms < 20) {blinkingGreenLight(&colour);}//Implemented in traffic_lights.c
	}
	/*Reset pedestrian light to red and pedestrian button to FALSE*/
	colour = RED;
	XGpio_DiscreteWrite(&region, 1, colour_select(colour));
	ped_btn = FALSE;
}
/*
 * The main loop for vga, 7seg and led display,
 *
 * */
void roads_loop(u8* light_var_ct,u8* pd_var_ct, struct road* roads, u8 rd_num,struct pedestrianLight* ped_light) {
	s8 ms;
	//While not reach the end of light's states sequence for current road roads[rd_num],
	//move to the next light's state
	while (roads[rd_num].curr_state_index < len_of_states_array) {
		//This function is defined in traffic_light.c, which displays current light state
		traffic_light_loop(roads[rd_num]);
		//Decrease the seg-7 display for every 0.1s
		for (ms = 10*light_counter; ms > 0; ms--) {
			//While not reach 0.1s, keep updating pedestrian button status,
			//displaying led states and display the remain time of this light state
			while (!timer_served) {
				/****************Extra features:******************/
				checkYellowFlashing(light_var_ct,pd_var_ct,roads,ped_light,ped_btn);
				checkPause(rd_num,ms);
				checkAllRed(roads, ped_light);
				checkChangeNextSpeed(light_var_ct,pd_var_ct);

				/************************************************/
				ped_btn = XGpio_DiscreteRead(&P_BTN_U, 1) || ped_btn; //update_ped_btn
				display_leds(ped_btn, roads, num_of_roads,ped_light); // Defined in led_display.c
				displayRoadNumberAndTimer(rd_num + 1, ms, TRUE);//display in ms with decimal point
			}
			timer_served = FALSE;//after reach 0.1s, reset timer_served and move to the next loop
		}
		//after reach 1s, move to the next state for this road
		roads[rd_num].curr_state_index = roads[rd_num].curr_state_index + 1;
	}
	//after reach the end of state sequence for this road, reset state, and end this function.
	roads[rd_num].curr_state_index = 0;
}

void checkYellowFlashing(u8* light_var_ct,u8* pd_var_ct,struct road* roads,struct pedestrianLight* ped_light,u8 ped_btn){
	if(XGpio_DiscreteRead(&SLIDE_SWICHES, 1)==1 && ped_btn){
		init_traffic_colour(roads, 3 , ped_light); //Define in traffic_lights.c, set all to red
		pd_counter = 3;
		pd_cross(light_var_ct, pd_var_ct, roads,ped_light);
		pd_counter = 5;
	}

}

void checkPause(u8 rd_num,s8 ms){
	while(XGpio_DiscreteRead(&SLIDE_SWICHES, 1)==2){
		if(rd_num == 'p'){
			displayRoadNumberAndTimer(rd_num, ms, TRUE);//display in ms with decimal point
		}else{
			displayRoadNumberAndTimer(rd_num + 1, ms, TRUE);//display in ms with decimal point
		}
	}
}

void checkAllRed(struct road* roads,struct pedestrianLight* ped_light){
	while(XGpio_DiscreteRead(&SLIDE_SWICHES, 1)==4){
		init_traffic_colour(roads, 3 , ped_light);
		displayRoadNumberAndTimer(NUMBER_DASH, 100, FALSE);//100 is greater than 99, thus display all dash
	}
}

void checkChangeNextSpeed(u8* light_var_ct, u8* pd_var_ct){
	if(XGpio_DiscreteRead(&SLIDE_SWICHES, 1)==0b1000000000000000){
		u8 pd_ct = 0;
		u8 light_ct = 0;
		while(!XGpio_DiscreteRead(&P_BTN_R,1)){
			displayRoadNumberAndTimer(NUMBER_DASH, 100, FALSE);//100 is greater than 99, thus display all dash
			u16 slideIn = XGpio_DiscreteRead(&SLIDE_SWICHES, 1);
			light_ct = (slideIn&0b0111100000000000)>>11;
			pd_ct =(slideIn&0b0000011110000000)>>7;
		}
		if(pd_ct&&light_ct){//not zero
			*pd_var_ct = pd_ct;
			*light_var_ct = light_ct;
		}

	}
}
