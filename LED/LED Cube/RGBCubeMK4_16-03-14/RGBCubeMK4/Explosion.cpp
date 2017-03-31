/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Explosion.h"

Explosion::Explosion(){
}

void Explosion::begin(void){
    r = 0;
    Rsquared = r*r;
    cRsquared = 0;
    fadeOutDone = 0;
    dir = 1;
}

uint8_t Explosion::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
    if(fadeIn){
        begin();
        memset(arrayP, 0, SIZE3*sizeof(COLOUR));
        rc = startBuck.colour_pos;
    }
    
    for(uint8_t x = 0; x < SIZE; x++){
        for(uint8_t y = 0; y < SIZE; y++){
            for(uint8_t z = 0; z < SIZE; z++){
                X = myMap(x, 0, SIZE-1, -SIZE/2, SIZE/2);
                Y = myMap(y, 0, SIZE-1, -SIZE/2, SIZE/2);
                Z = myMap(z, 0, SIZE-1, -SIZE/2, SIZE/2);
                cRsquared = X*X + Y*Y + Z*Z;
                Rsquared = r*r;
                if(cRsquared >= Rsquared-r/2 && cRsquared <= Rsquared+r/2){
                    if(dir == 1){
                       arrayP[ar(x, y, z)] =  colourWheel.get_colour(rc + r*20);
                    }else{
                      arrayP[ar(x, y, z)] = {0,0,0};
                    }
                }
            }
        }
    }
    
    r += dir*0.8;
    
    if((dir == 1 && r >= 8) ||(dir == -1 && ((!fadeOut && r < 1.5)||(fadeOut && r < 0)))){
        if(dir == -1){		
            memset(arrayP, 0, SIZE3*sizeof(COLOUR));
		    fadeOutDone = 1;
		}
		
		dir*= -1;
        rc++;
    }
    
    return fadeOutDone;
}