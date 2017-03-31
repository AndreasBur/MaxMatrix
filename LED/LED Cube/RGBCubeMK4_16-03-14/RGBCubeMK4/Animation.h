/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "ColourWheel.h"
#include "Common.h"
#include "Function.h"

#ifndef ANIMATION_h
#define ANIMATION_h

#define ar(x, y, z) (uint16_t)(z*SIZE2 + y*SIZE + x)

typedef struct{
	int8_t x;
	int8_t y;
	int8_t z;
	COLOUR c;
}VOXEL;

typedef struct{
	int8_t x;
	int8_t y;
	int8_t z;
}DIR;

typedef struct{
	uint16_t colour_pos;
	COLOUR col;
	VOXEL vox;
	DIR dir;
	uint8_t lastAnimation;
}BUCK;

class Animation{
	public:
		Animation();
		virtual ~Animation();
		virtual uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
		BUCK collect_buck(void);
		void increment_colour_pos(uint8_t i);
		uint8_t is_zero(COLOUR c);
		VOXEL random_voxel(COLOUR *arrayP);
	protected:
		BUCK endBuck;
		uint8_t fadeInDone, fadeOutDone;
		uint16_t colourPos;
		int8_t dir;
		float X, Y, Z, phase;
};

#endif