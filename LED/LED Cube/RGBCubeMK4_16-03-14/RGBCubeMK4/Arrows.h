/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef ARROWS_h
#define ARROWS_h

const uint8_t arrowArray[] = {0x18, 0x3C, 0x7E, 0x7E, 0x7E, 0x66, 0x66, 0x42};
const uint8_t lag[] = {4, 14, 0};

class Arrows : public Animation{
    public:
        Arrows();
        uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
    protected:
    	void begin(void);
    	void arrow(uint8_t type);
		void loop(uint8_t type);
		void outerRotate(uint8_t type);
		void copyColumn(uint8_t type, uint8_t i, uint8_t j, uint8_t col);
		uint8_t ORArray[28][3];
		int8_t rotatePos[3];
		uint16_t arrowColour[3];
		uint8_t startup[3];
		uint8_t startArrow;
		uint8_t this_fadeOut;
		COLOUR *array_pointer;
};

#endif