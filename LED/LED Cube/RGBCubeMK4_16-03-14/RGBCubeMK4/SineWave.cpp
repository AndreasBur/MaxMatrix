/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "SineWave.h"

SineWave::SineWave(){
}

void SineWave::begin(void){
	fadeInDone = 1;
	fadeOutDone = 0;
	phase = myPI;
}

uint8_t SineWave::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	if(fadeIn){
		begin();
		memset(arrayP, 0, SIZE3*sizeof(COLOUR));
	}

	fadeOutDone = 1;
	
    for(uint8_t x = 0; x < SIZE; x++){
		for(uint8_t y = 0; y < SIZE; y++){
			X = myMap(x, 0, SIZE-1, -2, 2);
			Y = myMap(y, 0, SIZE-1, -2, 2);
			
			//RECALCULTE PREVIOUS Z
			Z = mySin(phase - myPI/8 + mySqrt(X*X + Y*Y));
			Z = myRound(myMap(Z,-1,1,0,SIZE-1));
			
			//CLEAR PREVIOUS Z
			if(!fadeOut || (fadeOut && Z!= 0)){
				arrayP[ar(x, y, Z)] = {0,0,0};
			}
			
			//CALCULATE NEXT Z
			Z = mySin(phase + mySqrt(X*X + Y*Y));
			Z = myRound(myMap(Z,-1,1,0,SIZE-1));
			
			//LOAD COLOUR, IF FADEOUT THEN HOLD COLOUR AT Z = 0
			if(!fadeOut || (fadeOut && is_zero(arrayP[ar(x, y, 0)]))){
				arrayP[ar(x, y, Z)] = colourWheel.get_colour(colourPos + Z*10);
				fadeOutDone = 0;
			}
		}
    }

    increment_colour_pos(2);

	phase += myPI/8;
	if(phase >= 2*myPI){
		phase -= 2*myPI;
	}

	return fadeOutDone;
}