/*
fornax LED cube V1.1

Copyright (C) 2009 Andreas Fiessler aka fornax

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include "ledcube.h"
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "ab.h"


void cube_char( char ch, uint8_t z )
{
	switch (ch)
	{
		case '0':
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][2] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][3] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][4] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case '1':
		        cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][3] |= z;
			cube[2][3] |= z;
			cube[3][3] |= z;
			cube[4][3] |= z;
			cube[5][2] |= z;
			cube[5][3] |= z;
			cube[6][3] |= z;
			break;
		case '2':
		        cube[0][1] |= z;
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[0][5] |= z;
			cube[1][2] |= z;
			cube[2][3] |= z;
			cube[3][4] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case '3':
		        cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][5] |= z;
			cube[3][4] |= z;
			cube[4][3] |= z;
			cube[5][4] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case '4':
		        cube[0][4] |= z;
			cube[1][4] |= z;
			cube[2][1] |= z;
			cube[2][2] |= z;
			cube[2][3] |= z;
			cube[2][4] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][4] |= z;
			cube[4][2] |= z;
			cube[4][4] |= z;
			cube[5][3] |= z;
			cube[5][4] |= z;
			cube[6][4] |= z;
			break;
		case '5':
		        cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][5] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][2] |= z;
			cube[4][3] |= z;
			cube[4][4] |= z;
			cube[5][1] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case '6':
		        cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[5][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case '7':
		        cube[0][2] |= z;
			cube[1][2] |= z;
			cube[2][2] |= z;
			cube[3][3] |= z;
			cube[4][4] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case '8':
		        cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case '9':
		        cube[0][2] |= z;
			cube[0][3] |= z;
			cube[1][4] |= z;
			cube[2][5] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;

		case '!':
			cube[0][3] |= z;
			cube[3][3] |= z;
			cube[4][3] |= z;
			cube[5][2] |= z;
			cube[5][3] |= z;
			cube[5][4] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case ':':
			cube[1][2] |= z;
			cube[1][3] |= z;
			cube[2][2] |= z;
			cube[2][3] |= z;
			cube[4][2] |= z;
			cube[4][3] |= z;
			cube[5][2] |= z;
			cube[5][3] |= z;
			break;
		case '.':
			cube[1][2] |= z;
			cube[1][3] |= z;
			cube[2][2] |= z;
			cube[2][3] |= z;
			break;
		case '?':
			cube[0][3] |= z;
			cube[2][3] |= z;
			cube[3][4] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case '&':
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][4] |= z;
			cube[2][1] |= z;
			cube[2][3] |= z;
			cube[2][5] |= z;
			cube[3][2] |= z;
			cube[4][1] |= z;
			cube[4][3] |= z;
			cube[5][1] |= z;
			cube[5][4] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			break;
		case 'a':
		        cube[0][1] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][2] |= z;
			cube[2][3] |= z;
			cube[2][4] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'b':
		        cube[0][1] |= z;
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'c':
		        cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][5] |= z;
			cube[1][1] |= z;
			cube[2][1] |= z;
			cube[3][1] |= z;
			cube[4][1] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[5][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'd':
		        cube[0][1] |= z;
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[1][1] |= z;
			cube[1][4] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][4] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			break;
		case 'e':
			cube[0][1] |= z;
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[2][1] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[5][1] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case 'f':
			cube[0][1] |= z;
			cube[1][1] |= z;
			cube[2][1] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[5][1] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case 'g':
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'h':
			cube[0][1] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'i':
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][3] |= z;
			cube[2][3] |= z;
			cube[3][3] |= z;
			cube[4][3] |= z;
			cube[5][3] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'j':
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[1][1] |= z;
			cube[1][4] |= z;
			cube[2][4] |= z;
			cube[3][4] |= z;
			cube[4][4] |= z;
			cube[5][4] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case 'k':
			cube[0][1] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][4] |= z;
			cube[2][1] |= z;
			cube[2][3] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[4][1] |= z;
			cube[4][3] |= z;
			cube[5][1] |= z;
			cube[5][4] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'l':
			cube[0][1] |= z;
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[2][1] |= z;
			cube[3][1] |= z;
			cube[4][1] |= z;
			cube[5][1] |= z;
			cube[6][1] |= z;
			break;
		case 'm':
			cube[0][1] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][3] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][3] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][2] |= z;
			cube[5][4] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'n':
			cube[0][1] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][4] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][3] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][2] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'o':
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'p':
			cube[0][1] |= z;
			cube[1][1] |= z;
			cube[2][1] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'q': 
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][4] |= z;
			cube[2][1] |= z;
			cube[2][3] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 'r':
			cube[0][1] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][4] |= z;
			cube[2][1] |= z;
			cube[2][3] |= z;
			cube[3][1] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			break;
		case 's':
			cube[0][1] |= z;
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][5] |= z;
			cube[2][5] |= z;
			cube[3][2] |= z;
			cube[3][3] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[5][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case 't':
			cube[0][3] |= z;
			cube[1][3] |= z;
			cube[2][3] |= z;
			cube[3][3] |= z;
			cube[4][3] |= z;
			cube[5][3] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		case 'u':
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'v':
			cube[0][3] |= z;
			cube[1][2] |= z;
			cube[1][4] |= z;
			cube[2][1] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'w':
			cube[0][2] |= z;
			cube[0][4] |= z;
			cube[1][1] |= z;
			cube[1][3] |= z;
			cube[1][5] |= z;
			cube[2][1] |= z;
			cube[2][3] |= z;
			cube[2][5] |= z;
			cube[3][1] |= z;
			cube[3][3] |= z;
			cube[3][5] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'x': 
			cube[0][1] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[1][5] |= z;
			cube[2][2] |= z;
			cube[2][4] |= z;
			cube[3][3] |= z;
			cube[4][2] |= z;
			cube[4][4] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'y':
			cube[0][3] |= z;
			cube[1][3] |= z;
			cube[2][3] |= z;
			cube[3][2] |= z;
			cube[3][4] |= z;
			cube[4][1] |= z;
			cube[4][5] |= z;
			cube[5][1] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][5] |= z;
			break;
		case 'z':
			cube[0][1] |= z;
			cube[0][2] |= z;
			cube[0][3] |= z;
			cube[0][4] |= z;
			cube[0][5] |= z;
			cube[1][1] |= z;
			cube[2][2] |= z;
			cube[3][3] |= z;
			cube[4][4] |= z;
			cube[5][5] |= z;
			cube[6][1] |= z;
			cube[6][2] |= z;
			cube[6][3] |= z;
			cube[6][4] |= z;
			cube[6][5] |= z;
			break;
		default:
			break;
	}
}

