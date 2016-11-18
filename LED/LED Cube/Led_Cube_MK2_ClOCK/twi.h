#include "cube.h"

#ifndef _twi_h_
#define _twi_h_

class twi{
	public: 
		twi();
		void begin(void);
		void beginTransmission(uint8_t address);
		void write(uint8_t data);
		uint8_t endTransmission(void);
		uint8_t requestFrom(uint8_t address, uint8_t length);
		uint8_t receive(void);
	private:
};

#endif
