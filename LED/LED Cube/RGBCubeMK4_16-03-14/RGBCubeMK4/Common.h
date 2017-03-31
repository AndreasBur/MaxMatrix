/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include <stdlib.h>
#include <string.h>
#include <sys/attribs.h>
#include <inttypes.h>
#include <p32xxxx.h>
#include <plib.h>
#include "Setup.h"

#ifndef COMMON_h
#define COMMON_h

#define myPI  3.14159265358979323846
#define myDPI 1.2732395
#define myDPI2 0.40528473

typedef struct{
	unsigned layer_f:1;
	unsigned animate_f:1;
}FLAGS;

typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}COLOUR;

extern FLAGS flags;

#endif