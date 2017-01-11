/*

LED cube v1.0
Transformations.h

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



// Methoden
void fillFrame(uint8_t data);		// Füllt den Frame mit den übergebenen x-Daten
void clearFrame();					// Löscht den Frame
void copyFrame();					// Kopiert den vorherigen Frame
void shiftForward();				// Schiebt den Inhalt des Frames um eine Position nach vorne
void BackForward();					// Schiebt den Inhalt des Frames um eine Position nach hinten
void shiftDownward();				// Schiebt den Inhalt des Frames um eine Position nach unten
void shiftUpward();					// Schiebt den Inhalt des Frames um eine Position nach oben
void shiftLeft();					// Schiebt den Inhalt des Frames um eine Position nach links
void shiftRight();					// Schiebt den Inhalt des Frames um eine Position nach rechts
void rotateClockwise(uint8_t clk);  // Rotiert den Inhalt des Frames um eine Position im Uhrzeigersinn
										// clk: Gibt den Takt an um die inneren Positionen zum richtigen Zeitpunkt mitzudrehen
