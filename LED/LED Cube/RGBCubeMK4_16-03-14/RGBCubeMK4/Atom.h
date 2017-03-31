/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Animation.h"

#ifndef ATOM_h
#define ATOM_h

#define E 15
#define rad 4

class Atom : public Animation{
    public:
        Atom();
        uint8_t animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck);
    protected:
    	void begin(void);
    	void copyAtom(COLOUR *arrayP);
		int8_t elec;
		VOXEL electron[E];
		float theta[E];
		float beta[E];
		float temp; 
};

#endif