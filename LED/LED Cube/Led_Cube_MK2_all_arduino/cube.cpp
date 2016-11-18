#include "cube.h"

//the constructor sets up the timer and IOs
cube::cube(){
    cli();
	LE = 2;
	SDI = 3;
	CLK = 5;
	
	SB(DDRD, LE);
	SB(DDRD, SDI);
	SB(DDRD, CLK);
	PORTD = 0x00;
    
    //setup timer 1
	TCCR1A |= 0x00;
	TCNT1 = 0xFFF0; 
	TIMSK1 |= 0x01;
    TCCR1B |= 0x03;
    
    sei();
}

//send one data byte now with inbuilt flip statement 
void cube::send_data(uint8_t data, uint8_t flip){
    switch(flip){
        case 0:
            for(uint8_t i = 0; i < 8; i++){
                if(data & 0x01<<i){
                    SB(PORTD, SDI);
                }
                PB(PORTD, CLK);
                CB(PORTD, SDI);
            } 
            break;
        case 1:
            for(int8_t i = 7; i >= 0; i--){
                if(data & 0x01<<i){
                    SB(PORTD, SDI);
                }
                PB(PORTD, CLK);
                CB(PORTD, SDI);
            } 
            break;
    }
}

//fill an entire layer
void cube::fill_layer(uint8_t layer){
	send_data(0x01<<(layer-1), 0);
	send_data(0x00, 0);

	for(int8_t j = (8*layer)-1; j >= 0+8*(layer-1); j--){
		if(j%2 == 0){
			send_data(cubeData[j], 0);
		}else{
			send_data(cubeData[j], 1);
		}  
	}
	PB(PORTD, LE);
}

//clear everything
void cube::clear_cube(void){
    CB(PORTD, SDI);
	for(uint8_t i = 0; i < 80; i++){
		PB(PORTD, CLK);  
	}
	PB(PORTD, LE);
	CB(PORTD, CLK);
}

//set a voxel in my space saver cube array
void cube::setVoxel(uint8_t x, uint8_t y, uint8_t z){
	cubeData[z*8 + y] |= 0x01<<x;
}

//clear a single voxel in my space saver cube array
void cube::clearVoxel(uint8_t x, uint8_t y, uint8_t z){
	cubeData[z*8 + y] &= ~(0x01<<x);
}

//return the value of the voxel in my cube array
uint8_t cube::getVoxel(uint8_t x, uint8_t y, uint8_t z){
    if(cubeData[z*8 + y] & 0x01<<x){
        return 1;
    }else{
        return 0;
    }
}

//clear all the voxels in the array
void cube::clearVoxels(void){
	memset(cubeData, 0, 64*sizeof(uint8_t));
}