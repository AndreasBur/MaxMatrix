/*

LED cube v1.0
Characters.h

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

 
// Variablen
extern volatile uint8_t charArray[5];	// Daten des Zeichens

// Methoden
void loadCharacter(char c);				// Lädt die Daten zum ausgewählten Zeichen in das globale Feld charArray

