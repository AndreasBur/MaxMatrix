/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "GameOfLife.h"

GameOfLife::GameOfLife(){
}

void GameOfLife::begin(void){
	coloured_voxels = 0;
	fadeOutDone = 0;
}

uint8_t GameOfLife::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	uint8_t neighbor_count = 0;
	uint8_t rx, ry, rz;
	VOXEL v;

	if(fadeIn){
		begin();
		memset(arrayP, 0, SIZE3*sizeof(COLOUR));

		for(uint16_t i = 0; i < 30; i++){
			rx = rand()%4;
			ry = rand()%4;
			rz = rand()%4;

			if(is_zero(arrayP[ar(rx, ry, rz)])){
				coloured_voxels++;
			}

			arrayP[ar(rx, ry, rz)] = colourWheel.get_colour(colourPos);
		}
		
		return 0;
	}

	if(fadeOut){
		for(uint8_t i = 0; i < 5; i++){
			v = random_voxel(arrayP);
			arrayP[ar(v.x, v.y, v.z)] = {0,0,0};
			coloured_voxels--;
			if(coloured_voxels == 0){
				fadeOutDone = 1;
			}
		}
	}else{
		for(uint8_t x = 0; x < SIZE; x++){
			for(uint8_t y = 0; y < SIZE; y++){
				for(uint8_t z = 0; z < SIZE; z++){
					neighbor_count = neighbors(arrayP, x, y, z);
					if(is_zero(arrayP[ar(x, y, z)])){
						if(neighbor_count == 4){
							arrayP[ar(x, y, z)] = colourWheel.get_colour(colourPos + x*2 + z*3 + y*1);
							coloured_voxels++;
						}
					}else{
						if(!(neighbor_count == 2 || neighbor_count == 3)){
							arrayP[ar(x, y, z)] = {0, 0, 0};
							coloured_voxels--;
						}
					}
				}
			}
		}
	}

	increment_colour_pos(2);

	return fadeOutDone;
}

uint8_t GameOfLife::neighbors(COLOUR *arrayP, uint8_t x, uint8_t y, uint8_t z){
	uint8_t neighbor_count = 0;

	for(int8_t x_ = x-1; x_ <= x+1; x_++){
		for(int8_t y_ = y-1; y_ <= y+1; y_++){
			for(int8_t z_ = z-1; z_ <= z+1; z_++){
				if(!(z_ < 0 || z_ >= SIZE || y_ < 0 || y_ >= SIZE || x_ < 0 || x_ >= SIZE || (x_ == z && y_ == y && z_ == z))){
					if(!is_zero(arrayP[ar(x_, y_, z_)])){
						neighbor_count++;
					}
				}
			}
		}
	}

	return neighbor_count;
}