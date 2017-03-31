/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef FLIP_h
#define FLIP_h

class Flip : public Animation{
    public:
        Flip();
        uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
    protected:
    	void begin(void);
    	float theta;
};

#endif