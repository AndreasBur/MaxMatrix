/*
fornax LED cube V1.1

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRANSLATION_H
#define TRANSLATION_H
/* y, x, z are the bits in x*/

#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>

void move_z_fwd ( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );
void move_z_rev ( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );
void move_y_fwd( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );
void move_y_rev( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );
void move_x_fwd( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );
void move_x_rev( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );

#endif
