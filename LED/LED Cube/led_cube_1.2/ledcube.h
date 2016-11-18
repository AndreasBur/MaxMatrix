/*
fornax LED cube V1.1

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LEDCUBE_H
#define LEDCUBE_H

#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>

/* this shouldnt be declared volatile, keep the code that way */
uint8_t cube[8][8];
/* y, x, z are the bits in x*/

void cube_show_init( void );

void cube_clear ( void );
void cube_clear_layer(uint8_t layer);
void cube_full ( void );
void cube_cube_3 ( void );
void cube_cube_4_line ( void );
void cube_random( void );
void cube_test_z( void );
void cube_test_y( void );
void cube_test_x( void );

void cube_show( void );
void cube_show_loop( uint8_t cycle );
void cube_show_loop_wo_int( uint8_t cycle );
#endif
