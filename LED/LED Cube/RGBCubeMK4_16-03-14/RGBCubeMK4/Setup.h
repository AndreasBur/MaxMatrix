/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#ifndef SETUP_h
#define SETUP_h

#define F_CPU 80000000UL
#define REFRESH_RATE 90
#define BAM_RESOLUTION 4

#define SIZE 8
#define SIZE2 64
#define SIZE3 512

#define COLOUR_WHEEL_LENGTH 512

typedef enum{
	SINEWAVE,	//0
	SNAKE,		//1	
	SPIRAL, 	//2
	EXPLOSION, 	//3
	ARROWS,		//4
	SPIN,		//5
	WISPS,		//6
	RAINDROPS,	//7
	ATOM,		//8
	GAMEOFLIFE,	//9
	NUMBER_OF_ANIMATIONS = 10
}ANIMATIONS;

typedef struct{
	ANIMATIONS a;
	uint8_t time;
	uint8_t frame_rate;
}ANIMATION;

//ORDER OF ANIMATION, THE RESPECTIVE RUNTIME IN SECONDS AND ANIMATIONS THE FRAME RATE
const ANIMATION animationOrder[NUMBER_OF_ANIMATIONS] = {{GAMEOFLIFE, 10, 10},{RAINDROPS, 15, 22},{SPIN, 15, 22},{ATOM, 10, 42},{WISPS, 20, 25},{SNAKE, 30, 20},{ARROWS,15, 20},{SPIRAL, 18, 30},{EXPLOSION, 1, 30},{SINEWAVE, 15, 20}};

#endif