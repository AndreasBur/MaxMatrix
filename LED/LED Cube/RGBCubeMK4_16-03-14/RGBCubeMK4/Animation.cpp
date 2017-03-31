/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

Animation::Animation(){
}

Animation::~Animation(){
}

uint8_t Animation::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
}

BUCK Animation::collect_buck(void){
	return endBuck;
}

void Animation::increment_colour_pos(uint8_t i){
	colourPos += i;
	while(colourPos >= COLOUR_WHEEL_LENGTH){
		colourPos -= COLOUR_WHEEL_LENGTH;
	}
}

uint8_t Animation::is_zero(COLOUR c){
	uint8_t zero = c.r|c.g|c.b;

	return !zero;
}

VOXEL Animation::random_voxel(COLOUR *arrayP){
	uint8_t x = rand()%8;
	uint8_t y = rand()%8;
	uint8_t z = rand()%8;

	VOXEL vret;
	uint16_t count = 0;

	while(is_zero(arrayP[ar(x, y, z)])){
		x = rand()%8;
		y = rand()%8;
		z = rand()%8;
		if(++count > 500){
			break;
		}
	}

	vret = {x, y, z, arrayP[ar(x, y, z)]};

	return vret;
}