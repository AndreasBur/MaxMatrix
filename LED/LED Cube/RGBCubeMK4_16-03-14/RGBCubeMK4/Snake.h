/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef SNAKE_h
#define SNAKE_h

#define maxSLength 70

class Snake : public Animation{
	public:
		Snake();
		uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
	protected:
		void begin(void);
		void copySnake(COLOUR *arrayP);
		uint8_t checkMove(uint8_t x, uint8_t y, uint8_t z);
		void changeDirection(void);
		VOXEL snake[maxSLength];
		uint8_t snakeLength;
		DIR direction;
		uint8_t next_x, next_y, next_z;
		uint32_t stuck;
		uint8_t change;
		uint8_t check;
		uint8_t this_fadeOut;
};

#endif