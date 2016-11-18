/*
fornax LED cube V1.0

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/


/* 

PORTA is LE
PORTB is not used
PORTC is layer enable
PORTD is 8 bit latch input

*/

#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>
#include "ledcube.h"
#include "animations.h"

void init(void)
{
	/* 
           most important: configure layer enable as output and low
           as the latches are not able to source more than one layer
           at once
        */
	PORTC = 0;
	DDRC = 0xFF;
	DDRA = 0xFF;
	DDRD = 0xFF;
	PORTA = 0;
	PORTD = 0;
	cube_show_init();
	_delay_ms(1);
	sei();
	_delay_ms(1);
}	

int main(void)
{
	init();
	_delay_ms(100);
	_delay_ms(100);
	cube_clear(); /* never forget this */

	while ( 1 ){	

		cube_string_belt("  fornax led cube demo");
		cube_explosion();

		cube_string_to_front("leyanda.de");
		cube_belt_wipe();

		cube_walking();
		cube_block_wipe();

		cube_fixed_string();
		cube_block_wipe();

		cube_2_auto();
		cube_wipe();

		cube_stripes();
		cube_belt_wipe();
		
		cube_invaders();

		cube_waves();
		cube_explosion();

		outline_shrink();
		cube_explosion();

		cube_string("42");

		cubes_4();
		cube_belt_wipe();

		cube_outline();
		cube_explosion();

		cube_stars();
		cube_explosion();

		cube_sonic();
		cube_belt_wipe();
	}

		/* 
		 * this might be used for a random assortment 
 		 * of the effects, add them to the switch statement
		*/
		/*
		#define EFFECTS
		uint8_t prog[EFFECT];
        	uint8_t j = 0;
	        while ( j < EFFECTS ) {
	                uint8_t k = 0;
	                uint8_t number = rand()%EFFECTS;
	                uint8_t isunique = 1;
	                for ( k = 0; k < j; k++ ){
	                        if ( prog[k] == number )
	                            isunique = 0;
	                }
	                if (isunique == 1) {
	                        prog[k] = number;
	                        j++;
	                }
	        }
		for (uint8_t i = 0; i < EFFECTS; i++ ) {
			switch (prog[i])
			{
				default:
				    break;
			}
		}
		*/
}

