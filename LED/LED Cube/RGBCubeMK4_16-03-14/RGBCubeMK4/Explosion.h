/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef EXPLOSION_h
#define EXPLOSION_h

class Explosion : public Animation{
    public:
        Explosion();
        uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
    protected:
    	void begin(void);
        double r, cRsquared, Rsquared;
        double rc;
};

#endif