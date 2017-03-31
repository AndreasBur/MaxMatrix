/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Spin.h"

Spin::Spin(){
}

void Spin::begin(void){
	fadeInDone = 1;
	fadeOutDone = 0;
	phase = 0;
	distort = 1;
	dir = 1;
}

uint8_t Spin::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	if(fadeIn){
		begin();
		memset(arrayP, 0, SIZE3*sizeof(COLOUR));
	}

    for(uint8_t x = 0; x < SIZE; x++){
        for(uint8_t y = 0; y < SIZE; y++){
            for(uint8_t z = 0; z < SIZE; z++){
				X = (x-3.5)*mySin(phase+myMap(y, 0, SIZE, 0, distort*myPI/2));
				Z = (z-3.5)*myCos(phase+myMap(y, 0, SIZE, 0, distort*myPI/2));
				if(myAbs(X-Z) < 0.7){
					arrayP[ar(x, y, z)] = colourWheel.get_colour(colourPos + x*2 +y*3);
                }else{
                	arrayP[ar(x, y, z)] = {0,0,0};
                }
            }
        }
    }

    increment_colour_pos(2);

    distort+=0.02*dir;
	
	if(distort > 1.5 || distort < 0.5){
		dir*= -1;
	}

	phase += myPI/10;
	
	if(phase >= 2*myPI){
		phase -= 2*myPI;
	}

	fadeOutDone = fadeOut;
	return fadeOutDone;
}