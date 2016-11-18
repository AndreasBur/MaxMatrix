#include "cube.h"

#ifndef _animation_h_
#define _animation_h_

class animation{
	public:
		animation();
        uint8_t bouncePlane(cube &c, uint8_t axis);
        uint8_t randomExpand(cube &c);
        uint8_t helix(cube &c);
        uint8_t rain(cube &c);
        uint8_t cubePulse(cube &c);
        void bouncePos(void);
        void reset(void);
        float get_sinA(int16_t deg);
	private:
        int8_t dir;
        uint8_t pos;
        uint16_t slow;
        uint16_t phase;
        float sinA[91];
        int16_t absA(int16_t in);
};

#endif

