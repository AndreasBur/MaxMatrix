/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef WISPS_h
#define WISPS_h

#define maxWLength 10
#define numWisps 3
#define L 1

class Wisps : public Animation{
	public:
		Wisps();
		uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
	protected:
		void begin(void);
		void copyWisps(COLOUR *arrayP);
		uint8_t checkMove(uint8_t x, uint8_t y, uint8_t z, uint8_t j);
		void changeDirection(uint8_t j);
		VOXEL wisps[maxWLength][numWisps];
		VOXEL lastPos[numWisps];
		int8_t wispLength[numWisps];
		DIR direction[numWisps];
		uint16_t colP[numWisps];
		int8_t next_x, next_y, next_z;
		uint32_t stuck;
		uint8_t change;
		uint8_t slowGrow, grow, slowShrink, shrink;
		float theta[numWisps];
		float phi[numWisps];
		float dummyX, dummyY, dummyZ;
};

#endif