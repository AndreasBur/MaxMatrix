/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Raindrops.h"

Raindrops::Raindrops(){
}

void Raindrops::begin(void){
	fadeOutDone = 0;
	fadeInDone = 0;
	numDrops = 0;
	memset(drops, 0, maxNumDrops*sizeof(VOXEL));
}

uint8_t Raindrops::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	if(fadeIn){
		begin();
		array_pointer = arrayP;
	}
	
	memset(arrayP, 0, SIZE3*sizeof(COLOUR));

	for(uint8_t j = 0; j < maxNumDrops; j++){
		if(drops[j].z > 0){
			drops[j].z -= 1;
		}else{
			drops[j] = {0,0,0,{0,0,0}};
			numDrops--;
			if(fadeOut && numDrops == 0){
				fadeOutDone = 1;
			}
		}
	}
	
	for(uint8_t i = 0; i < rand()%4+1; i++){
		uint8_t d = 0;
		if(numDrops >= maxNumDrops || fadeOut){
			break;
		}
		
		while(!is_zero(drops[d].c)){
			d++;
			if(d >= maxNumDrops-1){
				break;
			}
		}
		
		drops[d] = {rand()%8, rand()%8, 7, colourWheel.get_colour(colourPos+2*d)};
		
		numDrops++;
	}
	
	increment_colour_pos(2);
	
	copyDrops();

	return fadeOutDone;
}

void Raindrops::copyDrops(void){
	for(uint8_t i = 0; i < maxNumDrops; i++){
		array_pointer[ar(drops[i].x, drops[i].y, drops[i].z)] = drops[i].c;
	}
}