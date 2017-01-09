/*
fornax LED cube V1.1

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include "ledcube.h"
#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <math.h>

#include <avr/interrupt.h>

#include "ab.h"
#include "rotation.h"


void cube_show_init( void )
{
	TCCR0 |= (1 << CS02) | (0 << CS00); 
	TIMSK |= (1 << TOIE0);
}


ISR( TIMER0_OVF_vect )
{
	TCNT0 = 0xB0; /*at least 0xA0 */
	/* cli() not neccessary atm */
	PORTC = 0x0;
	static uint8_t cube_show_layer = 0;
	asm volatile("nop"); /* wait longer to avoid unintentionally lighting bottom leds */
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	for(uint8_t j = 0; j < 8; j++){
		PORTD = cube[cube_show_layer][j];
		asm volatile("nop");
		PORTA |= (1 << j);
		asm volatile("nop");
		PORTA = 0;
		asm volatile("nop");
	}
	PORTC |= (1 << cube_show_layer);
	asm volatile("nop");
	if (cube_show_layer < 7) {
		cube_show_layer++;
	} else {
		cube_show_layer = 0;
	}
}

/* basic transformations and view functions */

/* all leds off */
void cube_clear ( void )
{
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
			cube[i][j] = 0;
		}
	}
}

/* single layer clear to avoid flickering with auto cube */
void cube_clear_layer(uint8_t layer)
{
	for (uint8_t j = 0; j < 8; j++){
		cube[layer][j] = 0;
	}
}

void cube_full ( void )
{
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
			cube[i][j] = 255;
		}
	}
}
/* some test cubes */
void cube_cube_3 ( void )
{
	cube[0][0] = 7;
	cube[0][1] = 7;
	cube[0][2] = 7;
	cube[1][0] = 7;
	cube[1][1] = 7;
	cube[1][2] = 7;
	cube[2][0] = 7;
	cube[2][1] = 7;
	cube[2][2] = 7;
}

void cube_cube_4_line ( void )
{
	cube[0][0] = 15;
	cube[0][1] = 9;
	cube[0][2] = 9;
	cube[0][3] = 15;
	cube[1][0] = 9;
	cube[1][3] = 9;
	cube[2][0] = 9;
	cube[2][3] = 9;
	cube[3][0] = 15;
	cube[3][1] = 9;
	cube[3][2] = 9;
	cube[3][3] = 15;

}

void cube_random( void )
{
	for ( uint8_t i = 0; i < 8; i++ ){
		for ( uint8_t j= 0; j < 8; j++) {
			cube[i][j] = rand()%255;
		}
	}
}

/* one layer */
void cube_test_z( void )
{
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
			cube[i][j] = 1;
		}
	}
}

void cube_test_y( void )
{
	for (uint8_t i = 0; i < 8; i++){
		cube[0][i] = 255;
	}
}

void cube_test_x( void )
{
	for (uint8_t i = 0; i < 8; i++){
		cube[i][0] = 255;
	}
}

/* show one frame */
void cube_show( void )
{

	asm volatile("nop");
	for (uint8_t i = 0; i < 8; i++){
		for(uint8_t j = 0; j < 8; j++){
			PORTD = cube[i][j];
			asm volatile("nop");
			PORTA |= (1 << j);
			asm volatile("nop");
			PORTA = 0;
			asm volatile("nop");
		}
		PORTC |= (1 << i);
		_delay_ms(1);
		PORTC = 0x00;
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
	}
}

/* viewed through ISR by default, so just a delay */
void cube_show_loop( uint8_t cycle )
{
	for (uint16_t i = 0; i < cycle*2; i++){ 
		_delay_ms(8);
	}
}

/* some functions may flicker when the ISR views the cube
 * while transforming something -> use this loop */
void cube_show_loop_wo_int( uint8_t cycle )
{
	for (uint16_t i = 0; i < cycle*2; i++){
		cube_show();
	}
	
}
