/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Wisps.h"

Wisps::Wisps(){
}

void Wisps::begin(void){
	change = 0;
	slowGrow = 0;
	grow = 1;
	shrink = 1;
	fadeOutDone = 0;
	
	for(uint8_t j = 0; j < numWisps; j++){
		wispLength[j] = 0;
		theta[j] = 0;
		phi[j] = 0;
		colP[j] = (j+1)*COLOUR_WHEEL_LENGTH/numWisps;
		wisps[0][j] = {rand()%8, rand()%8, rand()%8, colourWheel.get_colour(colP[j])};
	}
}

uint8_t Wisps::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){	
	if(fadeIn){
		begin();
		memset(arrayP, 0, SIZE3*sizeof(COLOUR));
	}

	for(uint8_t j = 0; j < numWisps; j++){
		for(int8_t i = wispLength[j]; i >= 0; i--){
			if(i == 0){
				do{
					dummyX = L*mySin(theta[j]) + L*myCos(phi[j]);
					next_x = myRound(wisps[i][j].x + dummyX);
					direction[j].x = (int8_t)(dummyX/myAbs(dummyX));
					
					dummyY = L*myCos(theta[j]);
					next_y = myRound(wisps[i][j].y + dummyY);
					direction[j].y = (int8_t)(dummyY/myAbs(dummyY));
					
					dummyZ = L*mySin(phi[j]);
					next_z = myRound(wisps[i][j].z + dummyZ);
					direction[j].z = (int8_t)(dummyZ/myAbs(dummyZ));				
				}while(!checkMove(next_x, next_y, next_z, j));
							
				wisps[i][j] = {next_x, next_y, next_z, colourWheel.get_colour(colP[j] + i*2)};
			}else{
				wisps[i][j] = {wisps[i-1][j].x, wisps[i-1][j].y, wisps[i-1][j].z, colourWheel.get_colour(colP[j] + i*2)};
			}
			
			change += rand()%2 + 1;
			if(change > 8){
				changeDirection(j);
				change = 0;
			}
		}
	}
	
	if(wispLength[numWisps-grow] < maxWLength-1 && !fadeOut){
		wispLength[numWisps-grow]++;
	}else if(grow < numWisps){
		slowGrow++;
		if(slowGrow > 30){
			grow++;
			slowGrow = 0;
		}
	}
	
	if(fadeOut){
		if(wispLength[numWisps-shrink] > 0){
			wispLength[numWisps-shrink]--;
		}else if(shrink < numWisps){
			slowShrink++;
			if(slowShrink > 30){
				shrink++;
				slowShrink = 0;
			}
		}else{
			fadeOutDone = 1;
			endBuck.vox = wisps[1][0];
			endBuck.dir = direction[0];
			endBuck.colour_pos = colP[0];
		}
	}	
			
	copyWisps(arrayP);

	return fadeOutDone;
}

void Wisps::copyWisps(COLOUR *arrayP){
	memset(arrayP, 0, SIZE3*sizeof(COLOUR));

	for(uint8_t j = 0; j < numWisps; j++){
		for(uint8_t i = 0; i < wispLength[j]; i++){
			arrayP[ar(wisps[i][j].x, wisps[i][j].y, wisps[i][j].z)] =  wisps[i][j].c;
		}
	}
}

uint8_t Wisps::checkMove(uint8_t x, uint8_t y, uint8_t z, uint8_t j){
	if(x < 0 || x > 7 || y < 0 || y > 7 || z < 0 || z > 7){
		changeDirection(j);
		return 0;
	}
	
	for(uint8_t i = 0; i < wispLength[j]; i++){
		if(wisps[i][j].x == x && wisps[i][j].y == y && wisps[i][j].z == z){
			changeDirection(j);
			return 0;
		}
	}
	
	return 1;
}

void Wisps::changeDirection(uint8_t j){
	theta[j] += myPI/(rand()%8+4)*((rand()%200)>100?0:1);
	phi[j] += myPI/(rand()%8+4)*((rand()%200)>100?0:1);
	
	if(theta[j] >= 2*myPI){
		theta[j] -= 2*myPI;
	}
	
	if(phi[j] >= 2*myPI){
		phi[j] -= 2*myPI;
	}
}
