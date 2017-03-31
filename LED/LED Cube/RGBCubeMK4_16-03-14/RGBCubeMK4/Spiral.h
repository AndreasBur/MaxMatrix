/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef SPIRAL_h
#define SPIRAL_h

class Spiral : public Animation{
	public:
		Spiral();
		uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
	protected:
		void begin(void);
		uint8_t bottom, top; 
		float narrow;
		float speed;
};

#endif