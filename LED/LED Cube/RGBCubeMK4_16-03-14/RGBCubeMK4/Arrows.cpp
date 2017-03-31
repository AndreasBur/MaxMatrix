/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Arrows.h"

Arrows::Arrows(){
}

void Arrows::begin(void){
	fadeOutDone = 0;
	fadeInDone = 0;
	startup = {1, 0, 0};
	startArrow = 0;
}

uint8_t Arrows::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	if(fadeIn){
        begin();
        memset(arrayP, 0, SIZE3*sizeof(COLOUR));
        array_pointer = arrayP;
    }
	
	this_fadeOut = fadeOut;

	memset(arrayP, 0, SIZE3*sizeof(COLOUR));
	memset(ORArray, 0, 28*3*sizeof(uint8_t));
	
	arrow(0);
	arrow(1);
	arrow(2);
	
	if(!fadeInDone){
		(startup[startArrow]<8)?startup[startArrow]++:8;
	}else if(fadeOut){
		(startup[startArrow]>0)?startup[startArrow]--:0;
	}
	
	increment_colour_pos(2);
	
	return fadeOutDone;
}

void Arrows::arrow(uint8_t type){
	for(uint8_t i = 0; i < startup[type]; i++){
		ORArray[i+lag[type]][type] = arrowArray[i];
	}
	
	outerRotate(type);
}

//ROTATE data in ORArray AROUND OUTER PLANES
void Arrows::outerRotate(uint8_t type){
	loop(type);

	//wrap the ORArray arrpound the outside of the cube
	for(int8_t r = 0; r < 28; r++){
		if(r >= 0 && r < SIZE){
			copyColumn(type, SIZE-1-r, 7, ORArray[27-r][type]);
        }else if(r >= SIZE && r < SIZE*2-1){
			copyColumn(type, 0, SIZE - (r-SIZE) - 2, ORArray[27-r][type]);
        }else if(r >= SIZE*2-1 && r < SIZE*3-2){
			copyColumn(type, r-(2*SIZE-2), 0, ORArray[27-r][type]);
        }else if(r >= SIZE*3-2 && r < SIZE*4-3){
			copyColumn(type, 7, r-(3*SIZE-3), ORArray[27-r][type]);
        }
	}
}

//Loop ORArray used for rotate function
void Arrows::loop(uint8_t type){
	uint8_t temp = 0;
	
	//shift and loop the ORArray until the correct position is reached
	for(int8_t n = 0; n < rotatePos[type]; n++){
		//shift first column into a temp array
		temp = ORArray[0][type];

		//shift all columns down
		for(int8_t r = 0; r < SIZE*4-5; r++){
			ORArray[r][type] = ORArray[r+1][type];
		}

		//move data from first column into last
		ORArray[SIZE*4-5][type] = temp;
	}

	//increment/reset rotatePos, this is what gives the rotation effect
	rotatePos[type]++;
	if(rotatePos[type] > 27){
		rotatePos[type] = 0;
		
		if(!fadeInDone){
			if(startup[startArrow]>=8){
				startArrow++;
			}
			if(startArrow > 2){
				fadeInDone = 1;
			}
		}else if(this_fadeOut){
			if(startup[startArrow]<=0){
				startArrow--;
			}
			if(startup[0] <= 1){
				fadeOutDone = 1;
			}
		}
	}
}

void Arrows::copyColumn(uint8_t type, uint8_t i, uint8_t j, uint8_t col){
	switch(type){
		case 0:
			for(uint8_t k = 0; k < SIZE; k++){
				if(col&(0x01<<k)){
					array_pointer[ar(i, j, k)] = colourWheel.get_colour(colourPos + i*20);
				}
			}
			break;
		case 1:
			for(uint8_t k = 0; k < SIZE; k++){
				if(col&(0x01<<k)){
					array_pointer[ar(i, k, j)] = colourWheel.get_colour(colourPos + i*20);
				}
			}
			break;
		case 2:
			for(uint8_t k = 0; k < SIZE; k++){
				if(col&(0x01<<k)){
					array_pointer[ar(k, i, j)] = colourWheel.get_colour(colourPos + i*20);
				}
			}
			break;
	}
}