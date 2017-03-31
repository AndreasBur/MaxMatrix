/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef SINEWAVE_h
#define SINEWAVE_h

class SineWave : public Animation{
	public:
		SineWave();
		uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
	private:
		void begin(void);
};
	
#endif