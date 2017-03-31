/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Flip.h"

Flip::Flip(){
}

void Flip::begin(void){
    fadeOutDone = 0;
    fadeInDone = 0;
    theta = myPI/2;
}

uint8_t Flip::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
    if(fadeIn){
        begin();
        memset(arrayP, 0, SIZE3*sizeof(COLOUR));
    }
    
    for(uint8_t x = 0; x < SIZE; x++){
        Y = (float)x*mySin(theta);
        arrayP[ar(x, myRound(Y), 0)] = {15,0,0};
    }

    fadeOutDone = fadeOut;
    return fadeOutDone;
}