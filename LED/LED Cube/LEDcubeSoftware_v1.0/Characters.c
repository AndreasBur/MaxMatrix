/*

LED cube v1.0
Characters.c

Copyright (C) 2012 Alexander
 
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */ 


// include
#include <stdint.h>

// Variablendefinition
volatile uint8_t charArray[5];



// Liefert ein Zeichen zurück
// Die Zeichen werden "quer" gespeichert , um Speicherplatz zu sparen
// Die Daten müssen nachträglich noch um 90° gegen den Uhrzeigersinn gedreht werden
// Größe: 5 Pixel breit, 7 Pixel hoch, zusätzlich ein leerer Pixel unten
void loadCharacter(char c){
	
	switch(c){
		case ' ':
			charArray[0] = 0x00;
			charArray[1] = 0x00;
			charArray[2] = 0x00;
			charArray[3] = 0x00;
			charArray[4] = 0x00;
			break;	
		case '!':
			charArray[0] = 0x00;
			charArray[1] = 0x00;
			charArray[2] = 0x5F;
			charArray[3] = 0x00;
			charArray[4] = 0x00;
			break;		
		case '.':
			charArray[0] = 0x00;
			charArray[1] = 0x60;
			charArray[2] = 0x60;
			charArray[3] = 0x00;
			charArray[4] = 0x00;
			break;
		case '0':
			charArray[0] = 0x3E;
			charArray[1] = 0x51;
			charArray[2] = 0x49;
			charArray[3] = 0x45;
			charArray[4] = 0x3E;
			break;
		case '1':
			charArray[0] = 0x00;
			charArray[1] = 0x42;
			charArray[2] = 0x7F;
			charArray[3] = 0x40;
			charArray[4] = 0x00;
			break;
		case '2':
			charArray[0] = 0x42;
			charArray[1] = 0x61;
			charArray[2] = 0x51;
			charArray[3] = 0x49;
			charArray[4] = 0x46;
			break;
		case '3':
			charArray[0] = 0x21;
			charArray[1] = 0x41;
			charArray[2] = 0x45;
			charArray[3] = 0x4B;
			charArray[4] = 0x31;
			break;
		case '4':
			charArray[0] = 0x18;
			charArray[1] = 0x14;
			charArray[2] = 0x12;
			charArray[3] = 0x7F;
			charArray[4] = 0x10;
			break;
		case '5':
			charArray[0] = 0x27;
			charArray[1] = 0x45;
			charArray[2] = 0x45;
			charArray[3] = 0x45;
			charArray[4] = 0x39;
			break;
		case '6':
			charArray[0] = 0x3C;
			charArray[1] = 0x4A;
			charArray[2] = 0x49;
			charArray[3] = 0x49;
			charArray[4] = 0x31;
			break;
		case '7':
			charArray[0] = 0x01;
			charArray[1] = 0x71;
			charArray[2] = 0x09;
			charArray[3] = 0x05;
			charArray[4] = 0x03;
			break;
		case '8':
			charArray[0] = 0x36;
			charArray[1] = 0x49;
			charArray[2] = 0x49;
			charArray[3] = 0x49;
			charArray[4] = 0x36;
			break;
		case '9':
			charArray[0] = 0x06;
			charArray[1] = 0x49;
			charArray[2] = 0x49;
			charArray[3] = 0x29;
			charArray[4] = 0x1E;
			break;					
		case 'A':
			charArray[0] = 0x7E;
			charArray[1] = 0x11;
			charArray[2] = 0x11;
			charArray[3] = 0x11;
			charArray[4] = 0x7E;
			break;
		case 'B':
			charArray[0] = 0x7F;
			charArray[1] = 0x49;
			charArray[2] = 0x49;
			charArray[3] = 0x49;
			charArray[4] = 0x36;
			break;
		case 'C':
			charArray[0] = 0x3E;
			charArray[1] = 0x41;
			charArray[2] = 0x41;
			charArray[3] = 0x41;
			charArray[4] = 0x22;
			break;
		case 'D':
			charArray[0] = 0x7F;
			charArray[1] = 0x41;
			charArray[2] = 0x41;
			charArray[3] = 0x22;
			charArray[4] = 0x1C;
			break;
		case 'E':
			charArray[0] = 0x7F;
			charArray[1] = 0x49;
			charArray[2] = 0x49;
			charArray[3] = 0x49;
			charArray[4] = 0x41;
			break;
		case 'F':
			charArray[0] = 0x7F;
			charArray[1] = 0x09;
			charArray[2] = 0x09;
			charArray[3] = 0x09;
			charArray[4] = 0x01;
			break;
		case 'G':
			charArray[0] = 0x3E;
			charArray[1] = 0x41;
			charArray[2] = 0x49;
			charArray[3] = 0x49;
			charArray[4] = 0x3A;
			break;
		case 'H':
			charArray[0] = 0x7F;
			charArray[1] = 0x08;
			charArray[2] = 0x08;
			charArray[3] = 0x08;
			charArray[4] = 0x7F;
			break;
		case 'I':
			charArray[0] = 0x00;
			charArray[1] = 0x41;
			charArray[2] = 0x7F;
			charArray[3] = 0x41;
			charArray[4] = 0x00;
			break;
		case 'J':
			charArray[0] = 0x20;
			charArray[1] = 0x08;
			charArray[2] = 0x14;
			charArray[3] = 0x3F;
			charArray[4] = 0x01;
			break;
		case 'K':
			charArray[0] = 0x7F;
			charArray[1] = 0x08;
			charArray[2] = 0x14;
			charArray[3] = 0x22;
			charArray[4] = 0x41;
			break;
		case 'L':
			charArray[0] = 0x7F;
			charArray[1] = 0x40;
			charArray[2] = 0x40;
			charArray[3] = 0x40;
			charArray[4] = 0x40;
			break;
		case 'M':
			charArray[0] = 0x7F;
			charArray[1] = 0x02;
			charArray[2] = 0x0C;
			charArray[3] = 0x02;
			charArray[4] = 0x7F;
			break;
		case 'N':
			charArray[0] = 0x7F;
			charArray[1] = 0x04;
			charArray[2] = 0x08;
			charArray[3] = 0x10;
			charArray[4] = 0x7F;
			break;
		case 'O':
			charArray[0] = 0x3E;
			charArray[1] = 0x41;
			charArray[2] = 0x41;
			charArray[3] = 0x41;
			charArray[4] = 0x3E;
			break;
		case 'P':
			charArray[0] = 0x7F;
			charArray[1] = 0x09;
			charArray[2] = 0x09;
			charArray[3] = 0x09;
			charArray[4] = 0x06;
			break;
		case 'Q':
			charArray[0] = 0x3E;
			charArray[1] = 0x41;
			charArray[2] = 0x51;
			charArray[3] = 0x21;
			charArray[4] = 0x5E;
			break;
		case 'R':
			charArray[0] = 0x7F;
			charArray[1] = 0x09;
			charArray[2] = 0x19;
			charArray[3] = 0x29;
			charArray[4] = 0x46;
			break;
		case 'S':
			charArray[0] = 0x46;
			charArray[1] = 0x49;
			charArray[2] = 0x49;
			charArray[3] = 0x49;
			charArray[4] = 0x31;
			break;
		case 'T':
			charArray[0] = 0x01;
			charArray[1] = 0x01;
			charArray[2] = 0x7F;
			charArray[3] = 0x01;
			charArray[4] = 0x01;
			break;
		case 'U':
			charArray[0] = 0x3F;
			charArray[1] = 0x40;
			charArray[2] = 0x40;
			charArray[3] = 0x40;
			charArray[4] = 0x3F;
			break;
		case 'V':
			charArray[0] = 0x1F;
			charArray[1] = 0x20;
			charArray[2] = 0x40;
			charArray[3] = 0x20;
			charArray[4] = 0x1F;
			break;
		case 'W':
			charArray[0] = 0x3F;
			charArray[1] = 0x40;
			charArray[2] = 0x30;
			charArray[3] = 0x40;
			charArray[4] = 0x3F;
			break;
		case 'X':
			charArray[0] = 0x63;
			charArray[1] = 0x14;
			charArray[2] = 0x08;
			charArray[3] = 0x14;
			charArray[4] = 0x63;
			break;
		case 'Y':
			charArray[0] = 0x07;
			charArray[1] = 0x08;
			charArray[2] = 0x70;
			charArray[3] = 0x08;
			charArray[4] = 0x07;
			break;
		case 'Z':
			charArray[0] = 0x61;
			charArray[1] = 0x51;
			charArray[2] = 0x49;
			charArray[3] = 0x45;
			charArray[4] = 0x43;
			break;
	}		
}


