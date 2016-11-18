/*
 * cube.c
 *
 * Copyright 2012 Thomas Buck <xythobuz@me.com>
 *
 * This file is part of LED-Cube.
 *
 * LED-Cube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LED-Cube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LED-Cube.  If not, see <http://www.gnu.org/licenses/>.
 */
// Uses Timer 1!
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>

#include "cube.h"

#ifndef F_CPU
#define F_CPU 16000000L
#endif

/*
 * 24 fps, 8 layers => 192 Interrupts per second
 * We are at 16MHz, so we count to:
 * 16000000 / 192 = 83333,33...
 * We can only count to 2^16, therefore we have a second counter
 * COUNT * (COUNT2 + 1) = Realcount
 * 41666 * 2 = 83332
 * This is flickering really hard.
 *
 * COUNT2 set to 0 on the other hand, thus producing 48fps,
 * works flicker-free. Now we just return imgFlag / 2 as isFinished(),
 * so we still fake 24fps for the main program...
 */

#define COUNT 41666
#define COUNT2 0

volatile uint8_t imgBuffer[8][8];
volatile uint8_t changedFlag = 0;
volatile uint8_t imgFlag = 0;
volatile uint8_t layer = 0;
volatile uint8_t toggleFlag = 0;

volatile uint32_t timesTriggered = 0;
volatile uint8_t warningDisplayed = 0;

// A fix for our mirrored 64pin cable.
// Look around line 150 to change this
uint8_t lookUp[256] PROGMEM = { 	0,  64,  128,  192,  16,  80,  144,  208,  32,  96,  160,
									224,  48,  112,  176,  240,  4,  68,  132,  196,  20,
									84,  148,  212,  36,  100,  164,  228,  52,  116,  180,
									244,  8,  72,  136,  200,  24,  88,  152,  216,  40,
									104,  168,  232,  56,  120,  184,  248,  12,  76,  140,
									204,  28,  92,  156,  220,  44,  108,  172,  236,  60,
									124,  188,  252,  1,  65,  129,  193,  17,  81,  145,
									209,  33,  97,  161,  225,  49,  113,  177,  241,  5,
									69,  133,  197,  21,  85,  149,  213,  37,  101,  165,
									229,  53,  117,  181,  245,  9,  73,  137,  201,  25,
									89,  153,  217,  41,  105,  169,  233,  57,  121,  185,
									249,  13,  77,  141,  205,  29,  93,  157,  221,  45,
									109,  173,  237,  61,  125,  189,  253,  2,  66,  130,
									194,  18,  82,  146,  210,  34,  98,  162,  226,  50,
									114,  178,  242,  6,  70,  134,  198,  22,  86,  150,
									214,  38,  102,  166,  230,  54,  118,  182,  246,  10,
									74,  138,  202,  26,  90,  154,  218,  42,  106,  170,
									234,  58,  122,  186,  250,  14,  78,  142,  206,  30,
									94,  158,  222,  46,  110,  174,  238,  62,  126,  190,
									254,  3,  67,  131,  195,  19,  83,  147,  211,  35,
									99,  163,  227,  51,  115,  179,  243,  7,  71,  135,
									199,  23,  87,  151,  215,  39,  103,  167,  231,  55,
									119,  183,  247,  11,  75,  139,  203,  27,  91,  155,
									219,  43,  107,  171,  235,  59,  123,  187,  251,  15,
									79,  143,  207,  31,  95,  159,  223,  47,  111,  175,
									239,  63,  127,  191,  255 };

ISR(TIMER1_COMPA_vect) {
	uint8_t latchCtr;
	if (toggleFlag >= COUNT2) {

		if (changedFlag != 0) {
			// The picture changed. Restart!
			layer = 0;
			changedFlag = 0;
		}

		PORTD = 0;
		PORTB &= ~(24); // Disable all Fets

		for (latchCtr = 0; latchCtr < 8; latchCtr++) {
			// Disable all latches
			PORTC &= ~(0xFC); // 2 - 7
			PORTB &= ~(6); // 0 & 1

			PORTA = imgBuffer[layer][latchCtr]; // Put latch data

			// Enable desired latch
			if (latchCtr > 1) {
				PORTC |= (1 << latchCtr);
			} else {
				PORTB |= (1 << (latchCtr + 1));
			}
		}

		// Set FET
		PORTD = (1 << layer); // PD2 to 7
		PORTB = (PORTB & ~(24)) | (((1 << layer) << 3) & 24); // PB3 & 4
		
		// Select next layer
		if (layer < 7) {
			layer++;
		} else {
			layer = 0;
			imgFlag++; // Finished
		}

		toggleFlag = 0;
		timesTriggered++;
	} else {
		toggleFlag++;
	}
}

uint32_t getTriggerCount(void) {
	return timesTriggered;
}

void setImage(uint8_t *img) {
	uint8_t i, j;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		changedFlag = 1;
		imgFlag = 0;
		toggleFlag = 0;
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				// First line uses the lookUp Table to account for our wrong connection
				imgBuffer[j][7 - i] = ~(pgm_read_byte(&(lookUp[img[j + (i * 8)]])));

				// This line is simply displaying the data, for correct connections
				// imgBuffer[j][i] = ~(img[j + (i * 8)]);

				// Uncomment the desired line, comment the other...
			}
		}
	}
}

void fillBuffer(uint8_t val) {
	uint8_t i, j;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		changedFlag = 1;
		imgFlag = 0;
		toggleFlag = 0;
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				imgBuffer[i][j] = ~(val);
			}
		}
	}
}

uint8_t isFinished(void) {
	return (imgFlag / 2);
}

void initCube(void) {
	uint8_t x, y;

	TCCR1B |= (1 << CS10) | (1 << WGM12); // Prescaler: 1, CTC Mode
	OCR1A = COUNT;
	TIMSK = (1 << OCIE1A); // Enable Output Compare Interrupt

	fillBuffer(0); // Clear memory
	sei(); // Enable interrupts

	// Show test animation
	for (x = 0; x < 8; x++) {
		for (y = 0; y < 8; y++) {
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				imgBuffer[x][y] &= ~(0xFF); // Set a pixel
				changedFlag = 1;
				imgFlag = 0;
				toggleFlag = COUNT2; // Ensure next interrupts starts displaying
			}
			while(imgFlag < 1); // Wait for frame to display
		}
	}

	cli(); // Disable interrupts

	timesTriggered = 0;
	imgFlag = 0;
	toggleFlag = 0;
}
