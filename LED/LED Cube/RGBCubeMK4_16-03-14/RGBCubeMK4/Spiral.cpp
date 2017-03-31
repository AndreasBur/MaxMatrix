/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Spiral.h"

Spiral::Spiral(){
}

void Spiral::begin(void){
	fadeOutDone = 0;
	fadeInDone = 0;
	bottom = 0;
	top = 0;
	narrow = 0;
	speed = 1;
}

uint8_t Spiral::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	if(fadeIn){
		begin();
		memset(arrayP, 0, SIZE3*sizeof(COLOUR));
		colourPos = 0;
	}

	memset(arrayP, 0, SIZE3*sizeof(COLOUR));
	
	//Calculate frame
	for(uint8_t z = bottom; z < top; z++){
		for(uint8_t i = 0; i < 4; i++){
			Y = myCos(phase + myMap(z, 0, SIZE-1, 0, 2*myPI) + i*myPI/8);
			X = mySin(phase + myMap(z, 0, SIZE-1, 0, 2*myPI) + i*myPI/8);
			Y = myMap(Y, -1.1, 0.9, narrow, (float)SIZE-1-narrow);
			X = myMap(X, -1.1, 0.9, narrow, (float)SIZE-1-narrow);

			arrayP[ar((uint8_t)X, (uint8_t)Y, z)] = colourWheel.get_colour(colourPos + 10*z);
		}
	}
	
	increment_colour_pos(2);
	
	//Count periods
	phase += myPI/5*speed;
	if(phase >= 2*myPI){
		phase -= 2*myPI;
		
		//Fade out to the top
		if(fadeOut){
			bottom<2?bottom++:bottom;
			top>5?top--:top;
			speed+=0.6;
			narrow+=0.5;
			if(narrow == 2){
				endBuck.colour_pos = colourPos;
				fadeOutDone = 1;
			}
		}
		
		//Fade in from the bottom
		if(!fadeInDone){
			top++;
			if(top >= SIZE){
				fadeInDone = 1;
			}
		}
	}

	return fadeOutDone;
}
