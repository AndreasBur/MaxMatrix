/*
fornax LED cube V1.1

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <math.h>
#include "ab.h"
#include "rotation.h"
#include "ledcube.h"
#include "translation.h"

/* basic translations  */


/* 
 * thats easy... 
 * arguments define the size and position of the area to move
 */


void move_z_fwd (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	/* first loop can count x1 directly, second loop not */
	for (; x1 <= x2; x1++){
		for (uint8_t i = y1; i <= y2; i++){
			cube[x1][i] = (cube[x1][i] << 1);
		}
	}
}

void move_z_rev ( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )

{
	for (; x1 <= x2; x1++){
		for (uint8_t i= y1; i <= y2; i++){
			cube[x1][i] = (cube[x1][i] >> 1);
		}
	}
}

void move_y_fwd( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	for (uint8_t i = 7; i > 0; i--){
		for (uint8_t j = x1; j <= x2; j++){
			
			for (uint8_t k = y1; k <= y2; k++) {
				if ((cube[i-1][j] & (1 << k))){
					cube[i][j] |= (1 << k);
					cube[i-1][j] &= ~(1 << k);
				} else {
					cube[i-1][j] &= ~(1 << k);
				}
			}

		}
	}
}

void move_y_rev( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	for (uint8_t i = 1; i < 8; i++){

		for (uint8_t j = x1; j <= x2; j++){
			
			for (uint8_t k = y1; k <= y2; k++) {
				if ((cube[i][j] & (1 << k))){
					cube[i-1][j] |= (1 << k);
					cube[i][j] &= ~(1 << k);
				} else {
					cube[i-1][j] &= ~(1 << k);
				}
			}

		}

	}
}

void move_x_fwd( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	for (uint8_t layer = y1; layer <= y2; layer++) {
		for (uint8_t j = 7; j > 0; j--){
			for (uint8_t k = x1; k <= x2; k++) {
				if ((cube[layer][j-1] & (1 << k))){
					cube[layer][j] |= (1 << k);
					cube[layer][j-1] &= ~(1 << k);
				} else {
					/* delete the pixel anyway */
					cube[layer][j] &= ~(1 << k);
				}
			}

		}
	}
}

void move_x_rev( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{

	for (uint8_t layer = y1; layer <= y2; layer++) {
		for (uint8_t j = 0; j < 7; j++){
			for (uint8_t k = x1; k <= x2; k++) {
				if ((cube[layer][j+1] & (1 << k))){
					cube[layer][j] |= (1 << k);
					cube[layer][j+1] &= ~(1 << k);
				} else {
					cube[layer][j] &= ~(1 << k);
				}
			}

		}
	}
}

