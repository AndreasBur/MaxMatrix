/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "ColourWheel.h"

ColourWheel colourWheel;

ColourWheel::ColourWheel(){
}

void ColourWheel::fill_colour_wheel(void){
	float red, green, blue;
	float c, s;
	int32_t phase = 0;
	int16_t I = 0;

	while(phase < COLOUR_WHEEL_LENGTH){
		s = (1<<BAM_RESOLUTION)*mySin(myPI*(3*phase-I*COLOUR_WHEEL_LENGTH)/(2*COLOUR_WHEEL_LENGTH));
		c = (1<<BAM_RESOLUTION)*myCos(myPI*(3*phase-I*COLOUR_WHEEL_LENGTH)/(2*COLOUR_WHEEL_LENGTH));
		
		red = (I==0?1:0)*s + (I==1?1:0)*c;
		green = (I==1?1:0)*s + (I==2?1:0)*c;
		blue = (I==2?1:0)*s + (I==0?1:0)*c;
		
		array[phase] = {red, green, blue};
		if(++phase >= (1+I)*COLOUR_WHEEL_LENGTH/3){
			I++;
		}
	}
}

COLOUR ColourWheel::get_colour(int16_t p){
	if(p >= COLOUR_WHEEL_LENGTH){
		p -= COLOUR_WHEEL_LENGTH;
	}	
	
	return array[p];
}

COLOUR ColourWheel::get_next_colour(void){
	if(++pos >= COLOUR_WHEEL_LENGTH){
		pos -= COLOUR_WHEEL_LENGTH;
	}
	
	return array[pos];
}