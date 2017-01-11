/*

LED cube v1.0
LEDcube.h

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


// Konstanten
#ifndef F_CPU
	#define F_CPU 16000000UL		// CPU-Frequenz
#endif

#ifndef F_Refresh
	#define F_Refresh 100UL			// Bildwiederholfrequenz (theoretisch 61Hz...7800Hz)
#endif


#ifndef OCR2value
	#define OCR2value ((F_CPU/(F_Refresh * 8 * 128)) - 1)		//OCR-Wert zur Bildwiederholfrequenz
#endif

#ifndef frameTimeOCR
	#define frameTimeOCR(time) ((time*F_CPU)/1024 - 1)			//OCR-Wert zur Bildrate
#endif

