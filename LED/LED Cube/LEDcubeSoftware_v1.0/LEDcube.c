/*

LED cube v1.0
LEDcube.c

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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "Animations.h"
#include "LEDcube.h"


// Konstanten sind in LEDcube.h definiert


// Portbelegung
#define driverDataPort PORTC	// LED-Treiber: Daten
#define driverControlPort PORTB // LED-Treiber: Control
#define driverCLK PB0			// LED-Treiber: Clock
#define driverLE PB1			// LED-Treiber: Load Enable
#define driverOE PB3			// LED-Treiber: Output Enable

#define layerPort PORTA			// MOSFET-Treiber: Auswahl der Ebene


// Variablendefinition
volatile uint8_t Mem1 [8][8];					// Speicher reservieren für die Frames
volatile uint8_t Mem2 [8][8];					// Speicher reservieren für die Frames
volatile uint8_t *currentFrame = &Mem1[0][0];	// Zeiger für den aktuellen Frame
volatile uint8_t *nextFrame    = &Mem2[0][0];	// Zeiger für den nächsten Frame
volatile uint8_t frameReady = 0;				// Gibt an, dass der nächste Frame angezeigt werden soll
													// frameReady wird von der ISR Routine auf 0 gesetzt, wenn der neue Frame angezeigt werden soll



int main(void)
{
    // Initialisierung
	
	// IO-Ports
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRB = ((1<<driverCLK) | (1<<driverLE) | (1<<driverOE));
	
	// Timer2 für Bildwiederholfrequenz
	OCR2 = OCR2value;
	TCCR2 = ((1<<WGM21) | (1<<CS22) | (1<<CS20));			// CTC Mode, Prescaler 128
	
	// Timer1 für Bildrate
	OCR1A = frameTimeOCR(0.1);
	TCCR1B = ((1<<WGM12) | (1<<CS12) | (1<<CS10));			// CTC Mode, Prescaler 1024
	
	// Timer0 für PWM
	OCR0 = 255;
	TCCR0 = ((1<<WGM00) | (1<<WGM01) | (1<<COM00) | (1<<COM01) | (1<<CS01));	//Fast PWM Mode, Prescaler 8
	
	// Timer1/2 Interrupts
	TIMSK = (1<<OCIE2) | (1<<OCIE1A);						// Timer1/2 Compare Match Interrupt aktivieren
	
	
	sei();
	
	while(1)
    {
		dimmingCube(4);
		layerMoveXY(2);
		layerMoveXZ(2);
		layerMoveYZ(2);
		diagonalFill(2);
		belt(150);
		growingBox(10);
		bouncingBox(7);
		rainfall(100);
		LEDUpDown(30);
		randomFill(3);
		laser(100);
		wave2D(10);
		wave3D(10);
		ring(125);
		snake(256);
		shiftArrangeUpDown(4);		
		arrowBelt(1);
		random1(100);
		textBelt("LEDCUBE");
		textToFront("LEDCUBE");
    }
}


// Zeigt die nächste Ebene des aktuellen Frames an
ISR(TIMER2_COMP_vect){
	
	// Variablen aus dem Speicher lesen
	static uint8_t currentLayerSignalSave = 0;					// Signal für die Ansteuerung der MOSFETs (1,2,4,...,128)
	static uint8_t currentLayerNumberSave = 7;					// Nummer der Ebene (0,1,2,...,7)
	uint8_t currentLayerSignal = currentLayerSignalSave;
	uint8_t currentLayerNumber = currentLayerNumberSave;
	
	// Ebene neu berechnen
	currentLayerNumber++;
	currentLayerSignal <<= 1;
	
	if (currentLayerSignal == 0){
		currentLayerSignal = 1;
		currentLayerNumber = 0;
	}
		
		
	// Daten in das Register des LED-Treibers schieben
	for (uint8_t ct = 0; ct<=7; ct++){
		driverDataPort = *(currentFrame+currentLayerNumber*8+(7-ct));
		asm volatile("nop");
		driverControlPort |= (1<<driverCLK);
		asm volatile("nop");
		driverControlPort &= ~(1<<driverCLK);
	}
	
	// Übernahme der Daten in das Ausgaberegister
	driverControlPort  |= (1<<driverLE);				
	asm volatile("nop");
	driverControlPort  &= ~(1<<driverLE);
	
	// Nächte Ebene aktivieren
	layerPort = currentLayerSignal;							

	// Variablen in den Speicher zurückschreiben
	currentLayerSignalSave = currentLayerSignal;
	currentLayerNumberSave = currentLayerNumber;
}


// Übernimmt den nächsten berechneten Frame als aktuellen Frame
ISR(TIMER1_COMPA_vect){
	
	if(frameReady){
		uint8_t *ptr = currentFrame;
		currentFrame = nextFrame;
		nextFrame = ptr;
	
		frameReady = 0;
	}

}


// Wartet, bis der nächste Frame angezeigt werden soll
void waitForNextFrame(void){
	frameReady = 1;
	while(frameReady){
	}
}

