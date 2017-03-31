/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef GAMEOFLIFE_h
#define GAMEOFLIFE_h

class GameOfLife : public Animation{
    public:
        GameOfLife();
        uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
    protected:
    	void begin(void);
    	uint8_t neighbors(COLOUR *arrayP, uint8_t x, uint8_t y, uint8_t z);
    	int16_t coloured_voxels;
};

#endif