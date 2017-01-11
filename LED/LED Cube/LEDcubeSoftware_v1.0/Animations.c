/*

LED cube v1.0
Animations.c

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

// Notation:
// x-Koordinate: Reihe,  x=0 rechts
// y-Koordinate: Spalte, y=0 vorne
// z-Koordinate: Ebene,  z=0 unten

// Hinweis:
// Jeder neue Frame muss (nach einer waitForNextFrame()-Anweisung) mit einer Transformation oder mit einem kompletten Neuaufbau des Frames beginnen


// include
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include "Transformations.h"
#include "Characters.h"
#include "LEDcube.h"


extern void waitForNextFrame(void);




// Variablendefinition
extern volatile uint8_t *currentFrame;	// Zeiger für den aktuellen Frame
extern volatile uint8_t *nextFrame;		// Zeiger für den nächsten Frame

volatile uint8_t dimTable[32] = {0, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 8, 10, 11,		// PWM Werte zum linearen Dimmen der Beleuchtungsstärke
									 13, 16, 19, 23, 27, 32, 38, 45, 54, 64, 76,
									 91, 108, 128, 152, 181, 215, 255};


// xz-Ebene, die sich vorwärts/rückwärts bewegt
void layerMoveXZ(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.08);
	
	// Startposition hinten y=7
	clearFrame();
	for(uint8_t z=0; z<=7; z++){
		*(nextFrame+z*8+7)=0xFF;
	}
	waitForNextFrame();
	
	for(; frameCt>=1; frameCt--){
		// Bewegung nach vorne
		for(uint8_t ct=0; ct<=6; ct++){
			shiftForward();
			waitForNextFrame();
		}
		// Bewegung nach hinten
		for(uint8_t ct=0; ct<=6; ct++){
			shiftBackward();
			waitForNextFrame();
		}
		
	}
}


// xy-Ebene, die sich aufwärts/abwärts bewegt
void layerMoveXY(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.08);
	
	// Startposition unten z=0
	clearFrame();
	for(uint8_t y=0; y<=7; y++){
		*(nextFrame+0*8+y)=0xFF;
	}
	waitForNextFrame();
	
	for(; frameCt>=1; frameCt--){
		// Bewegung nach oben
		for(uint8_t ct=0; ct<=6; ct++){
			shiftUpward();
			waitForNextFrame();
		}
		// Bewegung nach hinten
		for(uint8_t ct=0; ct<=6; ct++){
			shiftDownward();
			waitForNextFrame();
		}
		
	}
}


// yz-Ebene, die sich rechts/links bewegt
void layerMoveYZ(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.08);
	
	// Startposition unten x=0
	clearFrame();
	for(uint8_t z=0; z<=7; z++){
		for(uint8_t y=0; y<=7; y++){
			*(nextFrame+z*8+y)=(1<<7);
		}
	}
	waitForNextFrame();
	
	for(; frameCt>=1; frameCt--){
		// Bewegung nach rechts
		for(uint8_t ct=0; ct<=6; ct++){
			shiftRight();
			waitForNextFrame();
		}
		// Bewegung nach links
		for(uint8_t ct=0; ct<=6; ct++){
			shiftLeft();
			waitForNextFrame();
		}
		
	}
}


// Quader, der von der Mitte aus zunächst anwächst und dann von der Mitte aus wieder aufgelöst wird
void growingBox(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.09);
	
	for(; frameCt>=1; frameCt--){
	
		clearFrame();
	
		uint8_t x[4] = {0b00011000, 0b00111100, 0b01111110, 0b11111111};
	
		for (uint8_t ct=0; ct<=1; ct++){
			for(uint8_t frameCt=4; frameCt<=7; frameCt++){
				for(uint8_t z=(7-frameCt); z<=frameCt; z++){
					for(uint8_t y=(7-frameCt); y <=frameCt; y++){
						*(nextFrame+z*8+y)=x[frameCt-4];
					}
				}
				waitForNextFrame();
				copyFrame();
			}
			x[0] = 0b11100111;
			x[1] = 0b11000011;
			x[2] = 0b10000001;
			x[3] = 0b00000000;
		}
	
	}	
}	


// Würfel wird diagonal von der vorderen linken Ecke aus gefüllt und anschließend wieder geleert
void diagonalFill(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.06);
	
	clearFrame();
	waitForNextFrame();
	
	for(; frameCt>=1; frameCt--){
	
		for(uint8_t mode=0; mode <=1; mode++){
			// mode=0: Würfel wird gefüllt
			// mode=1: Würfel wird geleert	
		
			// Erstes Drittel des Würfelvolumens
			for(uint8_t ct=0; ct<=7; ct++){
				shiftUpward();
				uint8_t data = 0xFF;
				for(uint8_t yct=0; yct<=ct; yct++){
					data = data>>1;
					*(nextFrame+0+ct-yct)= mode ? data : ~data;
				}
				for(uint8_t y=ct+1; y<=7; y++){
					*(nextFrame+0+y)= mode ? 0xFF : 0x00;
				}
				waitForNextFrame();
			}

			// Zweites Drittel des Würfelvolumens
			for(uint8_t ct=1; ct<=7; ct++){
				shiftUpward();
				uint8_t data = 0xFF;
				for(uint8_t y=0; y<=ct; y++){
					*(nextFrame+0+y)= mode ? 0x00 : 0xFF;
				}
				for(uint8_t y=ct+1; y<=7; y++){
					data = data<<1;
					*(nextFrame+0+y)=mode ? ~data : data;
				}
				waitForNextFrame();
			}
	
			// Letztes Drittel des Würfelvolumens
			for(uint8_t ct=0; ct<=6; ct++){
				shiftUpward();
				if (!mode) {
					for(uint8_t y=0; y<=7; y++){
						*(nextFrame+0+y)=0xFF;
					}
				}
				waitForNextFrame();
			}	
		}

	}	
	
}


// Voller Cube, der auf- und abgedimmt wird
void dimmingCube(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.08);
	OCR0 = 0;
	
	fillFrame(0xFF);
	waitForNextFrame();
	fillFrame(0xFF);
	waitForNextFrame();
	
	for(; frameCt>=1; frameCt--){
		
		//Fadein
		for(uint8_t ct=0; ct<32; ct++){
				OCR0 = dimTable[ct];
				waitForNextFrame();	
		}
		waitForNextFrame();
		waitForNextFrame();
		// Fadeout
		for(uint8_t ct=31; ct<32; ct--){
				OCR0 = dimTable[ct];
				waitForNextFrame();
		}
	}
	clearFrame();
	waitForNextFrame();
	OCR0 = 255;
	
}


// Zeichnet die Kanten eines Quaders
void drawCuboid(uint8_t xstart, uint8_t ystart, uint8_t zstart, uint8_t dx, uint8_t dy, uint8_t dz){

	uint8_t edge1 = 0;  //x-Daten für die x-Kante
	uint8_t edge2 = 1;  //x-Daten für die y und z-Kante
	for(uint8_t ct=dx; ct>=1; ct--){
		edge1 <<= 1;
		edge1 |= 1;
		edge2 <<= 1;
	}
	edge2 >>= 1;
	edge2 |= 1;
	
	edge1 <<= xstart;
	edge2 <<= xstart;
	if(dx==8){
		edge2 |= (1<<7);
	}
	
	// edge1
	*(nextFrame+ zstart*8      + ystart)       |= edge1;
	*(nextFrame+(zstart+dz-1)*8+ ystart)       |= edge1;
	*(nextFrame+ zstart*8      +(ystart+dy-1)) |= edge1;
	*(nextFrame+(zstart+dz-1)*8+(ystart+dy-1)) |= edge1;
	
	// edge2
	for(int8_t z=zstart+1; z<=zstart+dz-2; z++){
		*(nextFrame+z*8+ystart)      |= edge2;
		*(nextFrame+z*8+ystart+dy-1) |= edge2;
	}
	for(int8_t y=ystart+1; y<=ystart+dy-2; y++){
		*(nextFrame+zstart*8+y)        |= edge2;
		*(nextFrame+(zstart+dz-1)*8+y) |= edge2;
	}

}


// Box verändert seine Größe und bewegt sich zuällig in die Ecken des Würfels
void bouncingBox(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.07);
	
	// Anmination Start
	clearFrame();
	for(uint8_t ct=1; ct<=8; ct++){	
		clearFrame();
		drawCuboid(0,0,0,ct,ct,ct);
		waitForNextFrame();
	}
	
	// Animation Main
	for(; frameCt>=1; frameCt--){
		
		uint8_t xrand = rand() % 2;
		uint8_t yrand = rand() % 2;
		uint8_t zrand = rand() % 2;
	
		// verkleinern
		for(uint8_t ct=8; ct>=1; ct--){	
			uint8_t xstart = xrand ? 0 : 8-ct;
			uint8_t ystart = yrand ? 0 : 8-ct;
			uint8_t zstart = zrand ? 0 : 8-ct;
			clearFrame();
			drawCuboid(xstart,ystart,zstart,ct,ct,ct);
			waitForNextFrame();
		}
	
		// vergrößern
		for(uint8_t ct=1; ct<=8; ct++){	
			uint8_t xstart = xrand ? 0 : 8-ct;
			uint8_t ystart = yrand ? 0 : 8-ct;
			uint8_t zstart = zrand ? 0 : 8-ct;
			clearFrame();
			drawCuboid(xstart,ystart,zstart,ct,ct,ct);
			waitForNextFrame();
		}
	}
}


// 2 Ringe, die zyklisch sich gegenseitig durchwandern
void ring(uint16_t frameCt){

	OCR1A = frameTimeOCR(0.12);
	
  //uint8_t loopCount[3] = {2, 5, 9};	// Aktuelle Position der drei Ringe
	uint8_t loopCount[2] = {2, 7};		// Aktuelle Position der zwei Ringe	

	for(; frameCt>=1; frameCt--){
		
		// Ringe anzeigen
		clearFrame();
		for(uint8_t loopN=0; loopN<=1; loopN++){
			
			uint8_t xstart;								// x-startindex für das Rechteck
			uint8_t yzstart;							// x- und z-startindex für das Rechteck
			uint8_t delta;								// Höhe/Tiefe des Rechtecks
		
			if(loopCount[loopN]<=5){
				xstart = loopCount[loopN]+1;
				if ((loopCount[loopN]==2)||(loopCount[loopN]==3)){
					yzstart = 3;
					delta = 2;
				}else{
					yzstart = 2;
					delta = 4;				
				}
			}else{
				xstart = 11 - loopCount[loopN];
				yzstart = 1;
				delta = 6;
			}
			
			// Rechteck zeichnen
			drawCuboid(xstart, yzstart, yzstart, 1, delta, delta);		
		
			// Neue Position berechnen
			loopCount[loopN]++;
			if(loopCount[loopN]>=10){
				loopCount[loopN]=0;
			}
		}
		waitForNextFrame();
	}	
}


// Regenähnliche Animation
// Auf der obersten Ebene werden zufällige LEDs aktiviert, die sich dann nach unten bewegen	
void rainfall(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.1);
	
	// Animation-Start
	clearFrame();
	waitForNextFrame();
	
	// Animation-Main
	for(; frameCt>=1; frameCt--){
		shiftDownward();
	
		for(uint8_t ct = rand() % 5; ct>=1; ct--){
			uint8_t xpos = rand() % 8;
			uint8_t ypos = rand() % 8;
			*(nextFrame+7*8+ypos) |= (1<<xpos);
		}
		waitForNextFrame();	 		
	}

	// Animation-End
	for(uint8_t ct=0; ct<=7; ct++){
		shiftDownward(0);
		waitForNextFrame();
	}
	
	
}


// Punktweise up-down-Animation
// Auf der obersten Ebene werden zufällige LEDs aktiviert, die deakivierten LEDs leuchten auf der untersten Ebene
// Zufällig wird eine LED ausgewählt, die sich dann von oben nach unten, bzw. von unten nach oben bewegt
void LEDUpDown(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.06);
	
	clearFrame();
	for(uint8_t y=0; y<=7; y++){
		uint8_t randN = rand() % 256;
		*(nextFrame+7*8+y) =  randN;
		*(nextFrame+0*8+y) = ~randN;
	}
	waitForNextFrame();
	
	// Animation-Main
	for(; frameCt>=1; frameCt--){
		uint8_t xpos = rand() % 8;
		uint8_t ypos = rand() % 8;

	
		uint8_t xmask = (1<<xpos);
		if(*(currentFrame+0*8+ypos) & xmask){
			// nach oben
			for(uint8_t z=0; z<=6; z++){
				copyFrame();
				*(nextFrame+z*8+ypos)     &= ~xmask;
				*(nextFrame+(z+1)*8+ypos) |=  xmask;
				waitForNextFrame();
			}
		}else{
			// nach unten
			for(uint8_t z=7; z>=1; z--){
				copyFrame();
				*(nextFrame+z*8+ypos)     &= ~xmask;
				*(nextFrame+(z-1)*8+ypos) |=  xmask;
				waitForNextFrame();
			}
		}
		copyFrame();
		waitForNextFrame();
	}
	
	// Animation-End
	for(uint8_t ct=7; ct>=1; ct--){
		shiftDownward();
		for(uint8_t y=0; y<=7; y++){
			*(nextFrame+0*8+y) |= *(currentFrame+0*8+y);
		}
		waitForNextFrame();
	}
	clearFrame();
	waitForNextFrame();
}	


// Ebene bewegt sich in z-Richtung und verteilt dabei LEDs über das Würfelvolumen
void shiftArrangeUpDown(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.07);
	
	uint8_t randData[8][8];			// speichert das Zufallsbild
	
	// leeren Cube anzeigen
	clearFrame();
	waitForNextFrame();
	
	// untere Ebene füllen
	clearFrame();
	for(uint8_t y=0; y<=7; y++){
		*(nextFrame+0*8+y) = 0xFF;
	}
	waitForNextFrame();
	
	// Animation-Main
	for(; frameCt>=1; frameCt--){
		
		// ctUpDown=0: Bewegung nach oben, ctUpDow =1: Bewegung nach oben
		for(uint8_t ctUpDown=0; ctUpDown<=1; ctUpDown++){
			
			// Altes Zufallsbild löschen
			for(uint8_t y=0; y<=7; y++){
				for(uint8_t z=0; z<=7; z++){
					randData[y][z] = 0x00;
				}
			}
		
			// Zufallsbild neu berechnen
			for(uint8_t x=0; x<=7; x++){
				for(uint8_t y=0; y<=7; y++){
					uint8_t z = rand() % 8;
					randData[y][z] |= (1<<x);
				}
			}
			
			// ct=0: Verteilung der LEDs, ct=1: LEDs wieder zusammen
			for(uint8_t ct=0; ct<=1; ct++){
				copyFrame();
				waitForNextFrame();
				
				// Schleife für das Durchlaufen längs der z-Achse
				for(uint8_t ctz=0; ctz<=6; ctz++){
					uint8_t z1;
					uint8_t z2;
			
					if(ctUpDown==0){
						z1 = ctz + 1;
						z2 = ctz;
					}else{
						z1 = 6-ctz;
						z2 = 7-ctz;
					}
			
					copyFrame();
					for(uint8_t y=0; y<=7; y++){
						if(ct==0){
							*(nextFrame+z1*8+y) = *(nextFrame+z2*8+y) & ~randData[y][z2];
							*(nextFrame+(z2)*8+y) = randData[y][z2];
						}else{
							*(nextFrame+z1*8+y) |= *(nextFrame+z2*8+y);
							*(nextFrame+z2*8+y) = 0x00;
						}	
					}
					waitForNextFrame();
				}
			}
		}
	}		
	
}
			

// Laser
// "Laserpulse" wandern von unten nach oben, der Laser bewegt sich zufällig auf der untersten Ebene
void laser(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.07);
	
	// Animation-Start
	clearFrame();
	int8_t xpos = 4;
	int8_t ypos = 4;
	
	// Animation-Main
	for(; frameCt>=1; frameCt--){
		shiftUpward();
		int8_t movx = rand() % 3 - 1;
		int8_t movy = rand() % 3 - 1;
	
		xpos += movx;
		ypos += movy;
	
		if(xpos < 0){
			xpos = 1;
		}
		if(xpos>7){
			xpos = 6;
		}
		if(ypos < 0){
			ypos = 1;
		}
		if(ypos>7){
			ypos = 6;
		}
	
		*(nextFrame+0*8+ypos) = (1<<xpos);
		waitForNextFrame();	
	}
	
	// Animation-End
	for(uint8_t ct=0; ct <=7; ct++){
		shiftUpward();
		waitForNextFrame();
	}
}

	
// Snakeähnliche Animation, bei der sich eine Aneinanderreihung von 8 LEDs zufällig durch den Würfel bewegt
void snake(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.08);
	
	clearFrame();
	waitForNextFrame();
	
	uint8_t snakeBufferXY[8] = {};		// x und y Position (yyyyxxxx) der kompletten Schlange
	uint8_t snakeBufferZ[8] = {};		// z Position der kompletten Schlange
		
	uint8_t dir;						// Richtung (00zzyyxx) mit
											// 01 keine Änderung
											// 10 +1
											// 00 -1
	
	uint8_t indexTail = 1;				// index, der auf den Schwanz zeigt
	uint8_t indexHead = 0;				// index, der auf den Kopf zeigt
	
	snakeBufferXY[0] = 0b00110011;		// Anfangsposition (x=y=z=3)
	snakeBufferZ[0] = 0b00000011;
	dir = 0b00010110;					// Anfangsrichtung (x: +1)
	
	uint8_t xpos;						// aktuelle x-Position
	uint8_t ypos;						// aktuelle y-Position
	uint8_t zpos;						// aktuelle z-Position
	
	// Schlange baut sich auf und schlängelt
	for(; frameCt>=1; frameCt--){
	
		copyFrame();
		
		// Letztes Schwanzsegment löschen
		xpos =  snakeBufferXY[indexTail] & 0b00001111;
		ypos = (snakeBufferXY[indexTail] & 0b11110000) >> 4;
		zpos =  snakeBufferZ[indexTail]  & 0b00001111;
		*(nextFrame+zpos*8+ypos) &= ~(1<<xpos);
	
	
		// Neue Kopfposition berechnen
		uint8_t dirNew;			// Neue Richtung
		uint8_t loopCt = 0;		// Schutz vor Endlosschleife
		uint8_t err = 1;		// Error-Flag: =0: neue Position ok; =1: neue Position nicht ok, Position wird neu berechnet
	
	
		while(err==1){

			xpos =  snakeBufferXY[indexHead] & 0b00001111;
			ypos = (snakeBufferXY[indexHead] & 0b11110000) >> 4;
			zpos =  snakeBufferZ[indexHead]  & 0b00001111;
			
			loopCt++;
			if(loopCt==64){
				return;
			}
			
			uint8_t mov = rand() % 5;	// Richtung unverändert (=0,1,2), Richtung ändert sich (=3,4)
		
			if(mov<=2){	
				// Richtung bleibt bestehen
				xpos = xpos + ((dir & 0b00000011) >> 0) - 1;
				ypos = ypos + ((dir & 0b00001100) >> 2) - 1;
				zpos = zpos + ((dir & 0b00110000) >> 4) - 1;
				dirNew = dir;
			}else{
				// Richtung ändert sich
			
				uint8_t mov2 = rand() % 3;				// Welche Koordinaten-Richtung? (0=x, 1=y, 2=z)
				uint8_t mov3 = (rand() % 2) << 1;		// In positiver oder negativer Richtung? (0=-1, 2=+1)
			
				switch(mov2){
					case 0:
						// x prüfen, ob neue Richtung
						if(dir & 0b00000001){
							dirNew = (mov3 << 0) | 0b00010100;
							xpos = xpos + mov3 - 1;
							break;
						}
					case 1:
						// y prüfen, ob neue Richtung
						if(dir & 0b00000100){
							dirNew = (mov3 << 2) | 0b00010001;
							ypos = ypos + mov3 - 1;
							break;
						}
					case 2:
						// z prüfen, ob neue Richtung
						if(dir & 0b00010000){
							dirNew = (mov3 << 4) | 0b00000101;
							zpos = zpos + mov3 - 1;
							break;
						}
				}
			}
		
			// Neue Position überprüfen
			// Position außerhalb des Würfels?
			if((xpos<=7)&&(ypos<=7)&&(zpos<=7)){				
				// Position bereits belegt?
				if(!(*(nextFrame+zpos*8+ypos) & (1<<xpos))){
					err = 0;
				}								
			}
		}


		// Neue Position und Richtung abspeichern
		dir = dirNew;
		snakeBufferXY[indexTail] = ((ypos<<4) | xpos);
		snakeBufferZ[indexTail] = zpos;
	
		// Head- und Tailindex neu berechnen
		indexHead = indexTail;
		indexTail = indexTail + 1;
		if(indexTail>=8){
			indexTail = 0;
		}
	
		// Neuen Kopf anzeigen
		*(nextFrame+zpos*8+ypos) |= (1<<xpos);
		waitForNextFrame();
	
	}	
			
}

	
// Sinusförmige Welle in der x-z Ebene
void wave2D(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.09);
	
	for(; frameCt>=1; frameCt--){
	
		for(uint8_t ct=0; ct<=7; ct++){
			shiftLeft();
			for(uint8_t y=0; y<=7; y++){
				*(nextFrame+ct*8+y) |= 0b00000001;
			}
			waitForNextFrame();
		}
	
		for(uint8_t ct=0; ct<=7; ct++){
			shiftLeft();
			for(uint8_t y=0; y<=7; y++){
				*(nextFrame+(7-ct)*8+y) |= 0b00000001;
			}
			waitForNextFrame();
		}
	
	}
}


// 3D-Sinusfunktion sin(t + sqrt(x^2+y^2))
void wave3D(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.09);
	
	uint8_t waveData[8][8] = { {0b00110001, 0b01010100, 0b01010011, 0b01110110, 0b01100100, 0b01110111, 0b01110101, 0b01010111},
							   {0b00010000, 0b00110011, 0b00110001, 0b01100101, 0b01010011, 0b01110111, 0b01100011, 0b01100111},	
							   {0b00010000, 0b00100001, 0b00100001, 0b01010100, 0b01000001, 0b01110110, 0b01010010, 0b01110111},
							   {0b00000000, 0b00010001, 0b00010000, 0b00110010, 0b00100001, 0b01100101, 0b00110001, 0b01110110},
							   {0b00000001, 0b00000000, 0b00000000, 0b00100001, 0b00010000, 0b01010011, 0b00100000, 0b01100101},	
							   {0b00010010, 0b00000000, 0b00000001, 0b00010000, 0b00000000, 0b00110010, 0b00010000, 0b01100011},
							   {0b00100011, 0b00000001, 0b00000010, 0b00000000, 0b00000001, 0b00100001, 0b00000000, 0b01000010},
							   {0b00110101, 0b00010010, 0b00010011, 0b00000000, 0b00000010, 0b00010000, 0b00000001, 0b00110001} };								
	
	uint8_t z;			// z-Ebene, in der die LED aktiviert wird
	uint8_t index;		// index von waveData, wo die z-Daten gespeichert sind
	
	// Wiederholung einer Periode
	for(; frameCt>=1; frameCt--){				
		
		// 1. oder 2 Halbschwingung
		for(uint8_t ctp=0; ctp<=1; ctp++){		
		
			// frameCounter einer Halbschwingung
			for(uint8_t ct=0; ct<=7; ct++){
				clearFrame();

				for(uint8_t y=0; y<=3; y++){
					for(uint8_t xct=0; xct<=1; xct++){
				
						index = ((y<<1) + xct);
				
						z = waveData[ct][index] & 0b00001111;				
						if(ctp==1){
							z = 7-z;
						}
				
						*(nextFrame+z*8+y) |= (xct==0 ? (1<<0) : (1<<2));
						*(nextFrame+z*8+y) |= (xct==0 ? (1<<7) : (1<<5));
						*(nextFrame+z*8+(7-y)) |= (xct==0 ? (1<<0) : (1<<2));
						*(nextFrame+z*8+(7-y)) |= (xct==0 ? (1<<7) : (1<<5));
						
						z = waveData[ct][index] & 0b11110000;
						z = z>>4;			
						if(ctp==1){
							z = 7-z;
						}
						
						*(nextFrame+z*8+y) |= (xct==0 ? (1<<1) : (1<<3));
						*(nextFrame+z*8+y) |= (xct==0 ? (1<<6) : (1<<4));
						*(nextFrame+z*8+(7-y)) |= (xct==0 ? (1<<1) : (1<<3));
						*(nextFrame+z*8+(7-y)) |= (xct==0 ? (1<<6) : (1<<4));
					}
				}
				waitForNextFrame();
			}
		}
	}
}	


// Pfeil, der an der Außenseite im Kreis läuft
void arrowBelt(uint8_t frameCt){
	
	OCR1A = frameTimeOCR(0.1);
	
	uint8_t arrow[] = {0b00001100, 0b00001110, 0b01111111};
	
	// Animation-Start
	clearFrame();

	for(uint8_t z=0; z<=2; z++){
		*(nextFrame+(z+1)*8+7) = arrow[z];
		*(nextFrame+(6-z)*8+7) = arrow[z];
	}
	waitForNextFrame();
	
	// Animation-Main
	for(; frameCt>=1; frameCt--){
		for(uint8_t ct=0; ct <= 31; ct++){
			rotateClockwise(1);
			waitForNextFrame();
		}
	}
	
	// Animation-End
	for(uint8_t ct=0; ct <= 7; ct++){
		rotateClockwise(1);
		for(uint8_t z=0; z<=7; z++){
			*(nextFrame+z*8+6) = 0x00;
		}
		waitForNextFrame();
	}
}


// 2 Pixel breites Band bewegt sich um den Cube
void belt(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.07);
	
	clearFrame();
	waitForNextFrame();
	
	uint8_t zpos = 3;

	for (; frameCt>=1; frameCt--){
		
		rotateClockwise(1);
	
		// Neue Segmentposition berechnen
		uint8_t move = rand() % 4;
			// move=0,1 Höhe +0
			// move=2   Höhe -1
			// move=3   höhe +1
		
		
		if (move==2){
			if (zpos>1){
				zpos--;
			}else{
				zpos=2;
			}
			
		}
		if (move==3){
			if (zpos<7){
				zpos++;
			}else{
				zpos=6;
			}
		}
		
		
		// Neues Segment anzeigen
		for(uint8_t z=0; z<=7; z++){
			*(nextFrame+z*8+7) &= (1<<7);
			if ((z==zpos)|(z==zpos-1)){
				*(nextFrame+z*8+7) |= (1<<0);
			}else{
				*(nextFrame+z*8+7) &= ~(1<<0);
			}
		}
		waitForNextFrame();
	
	}
		
	// Band auslaufen lassen
	for(uint8_t ct=0; ct<=19; ct++){
		rotateClockwise(1);
		for(uint8_t z=0; z<=7; z++){
			*(nextFrame+z*8+7) &= 0b10000000;
		}
		waitForNextFrame();
	}	

}


// Schnelle, zufällige Verteilung von ca. 15-30 LEDs
void random1(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.05);
	
	for(; frameCt>=1; frameCt--){
	
		clearFrame();
	
		for(uint8_t number = (rand() % 32) + 25; number >= 1; number--){
			uint8_t xpos = rand() % 8;
			uint8_t ypos = rand() % 8;
			uint8_t zpos = rand() % 8;
			*(nextFrame+zpos*8+ypos) |= (1<<xpos);
		}
		
		waitForNextFrame();
	}
	
}


// Zufälliges füllen und leeren des Cubes
void randomFill(uint16_t frameCt){
	
	OCR1A = frameTimeOCR(0.003);
	
	clearFrame();
	waitForNextFrame();
	
	for(; frameCt>=1; frameCt--){
	
		for(uint8_t mode=0; mode <=1; mode++){
			// mode=0: Cube füllen
			// mode=1: Cube leeren
			
			for(uint16_t ct=0; ct < 1024; ct++){
				copyFrame();
				for(uint8_t n=0; n<=2; n++){
					uint8_t xpos = rand() % 8;
					uint8_t ypos = rand() % 8;
					uint8_t zpos = rand() % 8;
					if (mode) {
						*(nextFrame+zpos*8+ypos) &= ~(1<<xpos);
					}else{
						*(nextFrame+zpos*8+ypos) |= (1<<xpos);
					}						
						
				}
				waitForNextFrame();
			}
		}
		
		
	}
}


// Text, der von der rechten Seite beginnend, sich über die Frontseite zur linken Seite bewegt
void textBelt(char *string){
	
	OCR1A = frameTimeOCR(0.1);
	
	// Textband anzeigen
	clearFrame();
	waitForNextFrame();
	while(*string){
		
		loadCharacter(*string);
		
		// Nächstes Zeichen schrittweise anzeigen
		for(uint8_t ct=0; ct<=4; ct++){
			
			rotateClockwise(1);
			
			uint8_t cdata = charArray[ct];
			for(uint8_t z=0; z<=7; z++){
				*(nextFrame+z*8+7) &= 0b10000000;
				if(cdata & 0b10000000){
					*(nextFrame+z*8+7) |= 0b00000001;
				}
				cdata <<= 1;
			}
			
			waitForNextFrame();
		}
		
		// Zwei Leerstellen
		for(uint8_t ct=0; ct<=1; ct++){
			rotateClockwise(1);
			for(uint8_t z=0; z<=7; z++){
				*(nextFrame+z*8+7) &= 0b10000000;
			}
			waitForNextFrame();
		}
		
		// Nächstes Zeichen
		string++;
	}
	
	// Text auslaufen lassen
	for(uint8_t ct=0; ct<=19; ct++){
		rotateClockwise(1);
		for(uint8_t z=0; z<=7; z++){
			*(nextFrame+z*8+7) &= 0b10000000;
		}
		waitForNextFrame();
	}
}


// Text, der sich zeichenweise von hinten nach vorne bewegt
void textToFront(char *string){
	
	OCR1A = frameTimeOCR(0.1);
	
	clearFrame();
	waitForNextFrame();
	while(*string){
		
		loadCharacter(*string);
		
		clearFrame();
		// Zeichen hinten aufbauen
		for(uint8_t ct=0; ct<=4; ct++){
			uint8_t cdata = charArray[ct];
			for(uint8_t z=7; z>=1; z--){
				*(nextFrame+z*8+7) |= (cdata & 0b00000001);
				*(nextFrame+z*8+7) <<= 1;
				cdata >>= 1;
			}	
		}
		waitForNextFrame();
		
		// Zeichen nach vorne bewegen
		for(uint8_t ct=0; ct<=6; ct++){
			shiftForward();
			waitForNextFrame();
		}
		
		// Nächstes Zeichen
		string++;
		
	}
}


