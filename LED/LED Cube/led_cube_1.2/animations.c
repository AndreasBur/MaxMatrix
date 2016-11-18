/*
fornax LED cube V1.1

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include "ab.h"
#include "ledcube.h"
#include "animations.h"
#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "rotation.h"
#include "translation.h"



#define WALKING_DELAY 8

/* small guy who walks in through a door a few steps,
 * after that the environment starts moving for a while
 * I figured out it looks better if the guy heads for 
 * the front, so every frame is rotated by 270 deg. I 
 * was just too lazy to change everything. 
 */

void cube_walking_frame_static0( void )
{
	cube_clear();
	cube[0][0] |= 0b10000000;
	cube[0][1] |= 0b10000001;
	cube[0][2] |= 0b00000001;
	cube[0][3] |= 0b10000000;
	cube[0][4] |= 0b10000000;
	cube[0][5] |= 0b10000001;
	cube[0][6] |= 0b00000001;
	cube[0][7] |= 0b10000000;

	cube[1][0] |= 0b10000000;
	cube[2][0] |= 0b10000000;
	cube[3][0] |= 0b10000000;
	cube[4][0] |= 0b10000000;
	cube[5][0] |= 0b10000000;
	cube[6][0] |= 0b10000000;
	cube[7][0] |= 0b11000000;
	
	cube[1][4] |= 0b10000000;
	cube[2][4] |= 0b10000000;
	cube[3][4] |= 0b10000000;
	cube[4][4] |= 0b10000000;
	cube[5][4] |= 0b10000000;
	cube[6][4] |= 0b10000000;
	cube[7][4] |= 0b11000000;

	cube[1][1] |= 0b00000001;
	cube[1][2] |= 0b00000001;
	cube[1][5] |= 0b00000001;
	cube[1][6] |= 0b00000001;
}

void cube_walking_frame_static1( void )
{
	cube_clear();
	cube[0][0] |= 0b10000000;
	cube[0][1] |= 0b10000000;
	cube[0][2] |= 0b10000001;
	cube[0][3] |= 0b00000001;
	cube[0][4] |= 0b10000000;
	cube[0][5] |= 0b10000000;
	cube[0][6] |= 0b10000001;
	cube[0][7] |= 0b00000001;

	cube[1][1] |= 0b10000000;
	cube[2][1] |= 0b10000000;
	cube[3][1] |= 0b10000000;
	cube[4][1] |= 0b10000000;
	cube[5][1] |= 0b10000000;
	cube[6][1] |= 0b10000000;
	cube[7][1] |= 0b11000000;
	
	cube[1][5] |= 0b10000000;
	cube[2][5] |= 0b10000000;
	cube[3][5] |= 0b10000000;
	cube[4][5] |= 0b10000000;
	cube[5][5] |= 0b10000000;
	cube[6][5] |= 0b10000000;
	cube[7][5] |= 0b11000000;

	cube[1][2] |= 0b00000001;
	cube[1][3] |= 0b00000001;
	cube[1][6] |= 0b00000001;
	cube[1][7] |= 0b00000001;
}

void cube_walking_frame_static2( void )
{
	cube_clear();
	cube[0][0] |= 0b00000001;
	cube[0][1] |= 0b10000000;
	cube[0][2] |= 0b10000000;
	cube[0][3] |= 0b10000001;
	cube[0][4] |= 0b00000001;
	cube[0][5] |= 0b10000000;
	cube[0][6] |= 0b10000000;
	cube[0][7] |= 0b10000001;

	cube[1][2] |= 0b10000000;
	cube[2][2] |= 0b10000000;
	cube[3][2] |= 0b10000000;
	cube[4][2] |= 0b10000000;
	cube[5][2] |= 0b10000000;
	cube[6][2] |= 0b10000000;
	cube[7][2] |= 0b11000000;
	
	cube[1][6] |= 0b10000000;
	cube[2][6] |= 0b10000000;
	cube[3][6] |= 0b10000000;
	cube[4][6] |= 0b10000000;
	cube[5][6] |= 0b10000000;
	cube[6][6] |= 0b10000000;
	cube[7][6] |= 0b11000000;

	cube[1][3] |= 0b00000001;
	cube[1][4] |= 0b00000001;
	cube[1][7] |= 0b00000001;
	cube[1][0] |= 0b00000001;
}

void cube_walking_frame_static3( void )
{
	cube_clear();
	cube[0][0] |= 0b10000001;
	cube[0][1] |= 0b00000001;
	cube[0][2] |= 0b10000000;
	cube[0][3] |= 0b10000000;
	cube[0][4] |= 0b10000001;
	cube[0][5] |= 0b00000001;
	cube[0][6] |= 0b10000000;
	cube[0][7] |= 0b10000000;

	cube[1][3] |= 0b10000000;
	cube[2][3] |= 0b10000000;
	cube[3][3] |= 0b10000000;
	cube[4][3] |= 0b10000000;
	cube[5][3] |= 0b10000000;
	cube[6][3] |= 0b10000000;
	cube[7][3] |= 0b11000000;
	
	cube[1][7] |= 0b10000000;
	cube[2][7] |= 0b10000000;
	cube[3][7] |= 0b10000000;
	cube[4][7] |= 0b10000000;
	cube[5][7] |= 0b10000000;
	cube[6][7] |= 0b10000000;
	cube[7][7] |= 0b11000000;

	cube[1][4] |= 0b00000001;
	cube[1][5] |= 0b00000001;
	cube[1][0] |= 0b00000001;
	cube[1][1] |= 0b00000001;
}

void cube_walking_frame_door0_open( void )
{
	cube_walking_frame_static0();
	cube[0][7] |= 0b11000011;
	cube[1][7] |= 0b11000011;
	cube[2][7] |= 0b11000011;
	cube[3][7] |= 0b11000011;
	cube[4][7] |= 0b11000011;
	cube[5][7] |= 0b11000011;
	cube[6][7] |= 0b11000011;
	cube[7][7] |= 0b11000011;

	cube[0][6] |= 0b01000000;
	cube[1][6] |= 0b01000000;
	cube[2][6] |= 0b01000000;
	cube[3][6] |= 0b01000000;
	cube[4][6] |= 0b01000000;
	cube[5][6] |= 0b01000000;
	cube[6][6] |= 0b01000000;
	cube[7][6] |= 0b01000000;

	cube[0][5] |= 0b01000000;
	cube[1][5] |= 0b01000000;
	cube[2][5] |= 0b01000000;
	cube[3][5] |= 0b01000000;
	cube[4][5] |= 0b01000000;
	cube[5][5] |= 0b01000000;
	cube[6][5] |= 0b01000000;
	cube[7][5] |= 0b01000000;
	
	cube[0][4] |= 0b01000000;
	cube[1][4] |= 0b01000000;
	cube[2][4] |= 0b01000000;
	cube[3][4] |= 0b01000000;
	cube[4][4] |= 0b01000000;
	cube[5][4] |= 0b01000000;
	cube[6][4] |= 0b01000000;
	cube[7][4] |= 0b01000000;
	
	cube[0][3] |= 0b01000000;
	cube[1][3] |= 0b01000000;
	cube[2][3] |= 0b01000000;
	cube[3][3] |= 0b01000000;
	cube[4][3] |= 0b01000000;
	cube[5][3] |= 0b01000000;
	cube[6][3] |= 0b01000000;
	cube[7][3] |= 0b01000000;
}

// do not clear cube, just add three door frames
void cube_walking_frame_door0( void )
{
	cube_walking_frame_static0();
	
	cube[0][7] |= 0b11111111;
	cube[1][7] |= 0b11111111;
	cube[2][7] |= 0b11111111;
	cube[3][7] |= 0b11111111;
	cube[4][7] |= 0b11111111;
	cube[5][7] |= 0b11111111;
	cube[6][7] |= 0b11111111;
	cube[7][7] |= 0b11111111;
	
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY*10);

	cube_walking_frame_static0();
	cube[0][7] |= 0b11000011;
	cube[1][7] |= 0b11000011;
	cube[2][7] |= 0b11000011;
	cube[3][7] |= 0b11000011;
	cube[4][7] |= 0b11000011;
	cube[5][7] |= 0b11000011;
	cube[6][7] |= 0b11000011;
	cube[7][7] |= 0b11000011;

	cube[0][6] |= 0b00100000;
	cube[1][6] |= 0b00100000;
	cube[2][6] |= 0b00100000;
	cube[3][6] |= 0b00100000;
	cube[4][6] |= 0b00100000;
	cube[5][6] |= 0b00100000;
	cube[6][6] |= 0b00100000;
	cube[7][6] |= 0b00100000;

	cube[0][5] |= 0b00010000;
	cube[1][5] |= 0b00010000;
	cube[2][5] |= 0b00010000;
	cube[3][5] |= 0b00010000;
	cube[4][5] |= 0b00010000;
	cube[5][5] |= 0b00010000;
	cube[6][5] |= 0b00010000;
	cube[7][5] |= 0b00010000;
	
	cube[0][4] |= 0b00001000;
	cube[1][4] |= 0b00001000;
	cube[2][4] |= 0b00001000;
	cube[3][4] |= 0b00001000;
	cube[4][4] |= 0b00001000;
	cube[5][4] |= 0b00001000;
	cube[6][4] |= 0b00001000;
	cube[7][4] |= 0b00001000;
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY*2);

	/* this part is needed more than once */
	cube_walking_frame_door0_open();	
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY*2);

/* this is where the guy walks in */
	cube_walking_frame_door0_open();
	cube[0][6] |= 0b00010000;
	cube[1][7] |= 0b00010000;
	cube[3][7] |= 0b00000100;
	cube[6][7] |= 0b00011000;
	
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);
	cube_walking_frame_door0_open();
	cube[0][7] |= 0b00011000;

	cube[1][7] |= 0b00011000;
	cube[2][7] |= 0b00111100;
	cube[3][7] |= 0b00111100;
	cube[4][7] |= 0b00111100;
	
	cube[5][7] |= 0b00011000;
	
	cube[6][7] |= 0b00011000;
	cube[6][7] |= 0b00011000;
	cube[6][7] |= 0b00011000;

	cube[7][7] |= 0b00011000;

	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);
	


	cube_walking_frame_door0_open();

	cube[0][4] |= 0b00001000;

	cube[1][5] |= 0b00001000;
	cube[1][7] |= 0b00010000;

	cube[2][4] |= 0b00100000;
	cube[2][6] |= 0b00011000;
/*
 * uncomment this for a "spraining" arm
 *	cube[3][2] = 0b00100000;	
 */
	cube[3][5] |= 0b00100000;
	cube[3][6] |= 0b00011000;
	cube[3][7] |= 0b00000100;
	
	cube[4][6] |= 0b00111100;
	
	cube[5][6] |= 0b00011000;
	
	cube[6][5] |= 0b00011000;
	cube[6][6] |= 0b00011000;
	cube[6][7] |= 0b00011000;

	cube[7][6] |= 0b00011000;	
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);

	cube_walking_frame_door0_open();
	cube[0][5] |= 0b00011000;


	cube[1][5] |= 0b00011000;
	cube[2][5] |= 0b00111100;
	cube[3][5] |= 0b00111100;
	cube[4][5] |= 0b00111100;
	
	cube[5][5] |= 0b00011000;
	
	cube[6][4] |= 0b00011000;
	cube[6][5] |= 0b00011000;
	cube[6][6] |= 0b00011000;

	cube[7][5] |= 0b00011000;
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);


	cube_walking_frame_door0_open();
	cube_walking_frame0();	
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
/*
 * at this point the environment starts moving instead
 * of the guy himself 
 * we use a small fade in where the door frames are added
 */
	cube_walking_frame_static1();
	cube_walking_frame1();
	for (uint8_t layer = 0; layer < 8; layer++ ){
		cube[layer][4] |= 0b01000000;	
		cube[layer][5] |= 0b01000000;
		cube[layer][6] |= 0b01000000;
		cube[layer][7] |= 0b01000000;
	}
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);
	
	cube_walking_frame_static2();
	cube_walking_frame2();
	for (uint8_t layer = 0; layer < 8; layer++ ){
		cube[layer][5] |= 0b01000000;
		cube[layer][6] |= 0b01000000;
		cube[layer][7] |= 0b01000000;
	}
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);

	cube_walking_frame_static3();
	cube_walking_frame3();
	for (uint8_t layer = 0; layer < 8; layer++ ){
		cube[layer][6] |= 0b01000000;
		cube[layer][7] |= 0b01000000;
	}
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);

	cube_walking_frame_static0();
	cube_walking_frame0();
	for (uint8_t layer = 0; layer < 8; layer++ ){
		cube[layer][7] |= 0b01000000;
	}
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);

	/* "fade in" finished, we now can go on with the
	 * regular loop
	 */
}

void cube_walking_walk_out( void )
{
	cube_walking_frame_static1();
	cube_walking_frame2();
	move_x_rev(2,0,5,7);
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);

	cube_walking_frame_static1();
	cube_walking_frame3();
	move_x_rev(2,0,5,7);
	move_x_rev(2,0,5,7);
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);
	
	cube_walking_frame_static1();

	cube[0][3] |= 0b00001000;

	cube[1][0] |= 0b00010000;
	cube[1][2] |= 0b00001000;

	cube[2][1] |= 0b00011000;
	cube[2][3] |= 0b00100000;

	cube[3][0] |= 0b00000100;
	cube[3][1] |= 0b00011000;
	cube[3][2] |= 0b00100000;
	
	cube[4][1] |= 0b00111100;
	
	cube[5][1] |= 0b00011000;
	
	cube[6][0] |= 0b00011000;
	cube[6][1] |= 0b00011000;
	cube[6][2] |= 0b00011000;

	cube[7][1] |= 0b00011000;
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);


	cube_walking_frame_static1();
	cube[0][0] |= 0b00011000;

	cube[1][0] |= 0b00011000;
	cube[2][0] |= 0b00111100;
	cube[3][0] |= 0b00111100;
	cube[4][0] |= 0b00111100;

	cube[5][0] |= 0b00011000;
	
	cube[6][0] |= 0b00011000;
	cube[6][0] |= 0b00011000;
	cube[6][0] |= 0b00011000;

	cube[7][0] |= 0b00011000;
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
	cube_show_loop_wo_int(WALKING_DELAY);
}

void cube_walking_frame0( void )
{
	cube[0][2] |= 0b00010000;
	cube[0][6] |= 0b00001000;

	cube[1][3] |= 0b00010000;
	cube[1][5] |= 0b00001000;

	cube[2][2] |= 0b00000100;
	cube[2][4] |= 0b00011000;
	cube[2][6] |= 0b00100000;

	cube[3][3] |= 0b00000100;
	cube[3][4] |= 0b00011000;
	cube[3][5] |= 0b00100000;
	
	cube[4][4] |= 0b00111100;
	
	cube[5][4] |= 0b00011000;
	
	cube[6][3] |= 0b00011000;
	cube[6][4] |= 0b00011000;
	cube[6][5] |= 0b00011000;

	cube[7][4] |= 0b00011000;
}

void cube_walking_frame1( void )
{
	cube[0][4] |= 0b00011000;


	cube[1][4] |= 0b00011000;
	cube[2][4] |= 0b00111100;
	cube[3][4] |= 0b00111100;
	cube[4][4] |= 0b00111100;
	
	cube[5][4] |= 0b00011000;
	
	cube[6][3] |= 0b00011000;
	cube[6][4] |= 0b00011000;
	cube[6][5] |= 0b00011000;

	cube[7][4] |= 0b00011000;
}

void cube_walking_frame2( void )
{
	cube[0][2] |= 0b00001000;
	cube[0][6] |= 0b00010000;

	cube[1][3] |= 0b00001000;
	cube[1][5] |= 0b00010000;

	cube[2][2] |= 0b00100000;
	cube[2][4] |= 0b00011000;
	cube[2][6] |= 0b00000100;

//	cube[3][2] = 0b00100000;	
	cube[3][3] |= 0b00100000;
	cube[3][4] |= 0b00011000;
	cube[3][5] |= 0b00000100;
	
	cube[4][4] |= 0b00111100;
	
	cube[5][4] |= 0b00011000;
	
	cube[6][3] |= 0b00011000;
	cube[6][4] |= 0b00011000;
	cube[6][5] |= 0b00011000;

	cube[7][4] |= 0b00011000;
}

void cube_walking_frame3( void )
{
	cube[0][4] |= 0b00011000;


	cube[1][4] |= 0b00011000;
	cube[2][4] |= 0b00111100;
	cube[3][4] |= 0b00111100;
	cube[4][4] |= 0b00111100;
	
	cube[5][4] |= 0b00011000;
	
	cube[6][3] |= 0b00011000;
	cube[6][4] |= 0b00011000;
	cube[6][5] |= 0b00011000;

	cube[7][4] |= 0b00011000;
}

void cube_walking( void )
{
	/* disable interrupt and use another loop, flickering otherwise */
	cli();
	cube_walking_frame_door0();
		for (uint8_t cycle = 0; cycle < 13; cycle++){
			cube_walking_frame_static1();
			cube_walking_frame1();
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
			cube_show_loop_wo_int(WALKING_DELAY);
			cube_walking_frame_static2();
			cube_walking_frame2();
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
			cube_show_loop_wo_int(WALKING_DELAY);
			cube_walking_frame_static3();
			cube_walking_frame3();
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
			cube_show_loop_wo_int(WALKING_DELAY);
			cube_walking_frame_static0();
			cube_walking_frame0();
	rotate_90_deg();
	rotate_90_deg();
	rotate_90_deg();
			cube_show_loop_wo_int(WALKING_DELAY);
		}
	cube_walking_walk_out();
	sei();
}


void cube_stripes( void )
{
	cube_clear();
	for (uint8_t i = 0; i < 8; i++) {
		cube[0][0] |= ( 1 << i );
		cube[1][7] |= ( 1 << (7-i) );
		cube[2][0] |= ( 1 << i );
		cube[3][7] |= ( 1 << (7-i) );
		cube[4][0] |= ( 1 << i );
		cube[5][7] |= ( 1 << (7-i) );
		cube[6][0] |= ( 1 << i );
		cube[7][7] |= ( 1 << (7-i) );
		cube_show_loop(STRIPES_DELAY);
	}
	for (uint8_t j = 1; j < 8; j++) {
		cube_clear();
		for (uint8_t i = 0; i < 8; i++) {
			cube[0][j] |= ( 1 << i );
			cube[1][7-j] |= ( 1 << i );
			cube[2][j] |= ( 1 << i );
			cube[3][7-j] |= ( 1 << i );
			cube[4][j] |= ( 1 << i );
			cube[5][7-j] |= ( 1 << i );
			cube[6][j] |= ( 1 << i );
			cube[7][7-j] |= ( 1 << i );
		}
		cube_show_loop(STRIPES_DELAY);
	}
}

/* 
 * 2x2x2 cubes moving
 * build an 4x4x4 (or 4x4) array to organize the cubes
 * use an own function to map the array to the cube
 * choose xyz by rand, check if there is a cube set
 * if yes, choose a direction by rand 
 * check there is neither a bit set in the direction nor
 * the movement would be outside the cube
 * if not, move the pixel, show the cube
 * repeat with check
 * go on with the next pixel
*/

void cube_2_auto( void )
{
	cube_clear();
	char cube2_arr[4][4];
	for (uint8_t i = 0; i < 4; i++){
		for (uint8_t j = 0; j < 4; j++){
			cube2_arr[i][j] = 0;
		}
	}

	cube2_arr[0][0] |= (1 << 0);
	cube2_arr[0][1] |= (1 << 0);
	cube2_arr[0][2] |= (1 << 0);
	cube2_arr[0][3] |= (1 << 0);

	cube2_arr[1][0] |= (1 << 0);
	cube2_arr[1][1] |= (1 << 0);
	cube2_arr[1][2] |= (1 << 0);
	cube2_arr[1][3] |= (1 << 0);

	cube2_arr[0][0] |= (1 << 1);
	cube2_arr[0][1] |= (1 << 1);
	cube2_arr[0][2] |= (1 << 1);
	cube2_arr[0][3] |= (1 << 1);
	
	cube2_arr[0][0] |= (1 << 2);
	cube2_arr[0][1] |= (1 << 2);
	cube2_arr[0][2] |= (1 << 2);
	cube2_arr[0][3] |= (1 << 2);

	cube2_arr[0][0] |= (1 << 3);
	cube2_arr[0][1] |= (1 << 3);
	cube2_arr[0][2] |= (1 << 3);
	cube2_arr[0][3] |= (1 << 3);
	
	cube2_arr[1][0] |= (1 << 3);
	cube2_arr[1][1] |= (1 << 3);
	cube2_arr[1][2] |= (1 << 3);
	cube2_arr[1][3] |= (1 << 3);
	cube_2_auto_show(cube2_arr);
	
	for (uint16_t i = 0; i < 2000; i++) {
		/* TODO: make this faster */
		uint8_t x = rand()%4;
		uint8_t y = rand()%4;
		uint8_t z = rand()%4;
		if (cube2_arr[y][x] & (1 << z)) {
			/* 
			 * we have a chosen an existing pixel
			 * now chose a direction and move it until
			 * it hits something
			 */
			/* stay in case until the
			 * transform functions returns != 0
			 */
			switch (rand()%6)
			{
				case 0:
					/* this checks avoid the pixel gets moved outside
					 * the cube as well as decrementing uint8_t = 0 
					 */
					while(y < 3) {
						if (cube2_move_y_fwd(cube2_arr, y, x, z) == 0) {
							cube_2_auto_show(cube2_arr);
							y++; 
						} else { break; }
					}
					break;
				case 1:
					while(y > 0) {
						if (cube2_move_y_rev(cube2_arr, y, x, z) == 0) {
							cube_2_auto_show(cube2_arr);
							y--; 
						} else { break; }
					}
					break;
				case 2:
					while(x < 3) {
						if (cube2_move_x_fwd(cube2_arr, y, x, z) == 0) {
							cube_2_auto_show(cube2_arr);
							x++; 
						} else { break; }
					}
					break;
				case 3:
					while(x > 0) {
						if (cube2_move_x_rev(cube2_arr, y, x, z) == 0) {
							cube_2_auto_show(cube2_arr);
							x--; 
						} else { break; }
					}
					break;
				case 4:
					while(z < 3) {
						if (cube2_move_z_fwd(cube2_arr, y, x, z) == 0) {
							cube_2_auto_show(cube2_arr);
							z++; 
						} else { break; }
					}
					break;
				case 5:
					while(z > 0) {
						if (cube2_move_z_rev(cube2_arr, y, x, z) == 0) {
							cube_2_auto_show(cube2_arr);
							z--; 
						} else { break; }
					}
					break;
				default:
					break;
			}
		}
	}
}

/* 
 * no checks for pixel would move outside the cube or inactive pixels
 */

uint8_t cube2_move_y_fwd( char cube2[4][4], uint8_t y, uint8_t x, uint8_t z )
{
	/* check if the pixel is already set, if yes, return false */
	if (!(cube2[y+1][x] & (1 << z))){
		cube2[y][x] &= ~(1 << z);
		cube2[y+1][x] |= (1 << z);
		return 0;
	}
	return 1;
}

uint8_t cube2_move_y_rev( char cube2[4][4], uint8_t y, uint8_t x, uint8_t z  )
{
	if (!(cube2[y-1][x] & (1 << z))){
		cube2[y][x] &= ~(1 << z);
		cube2[y-1][x] |= (1 << z);
		return 0;
	}
	return 1;
}

uint8_t cube2_move_x_fwd( char cube2[4][4], uint8_t y, uint8_t x, uint8_t z  )
{
	if (!(cube2[y][x+1] & (1 << z))){
		cube2[y][x] &= ~(1 << z);
		cube2[y][x+1] |= (1 << z);
		return 0;
	}
	return 1;
}
uint8_t cube2_move_x_rev( char cube2[4][4], uint8_t y, uint8_t x, uint8_t z  )
{
	if (!(cube2[y][x-1] & (1 << z))){
		cube2[y][x] &= ~(1 << z);
		cube2[y][x-1] |= (1 << z);
		return 0;
	}
	return 1;
}

uint8_t cube2_move_z_fwd( char cube2[4][4], uint8_t y, uint8_t x, uint8_t z  )
{
	if (!(cube2[y][x] & (1 << (z+1)))){
		cube2[y][x] &= ~(1 << z);
		cube2[y][x] |= (1 << (z+1));
		return 0;
	}
	return 1;
}

uint8_t cube2_move_z_rev( char cube2[4][4], uint8_t y, uint8_t x, uint8_t z  )
{
	if (!(cube2[y][x] & (1 << (z-1)))){
		cube2[y][x] &= ~(1 << z);
		cube2[y][x] |= (1 << (z-1));
		return 0;
	}
	return 1;
}

void cube_2_auto_show( char cube2[4][4] )
{
	for (uint8_t layer = 0; layer < 4; layer++){
	/* avoid flickering, clear 2 layers only */
		cube_clear_layer(layer*2+1);
		cube_clear_layer(layer*2);
		for (uint8_t x = 0; x < 4; x++){
			for (uint8_t z = 0; z < 4; z++){
				if (cube2[layer][x] & ( 1 << z)) {
					cube[layer*2][x*2] |= (1 << (z*2));
					cube[layer*2][x*2] |= (1 << (z*2+1));
					cube[layer*2][x*2+1] |= (1 << (z*2));
					cube[layer*2][x*2+1] |= (1 << (z*2+1));
					cube[layer*2+1][x*2] |= (1 << (z*2));
					cube[layer*2+1][x*2] |= (1 << (z*2+1));
					cube[layer*2+1][x*2+1] |= (1 << (z*2));
					cube[layer*2+1][x*2+1] |= (1 << (z*2+1));
				}
			}
		}
	}
	cube_show_loop(CUBE_AUTO_DELAY);

}

void cubes_2( void )
{
	cube_clear();
	for (uint8_t i = 0; i < 8; i++ ){
		cube[0][i] = 0xFF;
		cube[1][i] = 0xFF;
	}
	cube_show_loop(CUBES2_DELAY);
	for (uint8_t i = 0; i < 6; i++) {
		move_y_fwd(2,2,3,3);
		move_y_fwd(2,6,3,7);
		move_y_fwd(6,2,7,3);
		move_y_fwd(6,6,7,7);
		cube_show_loop(CUBES2_DELAY);
	}
	for (uint8_t i = 0; i < 4; i++) {
		move_y_fwd(0,0,1,1);
		move_y_fwd(0,4,1,5);
		move_y_fwd(4,0,5,1);
		move_y_fwd(4,4,5,5);
		cube_show_loop(CUBES2_DELAY);
	}
	for (uint8_t i = 0; i < 2; i++) {
		move_y_fwd(2,0,3,1);
		move_y_fwd(6,0,7,1);
		move_y_fwd(2,4,3,5);
		move_y_fwd(6,4,7,5);
		cube_show_loop(CUBES2_DELAY);
	}

	for (uint8_t i = 0; i < 4; i++) {
		move_y_rev(6,2,7,3);
		cube_show_loop(CUBES2_DELAY);
	}
	for (uint8_t i = 0; i < 2; i++) {
		move_y_rev(4,0,5,1);
		cube_show_loop(CUBES2_DELAY);
	}
	for (uint8_t i = 0; i < 6; i++) {
		move_z_fwd(2,4,3,5);
		cube_show_loop(CUBES2_DELAY);
	}
	
	for (uint8_t i = 0; i < 2; i++) {
		move_y_rev(2,0,3,1);
		cube_show_loop(CUBES2_DELAY);
	}
	
	for (uint8_t i = 0; i < 6; i++) {
		move_z_fwd(0,2,1,3);
		cube_show_loop(CUBES2_DELAY);
	}
	
	for (uint8_t i = 0; i < 4; i++) {
		move_y_fwd(0,2,1,3);
		cube_show_loop(CUBES2_DELAY);
	}
	
	for (uint8_t i = 0; i < 2; i++) {
		move_x_fwd(2,4,3,5);
		cube_show_loop(CUBES2_DELAY);
	}
	cube_show_loop(CUBES2_DELAY*10);
}


/* cubemoving 4x4 */
void cubes_4( void )
{
	cube_clear();
	for (uint8_t layer = 0; layer <= 3; layer++){
		cube[layer][0] = 0b00001111;
		cube[layer][1] = 0b00001111;
		cube[layer][2] = 0b00001111;
		cube[layer][3] = 0b00001111;
	}
	for (uint8_t layer = 4; layer <= 7; layer++){
		cube[layer][4] = 0b11111111;
		cube[layer][5] = 0b11111111;
		cube[layer][6] = 0b11111111;
		cube[layer][7] = 0b11111111;
	}
	cube_show_loop(CUBES4_DELAY);
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_y_rev(4,0,7,3);
		cube_show_loop(CUBES4_DELAY);
		
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_z_fwd(0,0,3,3);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_y_fwd(0,4,3,7);	
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_y_rev(4,4,7,7);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_x_rev(0,0,3,3);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_z_rev(4,0,7,3);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_z_fwd(4,0,7,3);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_y_fwd(4,4,7,7);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_x_fwd(0,0,3,3);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_y_fwd(4,0,7,3);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_y_rev(0,4,3,7);
		cube_show_loop(CUBES4_DELAY);
	}
	for (uint8_t fourtimes = 0; fourtimes <= 3; fourtimes++){
		move_z_rev(0,0,3,3);
		cube_show_loop(CUBES4_DELAY);
	}
}

/* shrinking outline */
void outline_shrink( void )
{
	cube_clear();
	for (uint8_t i = 1; i < 7; i++ ){
		cube[i][0] = 0b10000001;
		cube[i][7] = 0b10000001;
	}
	for (uint8_t i = 1; i < 7; i++){
		cube[0][i] = 0b10000001;
		cube[7][i] = 0b10000001;
	}
	cube[0][0] = 0xFF;
	cube[7][0] = 0xFF;
	cube[0][7] = 0xFF;
	cube[7][7] = 0xFF;
	cube_show_loop(OUT_SHRINK_DELAY);

	for (uint8_t i = 2; i < 6; i++ ){
		cube[i][1] = 0b01000010;
		cube[i][6] = 0b01000010;
	}
	for (uint8_t i = 2; i < 6; i++){
		cube[1][i] = 0b01000010;
		cube[6][i] = 0b01000010;
	}
	cube[1][1] = 0b01111110;
	cube[6][1] = 0b01111110;
	cube[1][6] = 0b01111110;
	cube[6][6] = 0b01111110;
	cube_show_loop(OUT_SHRINK_DELAY);

	for (uint8_t i = 3; i < 5; i++ ){
		cube[i][2] = 0b00100100;
		cube[i][5] = 0b00100100;
	}
	for (uint8_t i = 3; i < 5; i++){
		cube[2][i] = 0b00100100;
		cube[5][i] = 0b00100100;
	}
	cube[2][2] = 0b00111100;
	cube[5][2] = 0b00111100;
	cube[2][5] = 0b00111100;
	cube[5][5] = 0b00111100;
	cube_show_loop(OUT_SHRINK_DELAY);

	cube[3][3] = 0b00011000;
	cube[4][3] = 0b00011000;
	cube[3][4] = 0b00011000;
	cube[4][4] = 0b00011000;	
	cube_show_loop(10*OUT_SHRINK_DELAY);
}


/* explosion illuminate all starting at middle an turn off in same order */
void cube_explosion( void )
{
	uint8_t led_state = 0b00011000;
	for (uint8_t i = 0; i <= 4; i++){
		for (uint8_t j = 3; j <= (i+3); j++) {
			for (uint8_t k = 3; k <= (i+3); k++){
				cube[j][7-k] = led_state;
				cube[j][k] = led_state;
				cube[7-j][7-k] = led_state;
				cube[7-j][k] = led_state;
			}
		}
		cube_show_loop(EXPLOSION_DELAY*(2*i+1));
		led_state |= (1 << (3-i)) | (1 << (4+i));
	}
	cube_show_loop(EXPLOSION_DELAY);
	for (uint8_t i = 0; i <= 4; i++){
		led_state &= ~((1 << (3-i)) | (1 << (4+i)));
		for (uint8_t j = 3; j <= (i+3); j++) {
			for (uint8_t k = 3; k <= (i+3); k++){
				cube[j][7-k] = led_state;
				cube[j][k] = led_state;
				cube[7-j][7-k] = led_state;
				cube[7-j][k] = led_state;
			}
		}
		cube_show_loop(EXPLOSION_DELAY*(2*i+1));
	}
	cube_show_loop(EXPLOSION_DELAY);
}

/* wipe */
void cube_wipe( void )
{
	cube_test_y();
	cube_show_loop(SWIPE_DELAY);
	for (uint8_t layer = 0; layer < 7; layer++){	
		for (uint8_t i = 0; i < 8; i++){
			cube[layer][i] = 0;
			cube[layer+1][i] = 0xFF;
		}
		cube_show_loop(SWIPE_DELAY);
	}
	for (uint8_t layer = 0; layer < 7; layer++){	
		move_y_rev(0,0,7,7);
		cube_show_loop(SWIPE_DELAY);
	}
}

/* moving chars on a belt
 * a cute function to display larger strings.
 * we need to define an own layer for the chars
 * which is then transformed to the cube array 
 */

void cube_string_belt( char *string )
{
	/* 2D 
	 * bits are Y, 0 is bottom, 7 is top
	 * 0 is the right end where the chars come in
	 * 20 visible lines, 5 are added to the "beginning"
	 * outside the cube so the chars can move in
	 */
	char cube_belt[25];
	for (uint8_t i = 0; i < 25; i++) { cube_belt[i] = 0; }

	string++;
	string++;
	while(*string){
		set_char_to_belt(*string, cube_belt);
		for (uint8_t i = 0; i < 6; i++){
			show_belt(cube_belt);
			move_belt_left(cube_belt);
		}
		string++;
	}
	/* move the rest out */
	for (uint8_t i = 0; i < 18; i++){
		show_belt(cube_belt);
		move_belt_left(cube_belt);
	}
}

void move_belt_left( char *belt )
{
	for (uint8_t i = 25; i > 1; i--){
		belt[i-1] = belt[i-2];
	}
	belt[0] = 0;
}
/*
 * set a char to the cube array, then transform this char to
 * the first 5 elements of the belt array 
 */
void set_char_to_belt( char character, char *belt )
{	
	cube_clear();
	cli();
	PORTC = 0x00;
	cube_char(character, 1);
	for (uint8_t layer = 0; layer < 8; layer++) {
		for (uint8_t i = 1; i < 6; i++){
			if (cube[layer][i] & 0x01) 
				belt[5-i] |= (1 << layer);
		}
	}
	cube_clear();
	sei();
}

/* this function passes the given array to the cube array */
void show_belt( char *belt )
{
	cube_clear();
	/* right side */
	for (uint8_t i = 5; i <= 11; i++){
		for (uint8_t j = 0; j < 8; j++){
			if (belt[i] & (1 << j))
				cube[j][7] |= (1 << (12-i));
		}
	}
	/* front side */
	for (uint8_t i = 12; i <= 17; i++){
		for (uint8_t j = 0; j < 8; j++){
			if (belt[i] & (1 << j))
				cube[j][18-i] |= (1 << 0);
		}
	}
	/* left side */
	for (uint8_t i = 18; i <= 24; i++){
		for (uint8_t j = 0; j < 8; j++){
			if (belt[i] & (1 << j))
				cube[j][0] |= (1 << (i-17));
		}
	}
	cube_show_loop(SHOW_BELT_DELAY);
}

/* 2 fixed chars rotating */
void cube_fixed_string( void )
{
	cube_clear();
	cube_clear();
	cube_char('3', 2);
	rotate_90_deg();
	rotate_90_deg();
	cube_char('d', 2);
	rotate_90_deg();

	rotate_90_auto(8);
}


/* back to front moving */
void cube_string_to_front( char *string )
{
	while(*string) {	
		for (uint8_t i = 8; i > 0; i--){
			cube_clear();
			cube_char(*string, (1 << (i-1)));
			cube_show_loop(3);
		}
		string++;
	}

}

/* automated text display, takes a string and adds
 * some effects. Usable for short strings.
 */
void cube_string( char *string )
{
	while(*string){
		cube_clear();
		cli();
		PORTC = 0x00;
		cube_char(*string, 16);
		string++;
		rotate_90_deg();
		rotate_90_deg();
		rotate_90_deg();
		move_x_rev(1,0,6,7);
		move_x_rev(1,0,6,7);
		move_x_rev(1,0,6,7);
		sei();
		cube_show_loop(CUBE_STRING_DELAY);
		move_x_fwd(1,0,6,7);
		cube_show_loop(CUBE_STRING_DELAY);
		move_x_fwd(1,0,6,7);
		cube_show_loop(CUBE_STRING_DELAY);
		move_x_fwd(1,0,6,7);
		cube_show_loop(CUBE_STRING_DELAY);
		rotate_90_auto(1);
		move_z_rev(0,0,7,7);
		cube_show_loop(CUBE_STRING_DELAY);
		move_z_rev(0,0,7,7);
		cube_show_loop(CUBE_STRING_DELAY);
		move_z_rev(0,0,7,7);
		cube_show_loop(CUBE_STRING_DELAY);
		move_z_rev(0,0,7,7);
		cube_show_loop(CUBE_STRING_DELAY);
		cube_flash(5);
		cube_explosion();
	}
	cube_explosion();
	cube_explosion();
}

void cube_outline( void )
{
	cube_clear();
	cube_show_loop(OUTLINE_DELAY);
	for (uint8_t i = 0; i < 8; i++) {
		cube[0][0] |= (1 << i);
		cube[i][0] |= 1;
		cube[0][i] |= 1;
		cube_show_loop(OUTLINE_DELAY);
	}
	for (uint8_t i = 1; i < 8; i++) {
		cube[7][0] |= (1 << i);
		cube[7][i] |= 1;
		cube[i][0] |= 128;
		cube[0][i] |= 128;
		cube[0][7] |= (1 << i);
		cube[i][7] |= 1;
		cube_show_loop(OUTLINE_DELAY);
	}
	for (uint8_t i = 1; i < 8; i++) {
		cube[7][i] |= 128;
		cube[7][7] |= (1 << i);
		cube[i][7] |= 128;
		cube_show_loop(OUTLINE_DELAY);
	}
	cube_show_loop(20);
	/* shrink */
	cube_clear();
	cube[1][1] = 0b01111110;
	cube[1][6] = 0b01111110;
	cube[6][1] = 0b01111110;
	cube[6][6] = 0b01111110;
	for (uint8_t i=2; i< 6; i++) {
		cube[1][i] = 0b01000010;
		cube[6][i] = 0b01000010;
		cube[i][1] = 0b01000010;
		cube[i][6] = 0b01000010;
	}
	cube_show_loop(30);
	rotate_90_auto( 8 );
 
}

/* freaky: when looking at this for a while in a dark room
 * you begin to see pink dots in the cube */
void cube_stars( void )
{
	for (uint8_t j = 1; j < 30; j++){
		for (uint8_t loops = 0; loops < 18; loops++){
			cube_clear();
			for (uint8_t i = 0; i< j; i++){
				uint8_t randx = (uint8_t)rand()%8;
				uint8_t randy = (uint8_t)rand()%8;
				uint8_t randz = (uint8_t)rand()%8;
				cube[randy][randx] = (1 << randz);
			}
//		cube[randy][randx] &= ~(1 << randz);
			cube_show_loop(STARS_DELAY);
		}
	}
}

/* kind of sine wave
 * in other words, the best sine you can do with 8x8 pixels 
 */
void cube_waves( void )
{
	cube_clear();
	for (uint8_t i = 0; i < 10; i++){
		move_x_rev(0,0,7,7);
		cube[3][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[2][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[1][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[0][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[0][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[1][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[2][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[3][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[4][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[5][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[6][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[7][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[7][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[6][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[5][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
		move_x_rev(0,0,7,7);
		cube[4][7] |= WAVES_DEPTH;
		cube_show_loop(WAVES_DELAY);
	}
}

/* another wave simulation */
void cube_sonic( void )
{
	cli();
	for (uint8_t i = 0; i < 10; i++){
		cube_clear();
		cube_show_loop(20);
		uint8_t mode = rand()%5;
		uint8_t sonic_layers = (rand()%8)+1;
		if (mode <= 3){
			cube_clear();
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b10000000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			
			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b01000000;
				cube[layer][1] = 0b10000000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			
			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b00100000;
				cube[layer][1] = 0b00100000;
				cube[layer][2] = 0b11000000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			
			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b00010000;
				cube[layer][1] = 0b00010000;
				cube[layer][2] = 0b00100000;
				cube[layer][3] = 0b11000000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			
			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b00001000;
				cube[layer][1] = 0b00001000;
				cube[layer][2] = 0b00010000;
				cube[layer][3] = 0b00110000;
				cube[layer][4] = 0b11000000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			
			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b00000100;
				cube[layer][1] = 0b00000100;
				cube[layer][2] = 0b00000100;
				cube[layer][3] = 0b00001000;
				cube[layer][4] = 0b00010000;
				cube[layer][5] = 0b11100000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			
			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b00000010;
				cube[layer][1] = 0b00000010;
				cube[layer][2] = 0b00000010;
				cube[layer][3] = 0b00000100;
				cube[layer][4] = 0b00001000;
				cube[layer][5] = 0b00010000;
				cube[layer][6] = 0b11100000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);

			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b00000001;
				cube[layer][1] = 0b00000001;
				cube[layer][2] = 0b00000001;
				cube[layer][3] = 0b00000010;
				cube[layer][4] = 0b00000010;
				cube[layer][5] = 0b00000100;
				cube[layer][6] = 0b00011000;
				cube[layer][7] = 0b11100000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);

			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][4] = 0b00000001;
				cube[layer][5] = 0b00000010;
				cube[layer][6] = 0b00000100;
				cube[layer][7] = 0b00001000;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);

			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][6] = 0b00000001;
				cube[layer][7] = 0b00000010;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);

			cube_clear();	
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][7] = 0b00000001;
			}
			for (uint8_t deg = 0; deg < mode; deg++){
				rotate_90_deg();
			}
			cube_show_loop_wo_int(SONIC_DELAY);
		} else {
			/* center wave */
			cube_clear();
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][3] = 0b00011000;
				cube[layer][4] = 0b00011000;
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			cube_clear();
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][2] = 0b00011000;
				cube[layer][3] = 0b00100100;
				cube[layer][4] = 0b00100100;
				cube[layer][5] = 0b00011000;
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			cube_clear();
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][1] = 0b00011000;
				cube[layer][2] = 0b00100100;
				cube[layer][3] = 0b01000010;
				cube[layer][4] = 0b01000010;
				cube[layer][5] = 0b00100100;
				cube[layer][6] = 0b00011000;
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			cube_clear();
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b00011000;
				cube[layer][1] = 0b01100110;
				cube[layer][2] = 0b01000010;
				cube[layer][3] = 0b10000001;
				cube[layer][4] = 0b10000001;
				cube[layer][5] = 0b01000010;
				cube[layer][6] = 0b01100110;
				cube[layer][7] = 0b00011000;
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			cube_clear();
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b01000010;
				cube[layer][1] = 0b10000001;
				cube[layer][6] = 0b10000001;
				cube[layer][7] = 0b01000010;
			}
			cube_show_loop_wo_int(SONIC_DELAY);
			cube_clear();
			for (uint8_t layer = 0; layer < sonic_layers; layer++){
				cube[layer][0] = 0b10000001;
				cube[layer][7] = 0b10000001;
			}
			cube_show_loop_wo_int(SONIC_DELAY);
		}
	}
	sei();	
}
/* not sure, 8x8x8 is not really appropriate for diamonds */
void cube_diamond( void )
{
	cube_clear();
	for ( uint8_t i = 0; i < 3; i++ ){
		cube[4][2] = 0b01111110;
	}
}

/* move the actual frame out of the cube */
void cube_block_wipe( void )
{
	for (uint8_t i = 0; i < 8; i++){
		move_x_fwd(0,0,7,7);
		for(uint8_t layer = 0; layer < 8; layer++){
			cube[layer][0] |= 0xFF;
		}
		cube_show_loop(BLOCK_WIPE_DELAY);
	}
	cube_show_loop(BLOCK_WIPE_DELAY);
	for (uint8_t i = 8; i != 0; i--){
		for(uint8_t layer = 0; layer < 8; layer++){
			cube[layer][i-1] = 0x00;
		}
		cube_show_loop(BLOCK_WIPE_DELAY);
	}
		
}

/* belt moving from top to bottom deleting the frame */
void cube_belt_wipe( void )
{
	for (uint8_t layer = 8; layer != 0; layer--){
		cube[layer-1][0] |= 0xFF;
		cube[layer-1][7] |= 0xFF;
		for (uint8_t i = 1; i < 7; i++){ cube[layer-1][i] |= 0b10000001; }
		cube_show_loop(BELT_WIPE_DELAY);
	}
	for (uint8_t layer = 8; layer != 0; layer--){
		for (uint8_t i = 0; i < 8; i++){ cube[layer-1][i] = 0x00; }
		cube_show_loop(BELT_WIPE_DELAY);
	}
}

/* do not source the layer FETs */
void cube_flash( uint8_t cycle )
{
	for (; cycle > 0; cycle--){
	cli();
	PORTC = 0x00;
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
	sei();
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
		_delay_ms(BLINK_DELAY*5);
	}
}
/*
void invaders_show_monster( uint8_t *monster_array, uint8_t vert, uint8_t hor )
{
	for (uint8_t monster_x = 0; monster_x < 1; monster_x++){
		if (monster_array[monster_x] )
	}
}
*/

/* 
 * need special functions, otherwise the bullets would have to be stored
 * in an extra array (-> redraw cube every time)
 */
void invaders_move_right( uint8_t layer, int8_t *position )
{
	uint8_t check = 0b11000000;
	for (uint8_t i = 0; i < 3; i++){
		/* alway check for 2 bits to make sure its no bullet */
		if ( cube[layer][(*position)] & check && cube[layer-1][(*position)] & check ){
			cube[layer][(*position)] &= ~check;
			cube[layer-1][(*position)] &= ~check;
			cube[layer][(*position)+2] |= check;
			cube[layer-1][(*position)+2] |= check;
		}
		if ( cube[layer][(*position)+3] & check && cube[layer-1][(*position)+3] & check){
			cube[layer][(*position)+3] &= ~check;
			cube[layer-1][(*position)+3] &= ~check;
			cube[layer][(*position)+5] |= check;
			cube[layer-1][(*position)+5] |= check;
		}
		check = (check >> 3);
	}
	(*position)++;
}

void invaders_move_left( uint8_t layer, int8_t *position )
{
	uint8_t check = 0b11000000;
	for (uint8_t i = 0; i < 3; i++){
		if ( cube[layer][(*position)+1] & check && cube[layer-1][(*position)+1] & check){
			cube[layer][(*position)+1] &= ~check;
			cube[layer-1][(*position)+1] &= ~check;
			cube[layer][(*position)-1] |= check;
			cube[layer-1][(*position)-1] |= check;
		}
		if ( cube[layer][(*position)+4] & check && cube[layer-1][(*position)+4] & check){
			cube[layer][(*position)+4] &= ~check;
			cube[layer-1][(*position)+4] &= ~check;
			cube[layer][(*position)+2] |= check;
			cube[layer-1][(*position)+2] |= check;
		}
		check = (check >> 3);
	}
	(*position)--;
}

void invaders_move_down( uint8_t *layer, int8_t position )
{
	uint8_t check = 0b11000000;
	for (uint8_t i = 0; i < 3; i++){
		if ( cube[(*layer)][position] & check ){
			cube[(*layer) ][position] &= ~check;
			cube[(*layer)][position+1] &= ~check;
			cube[(*layer-2)][position] |= check;
			cube[(*layer-2)][position+1] |= check;
		}
		if ( cube[(*layer)][position+3] & check ){
			cube[(*layer)][position+3] &= ~check;
			cube[(*layer)][position+4] &= ~check;
			cube[(*layer-2)][position+3] |= check;
			cube[(*layer-2)][position+4] |= check;
		}
		check = (check >> 3);
	}
	(*layer)--;
}

#define MONSTER_RIGHT invaders_move_right((*m_layer), m_pos)
#define MONSTER_LEFT invaders_move_left((*m_layer), m_pos)
#define MONSTER_DOWN  invaders_move_down(m_layer, (*m_pos))
#define INV_DELAY invader_show_loop(&shot_layer, &shot_pos, &monster_layer, &monster_pos)
#define DEF_RIGHT move_x_fwd( 0, 0, 7, 1); defender_pos += 0x10;
#define DEF_LEFT move_x_rev( 0, 0, 7, 1); defender_pos -= 0x10;
#define DEF_BACK move_z_fwd( 0, 0, 1, 7); defender_pos += 0x01;
#define DEF_FWD move_z_rev( 0, 0, 1, 7); defender_pos -= 0x01;
#define DEF_FIRE def_fire( &shot_layer, &shot_pos, defender_pos )

static void def_fire( uint8_t *layer, uint8_t *position, uint8_t defender_position )
{
	(*position) = defender_position; 
	(*layer) = 2; 
	cube[(*layer)][(((*position) & 0xF0) >> 4)] |= (1 << ((*position) & 0x0F));
	cube_show_loop_wo_int(INVADERS_DELAY); /*otherwise first pixel could not be seen */
}

/* usual delay combined with moving the bullet one up */
static void invader_show_loop(uint8_t *layer, uint8_t *position, uint8_t *m_layer, int8_t *m_pos )
{
	static uint8_t prescaler = 1;	
	if ( (*layer) < 7){
		(*layer)++;
		/* check if monster is hit */ 
		if (cube[(*layer)][(((*position) & 0xF0) >> 4)] & ( 1 << ((*position) & 0x0F)) ) {
		/* flash all pixels around the bullet */
			uint8_t current_layer = (*layer)-1;
			while ( (current_layer <= ((*layer)+1)) && (current_layer < 8)){
				cube[current_layer][(((*position) & 0xF0) >> 4)+1] |= (7 << (((*position) & 0x0F)-1));
				cube[current_layer][(((*position) & 0xF0) >> 4)] |= (7 << (((*position) & 0x0F)-1));
				cube[current_layer][(((*position) & 0xF0) >> 4)-1] |= (7 << (((*position) & 0x0F)-1));
				current_layer++;
			}
			cube_show_loop_wo_int(INVADERS_DELAY);
			
			current_layer = (*layer)-1;
			while ( (current_layer <= ((*layer)+1)) && (current_layer < 8)){
				cube[current_layer][(((*position) & 0xF0) >> 4)+1] &= ~(7 << (((*position) & 0x0F)-1));
				cube[current_layer][(((*position) & 0xF0) >> 4)] &= ~(7 << (((*position) & 0x0F)-1));
				cube[current_layer][(((*position) & 0xF0) >> 4)-1] &= ~(7 << (((*position) & 0x0F)-1));
				current_layer++;
			}
		} else {
			/* no monster, move up, layer already incremented */
			cube[(*layer)-1][(((*position) & 0xF0) >> 4)] &= ~(1 << ((*position) & 0x0F)); 
			/* always inside the cube, checked by layer < 7*/
			cube[(*layer)][(((*position) & 0xF0) >> 4)] |= (1 << ((*position) & 0x0F)); 
			cube_show_loop_wo_int(INVADERS_DELAY);
		}
	} else {
		/* delete shot, increment layer to avoid counting up to inf */
		if ( (*layer) < 8) { 
			cube[(*layer)++][(((*position) & 0xF0) >> 4)] &= ~(1 << ((*position) & 0x0F)); 
		}
		cube_show_loop_wo_int(INVADERS_DELAY);
	}

	static uint8_t direction = 0;
	if ((prescaler % 6) == 0){
		if (((*m_pos) < 3) && (direction == 0)) {
			MONSTER_RIGHT;
		} else if (((*m_pos) >= 1) && (direction == 1)){
			MONSTER_LEFT;
		}

		if ((*m_pos) == 3) {
			direction = 1;
		} else if ((*m_pos) == 0) {
			direction = 0;
		}
	}
	if ((prescaler % 128) == 0){
		if ((*m_layer) > 2)
			MONSTER_DOWN;
	}
	if (prescaler < 255)
		prescaler++;
	else 
		prescaler = 0;
}

void cube_invaders( void )
{
	cli();
	cube_clear();
	uint8_t monster_layer = 7; /* vert.  monster layer,  */
	int8_t monster_pos = 1; /* hor. monster layer position */
	uint8_t defender_pos = 0x43; /* high nibble: x, low nibble: y */
	/* limit to only one shot at once, only defender is able to fire */
	uint8_t shot_pos = 0; /* save last shot position */
	uint8_t shot_layer = 9; /* current shot layer */

	cube[0][3] |= 0b00011100;
	cube[0][4] |= 0b00011100;
	cube[0][5] |= 0b00011100;
	cube[1][4] |= 0b00001000;
	
	cube[6][1] |= 0b11011011;
	cube[7][1] |= 0b11011011;
	cube[6][2] |= 0b11011011;
	cube[7][2] |= 0b11011011;
	
	cube[6][4] |= 0b11011011;
	cube[7][4] |= 0b11011011;
	cube[6][5] |= 0b11011011;
	cube[7][5] |= 0b11011011;

	while ( monster_layer > 2 ) {
		for (uint8_t i = 0; i < 20; i++){
			switch (rand()%4){
				case 0:
					if ((defender_pos & 0xF0) < 0x60) { DEF_RIGHT; }
					break;
				case 1:
					if ((defender_pos & 0xF0) > 0x10) { DEF_LEFT; }
					break;
				case 2: 
					if ((defender_pos & 0x0F) > 0x01) { DEF_FWD; }
					break;
				case 3:
					if ((defender_pos & 0x0F) < 0x06) { DEF_BACK; }
					break;
				default:
					break;
			}
			INV_DELAY;
			INV_DELAY;
		}
		DEF_FIRE;
	}
	sei();
	cube_explosion();
	cube_string_belt("  game over");
	cube_clear();
}
