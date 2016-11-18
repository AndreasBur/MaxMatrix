#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include "WProgram.h"

#ifndef _CUBE_H_
#define _CUBE_H_

#define PI 3.14159
#define Xd 8
#define Yd 8
#define Zd 8
#define centre 3.5
#define Vd Xd*Yd*Zd

#define SB(x,n) (x|=(1<<n)) 
#define CB(x,n) (x&=~(1<<n))
#define TB(x,n) (x^=(1<<n))
#define PB(x,n) do{SB(x,n);CB(x,n);}while(0)

class cube{
	public:
		cube();
		void send_data(uint8_t data, uint8_t flip);
		void fill_layer(uint8_t layer);
		void clear_cube(void);
		void setVoxel(uint8_t x, uint8_t y, uint8_t z);
        void clearVoxel(uint8_t x, uint8_t y, uint8_t z);
        uint8_t getVoxel(uint8_t x, uint8_t y, uint8_t z);
		void clearVoxels(void);
	private:
		uint8_t SDI;
		uint8_t CLK;
		uint8_t LE;
		uint8_t cubeData[64];
};

#endif