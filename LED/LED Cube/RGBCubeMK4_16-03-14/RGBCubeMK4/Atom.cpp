/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Atom.h"

Atom::Atom(){
}

void Atom::begin(void){
	elec = 0;
	fadeInDone = 0;
	fadeOutDone = 0;
	for(uint8_t i = 0; i < E; i++){
		theta[i] = myMap(i, 0, E, 0, 2*myPI);
		beta[i] = myMap(i, 0, E, -myPI, myPI);
	}
}

uint8_t Atom::animate(COLOUR *arrayP, uint8_t fadeIn, uint8_t fadeOut, BUCK startBuck){
	if(fadeIn){
		begin();
	}
	
	memset(arrayP, 0, SIZE3*sizeof(COLOUR));

	for(uint8_t i = 0; i < E; i++){
		X = rad*mySin(theta[i]);
		Y = rad*myCos(theta[i]);
		Z = Y*mySin(beta[i]);
		
		electron[i] = {myRound(myMap(X, -4, 4, 0, 7)), myRound(myMap(Y, -4, 4, 0, 7)), myRound(myMap(Z, -4, 4, 0, 7)), colourWheel.get_colour((i+1)*COLOUR_WHEEL_LENGTH/E)};
	
		theta[i] += myPI/12;
		
		if(theta[i] >= 2*myPI){
			theta[i] -= 2*myPI;
			
			if(i == 0){
				if(!fadeInDone){
					elec++;
					if(elec >= E){
						fadeInDone = 1;
					}
				}else{
					if(fadeOut){
						elec--;
						if(elec <= 1){
							fadeOutDone = 1;
						}
					}
				}
			}
		}
	}
	
	temp = beta[0];
	for(uint8_t i = 0 ; i < E-1; i++){
		beta[i] = beta[i+1];
	}
	beta[E-1] = temp;
	
	copyAtom(arrayP);

	return fadeOutDone;
}


void Atom::copyAtom(COLOUR *arrayP){
	for(uint8_t i = 0; i < elec; i++){
		arrayP[ar(electron[i].x, electron[i].y, electron[i].z)] = electron[i].c;
	}
}
