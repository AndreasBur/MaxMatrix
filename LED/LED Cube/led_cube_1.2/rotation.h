/*
fornax LED cube V1.1

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ROTATION_H
#define ROTATION_H

#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>

#define CL_LOOP_DELAY 5 //was 6
void rotate_90_auto ( uint8_t cycle );

void rotate_15_deg( void );
void rotate_30_deg( void );
void rotate_45_deg( void );
void rotate_60_deg( void );
void rotate_75_deg( void );
void rotate_90_deg( void );

#endif
