/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Common.h"
#include "Function.h"

#ifndef COLOURWHEEL_h
#define COLOURWHEEL_h

class ColourWheel{
	public:
		ColourWheel();
		void fill_colour_wheel(void);
		COLOUR get_colour(int16_t p);
		COLOUR get_next_colour(void);
	private:
		COLOUR array[COLOUR_WHEEL_LENGTH];
		int16_t pos;
};

extern ColourWheel colourWheel;

#endif