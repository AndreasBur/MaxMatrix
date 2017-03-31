/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef SPIN_h
#define SPIN_h

class Spin : public Animation{
	public:
		Spin();
		uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
	private:
		void begin(void);
		float distort;
};
	
#endif