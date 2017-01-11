/*

LED cube v1.0
Transformations.c

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


// Die Funktionen transformieren den aktuellen Frame (currentFrame) und speichern die Änderung im nächsten Frame (nextFrame).

// include
#include <stdint.h>


// Variablendefinition
extern volatile uint8_t *currentFrame;	// Zeiger für den aktuellen Frame
extern volatile uint8_t *nextFrame;		// Zeiger für den nächsten Frame




// Füllt den Inhalt
void fillFrame(uint8_t data){
	for (uint8_t z=0; z<=7; z++){
		for (uint8_t y=0; y<=7; y++){
			*(nextFrame+z*8+y)=data;
		}			
	}
}

// Löscht den Inhalt
void clearFrame(){
	fillFrame(0x00);
}

// Kopiert den Frame
void copyFrame(){
	for (uint8_t z=0; z<=7; z++){
		for (uint8_t y=0; y<=7; y++){
			*(nextFrame+z*8+y)=*(currentFrame+z*8+y);
		}			
	}
}


// Schiebt den Inhalt um eine Position nach vorne, Einfügen von Nullen hinten
void shiftForward(){
	for (uint8_t z=0; z<=7; z++){
		for (uint8_t y=0; y<=6; y++){
			*(nextFrame+z*8+y)=*(currentFrame+z*8+y+1);
		}
		*(nextFrame+z*8+7) = 0x00;			
	}
}

// Schiebt den Inhalt um eine Position nach hinten, Einfügen von Nullen vorne
void shiftBackward(){
	for (uint8_t z=0; z<=7; z++){
		for (uint8_t y=1; y<=8; y++){
			*(nextFrame+z*8+y)=*(currentFrame+z*8+y-1);
		}
		*(nextFrame+z*8+0) = 0x00;			
	}
}

// Schiebt den Inhalt um eine Position nach unten, Einfügen von Nullen oben
void shiftDownward(){
	for(uint8_t y=0; y<=7; y++){
		for(uint8_t z=0; z<=6; z++){
			*(nextFrame+z*8+y)=*(currentFrame+(z+1)*8+y);
		}
		*(nextFrame+7*8+y) = 0x00;
	}
}

// Schiebt den Inhalt um eine Position nach oben, Einfügen von Nullen unten
void shiftUpward(){
	for(uint8_t y=0; y<=7; y++){
		for(uint8_t z=0; z<=6; z++){
			*(nextFrame+(z+1)*8+y)=*(currentFrame+z*8+y);
		}
		*(nextFrame+0*8+y) = 0x00;
	}
}

// Schiebt den Inhalt um eine Position nach links
void shiftLeft(){
	for (uint8_t z=0; z<=7; z++){
		for (uint8_t y=0; y<=7; y++){
			*(nextFrame+z*8+y)=(*(currentFrame+z*8+y)<<1);
		}			
	}
}

// Schiebt den Inhalt um eine Position nach rechts
void shiftRight(){
	for (uint8_t z=0; z<=7; z++){
		for (uint8_t y=0; y<=7; y++){
			*(nextFrame+z*8+y)=(*(currentFrame+z*8+y)>>1);
		}			
	}
}


// Rotiert den Inhalt um eine Position im Uhrzeigersinn
void rotateClockwise(uint8_t clk){
	
	/*
	Nach folgendem Schema werden die 4 verschiedenen Kreise (Nummerierung von außen nach innen) gedreht
	clk beginnt bei 1
	
	clk 		0 0 0 0 0 0 0 | 0 0 1 1 1 1 1 | 1 1 1 1 1 2 2 | 2 2 2 2 2 2 2
				1 2 3 4 5 6 7 | 8 9 0 1 2 3 4 | 5 6 7 8 9 0 1 | 2 3 4 5 6 7 8
				--------------|---------------|---------------|--------------
	Kreis 1:	X X X X X X X | X X X X X X X | X X X X X X X | X X X X X X X		28 Takte pro Umdrehung
	Kreis 2:	X X   X   X X | X X   X   X X | X X   X   X X | X X   X   X X		20 Takte pro Umdrehung
	Kreis 3:	  X   X   X   |   X   X   X   |   X   X   X   |   X   X   X			12 Takte pro Umdrehung 
	Kreis 4:		  X       |       X       |       X       |       X				 4 Takte pro Umdrehung
	
	*/
	
	
	// Berechnung, welche Kreise gedreht werden müssen
	uint8_t rotCircle = 1;
	if(((clk-3)%7 != 0) && ((clk-5)%7 != 0)){
		rotCircle = 2;
	}		
	if((clk-4)%7 == 0){
		rotCircle = 4;
	}else{
		if(((clk-2)%7 == 0) || ((clk-6)%7 == 0)){
			rotCircle = 3;
		}
	}
	
	
	for(uint8_t z=0; z<=7; z++){
		
		// Kreis 1 von außen	
		*(nextFrame+z*8+0)=(*(currentFrame+z*8+0)<<1);						// Reihe vorne
		for(uint8_t y=0; y<=6; y++){
			*(nextFrame+z*8+(y+1))  = (*(currentFrame+z*8+ y   ) & (1<<7));	// Reihe links
			*(nextFrame+z*8+y)     |= (*(currentFrame+z*8+(y+1)) & (1<<0));	// Reihe rechts
		} 
		*(nextFrame+z*8+7)|=((*(currentFrame+z*8+7)>>1) & 0b01111111);		// Reihe hinten
		

		switch(rotCircle){
			case 4:
				// Kreis 4 von außen
				*(nextFrame+z*8+3) |= ((*(currentFrame+z*8+3)<<1) & 0b00010000);	// Reihe vorne
				*(nextFrame+z*8+4) |= ((*(currentFrame+z*8+4)>>1) & 0b00001000);	// Reihe hinten
				*(nextFrame+z*8+3) |= (*(currentFrame+z*8+4) & (1<<3));				// Reihe rechts
				*(nextFrame+z*8+4) |= (*(currentFrame+z*8+3) & (1<<4));				// Reihe links 		
				
			case 3:
				// Kreis 3 von außen
				*(nextFrame+z*8+2) |= ((*(currentFrame+z*8+2)<<1) & 0b00111000);	// Reihe vorne
				*(nextFrame+z*8+5) |= ((*(currentFrame+z*8+5)>>1) & 0b00011100);	// Reihe hinten
				for(uint8_t y=2; y<=4; y++){	
					*(nextFrame+z*8+y)     |= (*(currentFrame+z*8+(y+1)) & (1<<2));	// Reihe rechts
					*(nextFrame+z*8+(y+1)) |= (*(currentFrame+z*8+ y   ) & (1<<5));	// Reihe links
	
				}
			
			case 2:
				// Kreis 2 von außen
				*(nextFrame+z*8+1) |= ((*(currentFrame+z*8+1)<<1) & 0b01111100);	// Reihe vorne
				*(nextFrame+z*8+6) |= ((*(currentFrame+z*8+6)>>1) & 0b00111110);	// Reihe hinten
				for(uint8_t y=1; y<=5; y++){	
					*(nextFrame+z*8+y)     |= (*(currentFrame+z*8+(y+1)) & (1<<1));	// Reihe rechts
					*(nextFrame+z*8+(y+1)) |= (*(currentFrame+z*8+ y   ) & (1<<6));	// Reihe links
				}
		}			
	}	
}