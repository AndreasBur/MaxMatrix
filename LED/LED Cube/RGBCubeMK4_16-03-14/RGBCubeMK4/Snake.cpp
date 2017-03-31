/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Snake.h"

Snake::Snake(){
}

void Snake::begin(void){
	snakeLength = 0;
	fadeOutDone = 0;
	fadeInDone = 0;
	this_fadeOut = 0;
	stuck = 0;
}

uint8_t Snake::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	if(fadeIn){
		begin();
		memset(arrayP, 0, SIZE3*sizeof(COLOUR));
		direction = startBuck.dir;
		snake[0] = startBuck.vox;
		colourPos = startBuck.colour_pos;
	}
	
	this_fadeOut |= fadeOut;

	if(!this_fadeOut){
		for(int8_t i = snakeLength; i >= 0; i--){
			if(fadeOutDone){
				break;
			}
			
			if(i == 0){
				stuck = 0;
				do{
					if(++stuck > 200){
						this_fadeOut = 1;
						break;
					}
					next_x = snake[i].x + direction.x;
					next_y = snake[i].y + direction.y;
					next_z = snake[i].z + direction.z;
				}while(!checkMove(next_x, next_y, next_z)); 
				
				if(!this_fadeOut){
					snake[i] = {next_x, next_y, next_z, colourWheel.get_colour(colourPos)};
				}
				
				increment_colour_pos(2);
			}else{
				snake[i] = {snake[i-1].x, snake[i-1].y, snake[i-1].z, snake[i-1].c};
			}
		}
		
		if(!this_fadeOut && snakeLength < maxSLength-1){
			snakeLength++;
		}
	}else{
		snake[snakeLength] = {snake[snakeLength-1].x, snake[snakeLength-1].y, snake[snakeLength-1].z, snake[snakeLength-1].c};
		if(snakeLength > 1){
			snakeLength--;
		}else{
			memset(snake, 0, maxSLength*sizeof(VOXEL));
			fadeOutDone = 1;
		}
	}
	
	change += rand()%2 + 1;
	if(change > 6){
		changeDirection();
		change = 0;
	}
		
	copySnake(arrayP);

	return fadeOutDone;
}

void Snake::copySnake(COLOUR *arrayP){
	memset(arrayP, 0, SIZE3*sizeof(COLOUR));
	for(uint8_t i = 0; i < snakeLength; i++){
		arrayP[ar(snake[i].x, snake[i].y, snake[i].z)] = snake[i].c;
	}
}

uint8_t Snake::checkMove(uint8_t x, uint8_t y, uint8_t z){
	if(x < 0 || y < 0 || z < 0 || x >= SIZE || y >= SIZE || z >= SIZE){
		changeDirection();
		return 0;
	}
	
	for(uint8_t i = 0; i < snakeLength; i++){
		if(snake[i].x == x && snake[i].y == y && snake[i].z == z){
			changeDirection();
			return 0;
		}
	}
	
	return 1;
}

void Snake::changeDirection(void){
	int8_t rand_dir = rand()%200+1 > 100?-1:1;
	
	if(direction.x != 0){
		direction.x = 0;
		if(rand()%200+1 > 100){
			direction.y = rand_dir;	
		}else{
			direction.z = rand_dir;
		}		
	}else if(direction.y != 0){
		direction.y = 0;
		if(rand()%200+1 > 100){
			direction.x = rand_dir;	
		}else{
			direction.z = rand_dir;
		}
	}else if(direction.z != 0){
		direction.z = 0;
		if(rand()%200+1 > 100){
			direction.y = rand_dir;	
		}else{
			direction.x = rand_dir;
		}
	}	
}
