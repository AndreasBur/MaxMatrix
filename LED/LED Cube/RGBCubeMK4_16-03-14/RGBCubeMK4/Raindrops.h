/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef RAINDROPS_h
#define RAINDROPS_h

#define maxNumDrops 10

class Raindrops : public Animation{
    public:
        Raindrops();
        uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
    protected:
		void begin(void);
		void copyDrops(void);
		VOXEL drops[maxNumDrops];
		int8_t numDrops;
		COLOUR *array_pointer;
};

#endif