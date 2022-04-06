/*
*
* File Name: display_control.h
* Project Name: Traffic Light
* Target Device/Platform: Basys3 Board (with Microblaze processor on the Artix-7 FPGA)
* Tool Version: Xilinx SDK 2015.2
* Name: Yunong Liu
* Company: University of Edinburgh
* Creation Date and Time: 20/11/2021; 07:23
* Description: This header file contains all functions declarations in display_control.c source
* file and also the definition of variables used to control traffic lights and pedestrian lights.
*/

#ifndef DISPLAY_CONTROL_H_
#define DISPLAY_CONTROL_H_

/******************* Include Header Files ******************************/
#include "xil_types.h"		// Added for integer type definitions
#include "traffic_lights.h" // Added for struct road and struct pedestrianLight

/******************* Variables used to control the interrupt system ******************************/
u8 len_of_states_array; //integer used to store the length of states, for example,
                        //in this coursework it should be 4 for {R, RY, Y, G}
u8 num_of_roads; //integer used to store the length of roads, in this coursework it should be 3 
u8 ped_btn; //Boolean of whether the pedestrian button has been pressed
u8 pd_counter;// Seconds for pedestrian light.
u8 light_counter;// Seconds for each traffic light.

/************************** Function Prototypes ******************************/
void roads_loop(struct road* roads, u8 rd_num,struct pedestrianLight* ped_light);//Defined in display_control.c
void pd_cross(struct pedestrianLight* ped_light);//Defined in display_control.c

#endif /* DISPLAY_CONTROL_H_ */

