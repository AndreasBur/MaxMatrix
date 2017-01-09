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
#include "ledcube.h"
#include "animations.h"
#include "rotation.h"
/* 
   fscking idea. 
   to get a defined state, every rotation must be divisible
   by 90 degrees. Im doing 5x15deg steps precalculated for
   every single pix per layer. Every step depends on the
   original array, not on the last step.
   This function uses the y axis.

   the perlscript used to calculate the transformation:
   (change the value in deg2rad() for the steps)

#!/usr/bin/perl -w
use strict;
use warnings;
use Math::Trig;
use POSIX;

my $deg = deg2rad(75);
my $x;
my $y;
for ($x = -3.5; $x < 4.5; $x += 1) {
        for ($y = -3.5; $y < 4.5; $y += 1) {
                my $newx = ($x*cos($deg) + $y*sin($deg))+3.5;
                my $newy = ($x*-1*sin($deg) + $y*cos($deg))+3.5;
                if ($newx >= 0) { $newx = floor($newx + 0.5); }
                else            { $newx = ceil($newx - 0.5); }
                if ($newy >= 0) { $newy = floor($newy + 0.5); }
                else            { $newy = ceil($newy - 0.5); }
                if ( $newx >= 0 && $newx <= 7 && $newy >= 0 && $newy <= 7) {
                        printf("if ( cube_org[i][" . ($x+3.5) . "] & (1<< ");
                        printf(($y+3.5) . " ) )\n   cube[i][$newx] |= ( 1 << $newy);\n");
                }
        }
}

*/

void rotate_90_auto ( uint8_t cycle )
{
	for (uint8_t loopcnt = 0; loopcnt < cycle; loopcnt++) {
		uint8_t cube_org[8][8];
		for (uint8_t i = 0; i < 8; i++){
			for (uint8_t j = 0; j < 8; j++){
			     cube_org[i][j] = cube[i][j];
			}
		}
		cube_clear();
		/* 15 deg */
		for (uint8_t i = 0; i < 8; i++){
			if ( cube_org[i][0] & (1 << 2 ) )
			   cube[i][0] |= ( 1 << 3);
			if ( cube_org[i][0] & (1 << 3 ) )
			   cube[i][0] |= ( 1 << 4);
			if ( cube_org[i][0] & (1 << 4 ) )
			   cube[i][0] |= ( 1 << 5);
			if ( cube_org[i][0] & (1 << 5 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 6 ) )
			   cube[i][1] |= ( 1 << 7);
			if ( cube_org[i][1] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 1);
			if ( cube_org[i][1] & (1 << 1 ) )
			   cube[i][0] |= ( 1 << 2);
			if ( cube_org[i][1] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 3);
			if ( cube_org[i][1] & (1 << 3 ) )
			   cube[i][1] |= ( 1 << 4);
			if ( cube_org[i][1] & (1 << 4 ) )
			   cube[i][1] |= ( 1 << 5);
			if ( cube_org[i][1] & (1 << 5 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 6 ) )
			   cube[i][2] |= ( 1 << 7);
			if ( cube_org[i][2] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][2] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][2] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 2);
			if ( cube_org[i][2] & (1 << 3 ) )
			   cube[i][2] |= ( 1 << 3);
			if ( cube_org[i][2] & (1 << 4 ) )
			   cube[i][2] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 5 ) )
			   cube[i][2] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 6 ) )
			   cube[i][3] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 7 ) )
			   cube[i][3] |= ( 1 << 7);
			if ( cube_org[i][3] & (1 << 0 ) )
			   cube[i][2] |= ( 1 << 0);
			if ( cube_org[i][3] & (1 << 1 ) )
			   cube[i][2] |= ( 1 << 1);
			if ( cube_org[i][3] & (1 << 2 ) )
			   cube[i][3] |= ( 1 << 2);
			if ( cube_org[i][3] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 3);
			if ( cube_org[i][3] & (1 << 4 ) )
			   cube[i][3] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 5 ) )
			   cube[i][3] |= ( 1 << 5);
			if ( cube_org[i][3] & (1 << 6 ) )
			   cube[i][4] |= ( 1 << 6);
			if ( cube_org[i][3] & (1 << 7 ) )
			   cube[i][4] |= ( 1 << 7);
			if ( cube_org[i][4] & (1 << 0 ) )
			   cube[i][3] |= ( 1 << 0);
			if ( cube_org[i][4] & (1 << 1 ) )
			   cube[i][3] |= ( 1 << 1);
			if ( cube_org[i][4] & (1 << 2 ) )
			   cube[i][4] |= ( 1 << 2);
			if ( cube_org[i][4] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 4);
			if ( cube_org[i][4] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 5);
			if ( cube_org[i][4] & (1 << 6 ) )
			   cube[i][5] |= ( 1 << 6);
			if ( cube_org[i][4] & (1 << 7 ) )
			   cube[i][5] |= ( 1 << 7);
			if ( cube_org[i][5] & (1 << 0 ) )
			   cube[i][4] |= ( 1 << 0);
			if ( cube_org[i][5] & (1 << 1 ) )
			   cube[i][4] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 2 ) )
			   cube[i][5] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 3 ) )
			   cube[i][5] |= ( 1 << 3);
			if ( cube_org[i][5] & (1 << 4 ) )
			   cube[i][5] |= ( 1 << 4);
			if ( cube_org[i][5] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 5);
			if ( cube_org[i][5] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][5] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][6] & (1 << 1 ) )
			   cube[i][5] |= ( 1 << 0);
			if ( cube_org[i][6] & (1 << 2 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 3 ) )
			   cube[i][6] |= ( 1 << 2);
			if ( cube_org[i][6] & (1 << 4 ) )
			   cube[i][6] |= ( 1 << 3);
			if ( cube_org[i][6] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 4);
			if ( cube_org[i][6] & (1 << 6 ) )
			   cube[i][7] |= ( 1 << 5);
			if ( cube_org[i][6] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 6);
			if ( cube_org[i][7] & (1 << 1 ) )
			   cube[i][6] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 2 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][7] & (1 << 3 ) )
			   cube[i][7] |= ( 1 << 2);
			if ( cube_org[i][7] & (1 << 4 ) )
			   cube[i][7] |= ( 1 << 3);
			if ( cube_org[i][7] & (1 << 5 ) )
			   cube[i][7] |= ( 1 << 4);
		}
		cube_show_loop(CL_LOOP_DELAY);
		cube_clear();	
		/* 30 deg */
		for (uint8_t i = 0; i < 8; i++){
			if ( cube_org[i][0] & (1 << 2 ) )
			   cube[i][0] |= ( 1 << 4);
			if ( cube_org[i][0] & (1 << 3 ) )
			   cube[i][0] |= ( 1 << 5);
			if ( cube_org[i][0] & (1 << 4 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 5 ) )
			   cube[i][1] |= ( 1 << 7);
			if ( cube_org[i][0] & (1 << 6 ) )
			   cube[i][2] |= ( 1 << 7);
			if ( cube_org[i][1] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 2);
			if ( cube_org[i][1] & (1 << 1 ) )
			   cube[i][0] |= ( 1 << 3);
			if ( cube_org[i][1] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 3);
			if ( cube_org[i][1] & (1 << 3 ) )
			   cube[i][1] |= ( 1 << 4);
			if ( cube_org[i][1] & (1 << 4 ) )
			   cube[i][2] |= ( 1 << 5);
			if ( cube_org[i][1] & (1 << 5 ) )
			   cube[i][2] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 6 ) )
			   cube[i][3] |= ( 1 << 7);
			if ( cube_org[i][2] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 1);
			if ( cube_org[i][2] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 2);
			if ( cube_org[i][2] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 3);
			if ( cube_org[i][2] & (1 << 3 ) )
			   cube[i][2] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 4 ) )
			   cube[i][2] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 5 ) )
			   cube[i][3] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 6 ) )
			   cube[i][3] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 7 ) )
			   cube[i][4] |= ( 1 << 7);
			if ( cube_org[i][3] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][3] & (1 << 1 ) )
			   cube[i][2] |= ( 1 << 2);
			if ( cube_org[i][3] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 2);
			if ( cube_org[i][3] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 3);
			if ( cube_org[i][3] & (1 << 4 ) )
			   cube[i][3] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 5);
			if ( cube_org[i][3] & (1 << 6 ) )
			   cube[i][4] |= ( 1 << 6);
			if ( cube_org[i][3] & (1 << 7 ) )
			   cube[i][5] |= ( 1 << 7);
			if ( cube_org[i][4] & (1 << 0 ) )
			   cube[i][2] |= ( 1 << 0);
			if ( cube_org[i][4] & (1 << 1 ) )
			   cube[i][3] |= ( 1 << 1);
			if ( cube_org[i][4] & (1 << 2 ) )
			   cube[i][3] |= ( 1 << 2);
			if ( cube_org[i][4] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 4);
			if ( cube_org[i][4] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 5);
			if ( cube_org[i][4] & (1 << 6 ) )
			   cube[i][5] |= ( 1 << 5);
			if ( cube_org[i][4] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][5] & (1 << 0 ) )
			   cube[i][3] |= ( 1 << 0);
			if ( cube_org[i][5] & (1 << 1 ) )
			   cube[i][4] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 2 ) )
			   cube[i][4] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 3 ) )
			   cube[i][5] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 4 ) )
			   cube[i][5] |= ( 1 << 3);
			if ( cube_org[i][5] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 4);
			if ( cube_org[i][5] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 5);
			if ( cube_org[i][5] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 6);
			if ( cube_org[i][6] & (1 << 1 ) )
			   cube[i][4] |= ( 1 << 0);
			if ( cube_org[i][6] & (1 << 2 ) )
			   cube[i][5] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 3 ) )
			   cube[i][5] |= ( 1 << 2);
			if ( cube_org[i][6] & (1 << 4 ) )
			   cube[i][6] |= ( 1 << 3);
			if ( cube_org[i][6] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 4);
			if ( cube_org[i][6] & (1 << 6 ) )
			   cube[i][7] |= ( 1 << 4);
			if ( cube_org[i][6] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 5);
			if ( cube_org[i][7] & (1 << 1 ) )
			   cube[i][5] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 2 ) )
			   cube[i][6] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 3 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][7] & (1 << 4 ) )
			   cube[i][7] |= ( 1 << 2);
			if ( cube_org[i][7] & (1 << 5 ) )
			   cube[i][7] |= ( 1 << 3);
		}
		cube_show_loop(CL_LOOP_DELAY);
		cube_clear();

		/* 45 deg */
		for (uint8_t i = 0; i < 8; i++){
			if ( cube_org[i][0] & (1 << 2 ) )
			   cube[i][0] |= ( 1 << 5);
			if ( cube_org[i][0] & (1 << 3 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 4 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 5 ) )
			   cube[i][2] |= ( 1 << 7);
			if ( cube_org[i][1] & (1 << 1 ) )
			   cube[i][0] |= ( 1 << 4);
			if ( cube_org[i][1] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 4);
			if ( cube_org[i][1] & (1 << 3 ) )
			   cube[i][1] |= ( 1 << 5);
			if ( cube_org[i][1] & (1 << 4 ) )
			   cube[i][2] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 5 ) )
			   cube[i][3] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 6 ) )
			   cube[i][4] |= ( 1 << 7);
			if ( cube_org[i][2] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 2);
			if ( cube_org[i][2] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 3);
			if ( cube_org[i][2] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 3 ) )
			   cube[i][2] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 4 ) )
			   cube[i][3] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 6 ) )
			   cube[i][4] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 7 ) )
			   cube[i][5] |= ( 1 << 7);
			if ( cube_org[i][3] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][3] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 2);
			if ( cube_org[i][3] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 3);
			if ( cube_org[i][3] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 5);
			if ( cube_org[i][3] & (1 << 6 ) )
			   cube[i][5] |= ( 1 << 6);
			if ( cube_org[i][3] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][4] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][4] & (1 << 1 ) )
			   cube[i][2] |= ( 1 << 1);
			if ( cube_org[i][4] & (1 << 2 ) )
			   cube[i][3] |= ( 1 << 2);
			if ( cube_org[i][4] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 4);
			if ( cube_org[i][4] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 4);
			if ( cube_org[i][4] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 5);
			if ( cube_org[i][4] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][5] & (1 << 0 ) )
			   cube[i][2] |= ( 1 << 0);
			if ( cube_org[i][5] & (1 << 1 ) )
			   cube[i][3] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 2 ) )
			   cube[i][4] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 4 ) )
			   cube[i][5] |= ( 1 << 3);
			if ( cube_org[i][5] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 4);
			if ( cube_org[i][5] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 4);
			if ( cube_org[i][5] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 5);
			if ( cube_org[i][6] & (1 << 1 ) )
			   cube[i][4] |= ( 1 << 0);
			if ( cube_org[i][6] & (1 << 2 ) )
			   cube[i][4] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 3 ) )
			   cube[i][5] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 4 ) )
			   cube[i][6] |= ( 1 << 2);
			if ( cube_org[i][6] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 3);
			if ( cube_org[i][6] & (1 << 6 ) )
			   cube[i][7] |= ( 1 << 4);
			if ( cube_org[i][7] & (1 << 2 ) )
			   cube[i][5] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 3 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][7] & (1 << 4 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][7] & (1 << 5 ) )
			   cube[i][7] |= ( 1 << 2);

		}
		cube_show_loop(CL_LOOP_DELAY);
		cube_clear();
		/* 60 deg */
		for (uint8_t i = 0; i < 8; i++){
			if ( cube_org[i][0] & (1 << 1 ) )
			   cube[i][0] |= ( 1 << 5);
			if ( cube_org[i][0] & (1 << 2 ) )
			   cube[i][0] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 3 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 4 ) )
			   cube[i][2] |= ( 1 << 7);
			if ( cube_org[i][0] & (1 << 5 ) )
			   cube[i][3] |= ( 1 << 7);
			if ( cube_org[i][1] & (1 << 1 ) )
			   cube[i][0] |= ( 1 << 4);
			if ( cube_org[i][1] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 5);
			if ( cube_org[i][1] & (1 << 3 ) )
			   cube[i][2] |= ( 1 << 5);
			if ( cube_org[i][1] & (1 << 4 ) )
			   cube[i][3] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 6 ) )
			   cube[i][4] |= ( 1 << 7);
			if ( cube_org[i][1] & (1 << 7 ) )
			   cube[i][5] |= ( 1 << 7);
			if ( cube_org[i][2] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 3);
			if ( cube_org[i][2] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 3 ) )
			   cube[i][2] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 4 ) )
			   cube[i][3] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 6 ) )
			   cube[i][5] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 7);
			if ( cube_org[i][3] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 2);
			if ( cube_org[i][3] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 3);
			if ( cube_org[i][3] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 3);
			if ( cube_org[i][3] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 5);
			if ( cube_org[i][3] & (1 << 6 ) )
			   cube[i][5] |= ( 1 << 5);
			if ( cube_org[i][3] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][4] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][4] & (1 << 1 ) )
			   cube[i][2] |= ( 1 << 2);
			if ( cube_org[i][4] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 2);
			if ( cube_org[i][4] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 4);
			if ( cube_org[i][4] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 4);
			if ( cube_org[i][4] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 5);
			if ( cube_org[i][5] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 0);
			if ( cube_org[i][5] & (1 << 1 ) )
			   cube[i][2] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 2 ) )
			   cube[i][3] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 4 ) )
			   cube[i][5] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 3);
			if ( cube_org[i][5] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 3);
			if ( cube_org[i][5] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 4);
			if ( cube_org[i][6] & (1 << 0 ) )
			   cube[i][2] |= ( 1 << 0);
			if ( cube_org[i][6] & (1 << 1 ) )
			   cube[i][3] |= ( 1 << 0);
			if ( cube_org[i][6] & (1 << 2 ) )
			   cube[i][3] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 4 ) )
			   cube[i][5] |= ( 1 << 2);
			if ( cube_org[i][6] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 2);
			if ( cube_org[i][6] & (1 << 6 ) )
			   cube[i][7] |= ( 1 << 3);
			if ( cube_org[i][7] & (1 << 2 ) )
			   cube[i][4] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 3 ) )
			   cube[i][5] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 4 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][7] & (1 << 5 ) )
			   cube[i][7] |= ( 1 << 1);
			if ( cube_org[i][7] & (1 << 6 ) )
			   cube[i][7] |= ( 1 << 2);

		}

		cube_show_loop(CL_LOOP_DELAY);
		cube_clear();
		/* 75 deg */
		for (uint8_t i = 0; i < 8; i++){
			if ( cube_org[i][0] & (1 << 1 ) )
			   cube[i][0] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][0] & (1 << 3 ) )
			   cube[i][2] |= ( 1 << 7);
			if ( cube_org[i][0] & (1 << 4 ) )
			   cube[i][3] |= ( 1 << 7);
			if ( cube_org[i][0] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 7);
			if ( cube_org[i][1] & (1 << 1 ) )
			   cube[i][0] |= ( 1 << 5);
			if ( cube_org[i][1] & (1 << 2 ) )
			   cube[i][1] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 3 ) )
			   cube[i][2] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 4 ) )
			   cube[i][3] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 5 ) )
			   cube[i][4] |= ( 1 << 6);
			if ( cube_org[i][1] & (1 << 6 ) )
			   cube[i][5] |= ( 1 << 7);
			if ( cube_org[i][1] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 7);
			if ( cube_org[i][2] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 4);
			if ( cube_org[i][2] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 5);
			if ( cube_org[i][2] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][2] & (1 << 7 ) )
			   cube[i][6] |= ( 1 << 6);
			if ( cube_org[i][3] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 3);
			if ( cube_org[i][3] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 3);
			if ( cube_org[i][3] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 4);
			if ( cube_org[i][3] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 5);
			if ( cube_org[i][3] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 5);
			if ( cube_org[i][4] & (1 << 0 ) )
			   cube[i][0] |= ( 1 << 2);
			if ( cube_org[i][4] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 2);
			if ( cube_org[i][4] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 3);
			if ( cube_org[i][4] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 4);
			if ( cube_org[i][4] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 4);
			if ( cube_org[i][5] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 1 ) )
			   cube[i][1] |= ( 1 << 1);
			if ( cube_org[i][5] & (1 << 2 ) )
			   cube[i][2] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 3 ) )
			   cube[i][3] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 4 ) )
			   cube[i][4] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 5 ) )
			   cube[i][5] |= ( 1 << 2);
			if ( cube_org[i][5] & (1 << 6 ) )
			   cube[i][6] |= ( 1 << 3);
			if ( cube_org[i][5] & (1 << 7 ) )
			   cube[i][7] |= ( 1 << 3);
			if ( cube_org[i][6] & (1 << 0 ) )
			   cube[i][1] |= ( 1 << 0);
			if ( cube_org[i][6] & (1 << 1 ) )
			   cube[i][2] |= ( 1 << 0);
			if ( cube_org[i][6] & (1 << 2 ) )
			   cube[i][3] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 4 ) )
			   cube[i][5] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][6] & (1 << 6 ) )
			   cube[i][7] |= ( 1 << 2);
			if ( cube_org[i][7] & (1 << 2 ) )
			   cube[i][3] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 3 ) )
			   cube[i][4] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 4 ) )
			   cube[i][5] |= ( 1 << 0);
			if ( cube_org[i][7] & (1 << 5 ) )
			   cube[i][6] |= ( 1 << 1);
			if ( cube_org[i][7] & (1 << 6 ) )
			   cube[i][7] |= ( 1 << 1);

		}
		cube_show_loop(CL_LOOP_DELAY);
		cube_clear();

		for ( uint8_t layer = 0; layer < 8; layer++ ) {
			for ( uint8_t x = 0; x < 8; x++) {
				for ( uint8_t y = 0; y < 8; y++) {
					if ( cube_org[layer][y] & (0x80 >> x)) 
						cube[layer][(7-x)] |= (1 << (7-y));
				}
			}
		}
		cube_show_loop(CL_LOOP_DELAY);
	}
}

/*
void rotate_15_deg( void )
{
	uint8_t cube_org[8][8];
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
		     cube_org[i][j] = cube[i][j];
		}
	}
	cube_clear();

	for (uint8_t i = 0; i < 8; i++){
		if ( cube_org[i][0] & (1 << 2 ) )
		   cube[i][0] |= ( 1 << 3);
		if ( cube_org[i][0] & (1 << 3 ) )
		   cube[i][0] |= ( 1 << 4);
		if ( cube_org[i][0] & (1 << 4 ) )
		   cube[i][0] |= ( 1 << 5);
		if ( cube_org[i][0] & (1 << 5 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 6 ) )
		   cube[i][1] |= ( 1 << 7);
		if ( cube_org[i][1] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 1);
		if ( cube_org[i][1] & (1 << 1 ) )
		   cube[i][0] |= ( 1 << 2);
		if ( cube_org[i][1] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 3);
		if ( cube_org[i][1] & (1 << 3 ) )
		   cube[i][1] |= ( 1 << 4);
		if ( cube_org[i][1] & (1 << 4 ) )
		   cube[i][1] |= ( 1 << 5);
		if ( cube_org[i][1] & (1 << 5 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 6 ) )
		   cube[i][2] |= ( 1 << 7);
		if ( cube_org[i][2] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][2] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][2] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 2);
		if ( cube_org[i][2] & (1 << 3 ) )
		   cube[i][2] |= ( 1 << 3);
		if ( cube_org[i][2] & (1 << 4 ) )
		   cube[i][2] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 5 ) )
		   cube[i][2] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 6 ) )
		   cube[i][3] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 7 ) )
		   cube[i][3] |= ( 1 << 7);
		if ( cube_org[i][3] & (1 << 0 ) )
		   cube[i][2] |= ( 1 << 0);
		if ( cube_org[i][3] & (1 << 1 ) )
		   cube[i][2] |= ( 1 << 1);
		if ( cube_org[i][3] & (1 << 2 ) )
		   cube[i][3] |= ( 1 << 2);
		if ( cube_org[i][3] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 3);
		if ( cube_org[i][3] & (1 << 4 ) )
		   cube[i][3] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 5 ) )
		   cube[i][3] |= ( 1 << 5);
		if ( cube_org[i][3] & (1 << 6 ) )
		   cube[i][4] |= ( 1 << 6);
		if ( cube_org[i][3] & (1 << 7 ) )
		   cube[i][4] |= ( 1 << 7);
		if ( cube_org[i][4] & (1 << 0 ) )
		   cube[i][3] |= ( 1 << 0);
		if ( cube_org[i][4] & (1 << 1 ) )
		   cube[i][3] |= ( 1 << 1);
		if ( cube_org[i][4] & (1 << 2 ) )
		   cube[i][4] |= ( 1 << 2);
		if ( cube_org[i][4] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 4);
		if ( cube_org[i][4] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 5);
		if ( cube_org[i][4] & (1 << 6 ) )
		   cube[i][5] |= ( 1 << 6);
		if ( cube_org[i][4] & (1 << 7 ) )
		   cube[i][5] |= ( 1 << 7);
		if ( cube_org[i][5] & (1 << 0 ) )
		   cube[i][4] |= ( 1 << 0);
		if ( cube_org[i][5] & (1 << 1 ) )
		   cube[i][4] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 2 ) )
		   cube[i][5] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 3 ) )
		   cube[i][5] |= ( 1 << 3);
		if ( cube_org[i][5] & (1 << 4 ) )
		   cube[i][5] |= ( 1 << 4);
		if ( cube_org[i][5] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 5);
		if ( cube_org[i][5] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][5] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][6] & (1 << 1 ) )
		   cube[i][5] |= ( 1 << 0);
		if ( cube_org[i][6] & (1 << 2 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 3 ) )
		   cube[i][6] |= ( 1 << 2);
		if ( cube_org[i][6] & (1 << 4 ) )
		   cube[i][6] |= ( 1 << 3);
		if ( cube_org[i][6] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 4);
		if ( cube_org[i][6] & (1 << 6 ) )
		   cube[i][7] |= ( 1 << 5);
		if ( cube_org[i][6] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 6);
		if ( cube_org[i][7] & (1 << 1 ) )
		   cube[i][6] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 2 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][7] & (1 << 3 ) )
		   cube[i][7] |= ( 1 << 2);
		if ( cube_org[i][7] & (1 << 4 ) )
		   cube[i][7] |= ( 1 << 3);
		if ( cube_org[i][7] & (1 << 5 ) )
		   cube[i][7] |= ( 1 << 4);
	}
}


void rotate_30_deg( void )
{
	uint8_t cube_org[8][8];
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
		     cube_org[i][j] = cube[i][j];
		}
	}
	cube_clear();

	for (uint8_t i = 0; i < 8; i++){
		if ( cube_org[i][0] & (1 << 2 ) )
		   cube[i][0] |= ( 1 << 4);
		if ( cube_org[i][0] & (1 << 3 ) )
		   cube[i][0] |= ( 1 << 5);
		if ( cube_org[i][0] & (1 << 4 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 5 ) )
		   cube[i][1] |= ( 1 << 7);
		if ( cube_org[i][0] & (1 << 6 ) )
		   cube[i][2] |= ( 1 << 7);
		if ( cube_org[i][1] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 2);
		if ( cube_org[i][1] & (1 << 1 ) )
		   cube[i][0] |= ( 1 << 3);
		if ( cube_org[i][1] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 3);
		if ( cube_org[i][1] & (1 << 3 ) )
		   cube[i][1] |= ( 1 << 4);
		if ( cube_org[i][1] & (1 << 4 ) )
		   cube[i][2] |= ( 1 << 5);
		if ( cube_org[i][1] & (1 << 5 ) )
		   cube[i][2] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 6 ) )
		   cube[i][3] |= ( 1 << 7);
		if ( cube_org[i][2] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 1);
		if ( cube_org[i][2] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 2);
		if ( cube_org[i][2] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 3);
		if ( cube_org[i][2] & (1 << 3 ) )
		   cube[i][2] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 4 ) )
		   cube[i][2] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 5 ) )
		   cube[i][3] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 6 ) )
		   cube[i][3] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 7 ) )
		   cube[i][4] |= ( 1 << 7);
		if ( cube_org[i][3] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][3] & (1 << 1 ) )
		   cube[i][2] |= ( 1 << 2);
		if ( cube_org[i][3] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 2);
		if ( cube_org[i][3] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 3);
		if ( cube_org[i][3] & (1 << 4 ) )
		   cube[i][3] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 5);
		if ( cube_org[i][3] & (1 << 6 ) )
		   cube[i][4] |= ( 1 << 6);
		if ( cube_org[i][3] & (1 << 7 ) )
		   cube[i][5] |= ( 1 << 7);
		if ( cube_org[i][4] & (1 << 0 ) )
		   cube[i][2] |= ( 1 << 0);
		if ( cube_org[i][4] & (1 << 1 ) )
		   cube[i][3] |= ( 1 << 1);
		if ( cube_org[i][4] & (1 << 2 ) )
		   cube[i][3] |= ( 1 << 2);
		if ( cube_org[i][4] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 4);
		if ( cube_org[i][4] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 5);
		if ( cube_org[i][4] & (1 << 6 ) )
		   cube[i][5] |= ( 1 << 5);
		if ( cube_org[i][4] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][5] & (1 << 0 ) )
		   cube[i][3] |= ( 1 << 0);
		if ( cube_org[i][5] & (1 << 1 ) )
		   cube[i][4] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 2 ) )
		   cube[i][4] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 3 ) )
		   cube[i][5] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 4 ) )
		   cube[i][5] |= ( 1 << 3);
		if ( cube_org[i][5] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 4);
		if ( cube_org[i][5] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 5);
		if ( cube_org[i][5] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 6);
		if ( cube_org[i][6] & (1 << 1 ) )
		   cube[i][4] |= ( 1 << 0);
		if ( cube_org[i][6] & (1 << 2 ) )
		   cube[i][5] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 3 ) )
		   cube[i][5] |= ( 1 << 2);
		if ( cube_org[i][6] & (1 << 4 ) )
		   cube[i][6] |= ( 1 << 3);
		if ( cube_org[i][6] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 4);
		if ( cube_org[i][6] & (1 << 6 ) )
		   cube[i][7] |= ( 1 << 4);
		if ( cube_org[i][6] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 5);
		if ( cube_org[i][7] & (1 << 1 ) )
		   cube[i][5] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 2 ) )
		   cube[i][6] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 3 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][7] & (1 << 4 ) )
		   cube[i][7] |= ( 1 << 2);
		if ( cube_org[i][7] & (1 << 5 ) )
		   cube[i][7] |= ( 1 << 3);
	}
}

void rotate_45_deg( void )
{
	uint8_t cube_org[8][8];
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
		     cube_org[i][j] = cube[i][j];
		}
	}
	cube_clear();
	for (uint8_t i = 0; i < 8; i++){
		if ( cube_org[i][0] & (1 << 2 ) )
		   cube[i][0] |= ( 1 << 5);
		if ( cube_org[i][0] & (1 << 3 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 4 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 5 ) )
		   cube[i][2] |= ( 1 << 7);
		if ( cube_org[i][1] & (1 << 1 ) )
		   cube[i][0] |= ( 1 << 4);
		if ( cube_org[i][1] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 4);
		if ( cube_org[i][1] & (1 << 3 ) )
		   cube[i][1] |= ( 1 << 5);
		if ( cube_org[i][1] & (1 << 4 ) )
		   cube[i][2] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 5 ) )
		   cube[i][3] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 6 ) )
		   cube[i][4] |= ( 1 << 7);
		if ( cube_org[i][2] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 2);
		if ( cube_org[i][2] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 3);
		if ( cube_org[i][2] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 3 ) )
		   cube[i][2] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 4 ) )
		   cube[i][3] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 6 ) )
		   cube[i][4] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 7 ) )
		   cube[i][5] |= ( 1 << 7);
		if ( cube_org[i][3] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][3] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 2);
		if ( cube_org[i][3] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 3);
		if ( cube_org[i][3] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 5);
		if ( cube_org[i][3] & (1 << 6 ) )
		   cube[i][5] |= ( 1 << 6);
		if ( cube_org[i][3] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][4] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][4] & (1 << 1 ) )
		   cube[i][2] |= ( 1 << 1);
		if ( cube_org[i][4] & (1 << 2 ) )
		   cube[i][3] |= ( 1 << 2);
		if ( cube_org[i][4] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 4);
		if ( cube_org[i][4] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 4);
		if ( cube_org[i][4] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 5);
		if ( cube_org[i][4] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][5] & (1 << 0 ) )
		   cube[i][2] |= ( 1 << 0);
		if ( cube_org[i][5] & (1 << 1 ) )
		   cube[i][3] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 2 ) )
		   cube[i][4] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 4 ) )
		   cube[i][5] |= ( 1 << 3);
		if ( cube_org[i][5] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 4);
		if ( cube_org[i][5] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 4);
		if ( cube_org[i][5] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 5);
		if ( cube_org[i][6] & (1 << 1 ) )
		   cube[i][4] |= ( 1 << 0);
		if ( cube_org[i][6] & (1 << 2 ) )
		   cube[i][4] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 3 ) )
		   cube[i][5] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 4 ) )
		   cube[i][6] |= ( 1 << 2);
		if ( cube_org[i][6] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 3);
		if ( cube_org[i][6] & (1 << 6 ) )
		   cube[i][7] |= ( 1 << 4);
		if ( cube_org[i][7] & (1 << 2 ) )
		   cube[i][5] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 3 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][7] & (1 << 4 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][7] & (1 << 5 ) )
		   cube[i][7] |= ( 1 << 2);
	}
}

void rotate_60_deg( void )
{
	uint8_t cube_org[8][8];
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
		     cube_org[i][j] = cube[i][j];
		}
	}
	cube_clear();

	for (uint8_t i = 0; i < 8; i++){
		if ( cube_org[i][0] & (1 << 1 ) )
		   cube[i][0] |= ( 1 << 5);
		if ( cube_org[i][0] & (1 << 2 ) )
		   cube[i][0] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 3 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 4 ) )
		   cube[i][2] |= ( 1 << 7);
		if ( cube_org[i][0] & (1 << 5 ) )
		   cube[i][3] |= ( 1 << 7);
		if ( cube_org[i][1] & (1 << 1 ) )
		   cube[i][0] |= ( 1 << 4);
		if ( cube_org[i][1] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 5);
		if ( cube_org[i][1] & (1 << 3 ) )
		   cube[i][2] |= ( 1 << 5);
		if ( cube_org[i][1] & (1 << 4 ) )
		   cube[i][3] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 6 ) )
		   cube[i][4] |= ( 1 << 7);
		if ( cube_org[i][1] & (1 << 7 ) )
		   cube[i][5] |= ( 1 << 7);
		if ( cube_org[i][2] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 3);
		if ( cube_org[i][2] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 3 ) )
		   cube[i][2] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 4 ) )
		   cube[i][3] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 6 ) )
		   cube[i][5] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 7);
		if ( cube_org[i][3] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 2);
		if ( cube_org[i][3] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 3);
		if ( cube_org[i][3] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 3);
		if ( cube_org[i][3] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 5);
		if ( cube_org[i][3] & (1 << 6 ) )
		   cube[i][5] |= ( 1 << 5);
		if ( cube_org[i][3] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][4] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][4] & (1 << 1 ) )
		   cube[i][2] |= ( 1 << 2);
		if ( cube_org[i][4] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 2);
		if ( cube_org[i][4] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 4);
		if ( cube_org[i][4] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 4);
		if ( cube_org[i][4] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 5);
		if ( cube_org[i][5] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 0);
		if ( cube_org[i][5] & (1 << 1 ) )
		   cube[i][2] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 2 ) )
		   cube[i][3] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 4 ) )
		   cube[i][5] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 3);
		if ( cube_org[i][5] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 3);
		if ( cube_org[i][5] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 4);
		if ( cube_org[i][6] & (1 << 0 ) )
		   cube[i][2] |= ( 1 << 0);
		if ( cube_org[i][6] & (1 << 1 ) )
		   cube[i][3] |= ( 1 << 0);
		if ( cube_org[i][6] & (1 << 2 ) )
		   cube[i][3] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 4 ) )
		   cube[i][5] |= ( 1 << 2);
		if ( cube_org[i][6] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 2);
		if ( cube_org[i][6] & (1 << 6 ) )
		   cube[i][7] |= ( 1 << 3);
		if ( cube_org[i][7] & (1 << 2 ) )
		   cube[i][4] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 3 ) )
		   cube[i][5] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 4 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][7] & (1 << 5 ) )
		   cube[i][7] |= ( 1 << 1);
		if ( cube_org[i][7] & (1 << 6 ) )
		   cube[i][7] |= ( 1 << 2);

	}

}

void rotate_75_deg( void )
{
	uint8_t cube_org[8][8];
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
		     cube_org[i][j] = cube[i][j];
		}
	}
	cube_clear();
	for (uint8_t i = 0; i < 8; i++){
		if ( cube_org[i][0] & (1 << 1 ) )
		   cube[i][0] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][0] & (1 << 3 ) )
		   cube[i][2] |= ( 1 << 7);
		if ( cube_org[i][0] & (1 << 4 ) )
		   cube[i][3] |= ( 1 << 7);
		if ( cube_org[i][0] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 7);
		if ( cube_org[i][1] & (1 << 1 ) )
		   cube[i][0] |= ( 1 << 5);
		if ( cube_org[i][1] & (1 << 2 ) )
		   cube[i][1] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 3 ) )
		   cube[i][2] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 4 ) )
		   cube[i][3] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 5 ) )
		   cube[i][4] |= ( 1 << 6);
		if ( cube_org[i][1] & (1 << 6 ) )
		   cube[i][5] |= ( 1 << 7);
		if ( cube_org[i][1] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 7);
		if ( cube_org[i][2] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 4);
		if ( cube_org[i][2] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 5);
		if ( cube_org[i][2] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][2] & (1 << 7 ) )
		   cube[i][6] |= ( 1 << 6);
		if ( cube_org[i][3] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 3);
		if ( cube_org[i][3] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 3);
		if ( cube_org[i][3] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 4);
		if ( cube_org[i][3] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 5);
		if ( cube_org[i][3] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 5);
		if ( cube_org[i][4] & (1 << 0 ) )
		   cube[i][0] |= ( 1 << 2);
		if ( cube_org[i][4] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 2);
		if ( cube_org[i][4] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 3);
		if ( cube_org[i][4] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 4);
		if ( cube_org[i][4] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 4);
		if ( cube_org[i][5] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 1 ) )
		   cube[i][1] |= ( 1 << 1);
		if ( cube_org[i][5] & (1 << 2 ) )
		   cube[i][2] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 3 ) )
		   cube[i][3] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 4 ) )
		   cube[i][4] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 5 ) )
		   cube[i][5] |= ( 1 << 2);
		if ( cube_org[i][5] & (1 << 6 ) )
		   cube[i][6] |= ( 1 << 3);
		if ( cube_org[i][5] & (1 << 7 ) )
		   cube[i][7] |= ( 1 << 3);
		if ( cube_org[i][6] & (1 << 0 ) )
		   cube[i][1] |= ( 1 << 0);
		if ( cube_org[i][6] & (1 << 1 ) )
		   cube[i][2] |= ( 1 << 0);
		if ( cube_org[i][6] & (1 << 2 ) )
		   cube[i][3] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 4 ) )
		   cube[i][5] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][6] & (1 << 6 ) )
		   cube[i][7] |= ( 1 << 2);
		if ( cube_org[i][7] & (1 << 2 ) )
		   cube[i][3] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 3 ) )
		   cube[i][4] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 4 ) )
		   cube[i][5] |= ( 1 << 0);
		if ( cube_org[i][7] & (1 << 5 ) )
		   cube[i][6] |= ( 1 << 1);
		if ( cube_org[i][7] & (1 << 6 ) )
		   cube[i][7] |= ( 1 << 1);
	}
}
*/
void rotate_90_deg ( void )
{
	uint8_t cube_org[8][8];
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
		     cube_org[i][j] = cube[i][j];
		}
	}
	cube_clear();
	for ( uint8_t layer = 0; layer < 8; layer++ ) {
		for ( uint8_t x = 0; x < 8; x++) {
			for ( uint8_t y = 0; y < 8; y++) {
				if ( cube_org[layer][y] & (0x80 >> x)) 
					cube[layer][(7-x)] |= (1 << (7-y));
			}
		}
	}
}
