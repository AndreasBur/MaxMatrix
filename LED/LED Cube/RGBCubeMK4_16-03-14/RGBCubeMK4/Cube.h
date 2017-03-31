/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "ColourWheel.h"
#include "Common.h"
#include "Function.h"
#include "SineWave.h"
#include "Spin.h"
#include "Wisps.h"
#include "Spiral.h"
#include "Explosion.h"
#include "Arrows.h"
#include "Raindrops.h"
#include "Snake.h"
#include "Atom.h"
#include "GameOfLife.h"

#ifndef CUBE_h
#define CUBE_h

typedef struct{
	uint8_t layer;
	uint8_t bit_pos;
	uint32_t delay;
	uint32_t animate_c;
}BAM_STATE;

typedef struct{
	uint8_t print_f;
	uint8_t anim;
	uint8_t sec;
	uint16_t number_played;
}MSG;


#define OE 0x0001
#define SDIR 0x00008
#define SDIG 0x0400
#define SDIB 0x0200
#define CLK 0x0004
#define LE 0x00002
#define ENABLE_CUBE LATDCLR = 0x0001
#define DISABLE_CUBE LATDSET = LATDSET = 0x0001
#define LATCH do{LATDSET = LE;LATDCLR = LE;}while(0);
const uint8_t LAYER[SIZE] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/*
#define OE 0x02 
#define SDIR 0x04
#define SDIG 0x08 
#define SDIB 0x0400
#define CLK 0x0200 
#define LE 0x01 
#define ENABLE_CUBE LATDCLR = 0x00002
#define DISABLE_CUBE LATDSET = 0x00002
#define LATCH do{LATDSET = LE;LATDCLR = LE;}while(0);
const uint8_t LAYER[SIZE] = {0x40, 0x80, 0x10, 0x20, 0x04, 0x08, 0x01, 0x02};
*/

#define LAYERS LAYER[0]|LAYER[1]|LAYER[2]|LAYER[3]|LAYER[4]|LAYER[5]|LAYER[6]|LAYER[7]


const uint16_t minimum_interrupt_delay = F_CPU/(SIZE*REFRESH_RATE*(1<<BAM_RESOLUTION));

class Cube{
	public:
		Cube();
		void init(void);
		void multiplex_cube(void);
		void manage_animations(void);
		MSG log_msg(void);
		uint16_t analogueRead(void);
	private:
		Animation *aPoint;
		uint16_t refresh_ratio;
		uint8_t currentAnimation;
		uint32_t frameCount;
		uint32_t error_count;
		uint8_t fadeIn, fadeOut;
		COLOUR cubeArray[SIZE3];
		BAM_STATE bam;
		MSG log;
		BUCK buck, returnedBuck;

		SineWave sineWave;
		Spin spin;
		Wisps wisps;
		Spiral spiral;
		Explosion explosion;
		Arrows arrows;
		Raindrops raindrops;
		Snake snake;
		Atom atom;
		GameOfLife gameOfLife;
};

extern Cube cube;

#endif