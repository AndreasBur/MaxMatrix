/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
        
        Hi Everyone, feel free to use these animations on your own RGB Cubes.
        I did spend quite a lot of time on all of this code so if you do use it
        on the internet somewhere please add a link to my site www.HowNotToEngineer.com
        Cheers
        Nick Schulze
*/

#include "Cube.h"

void setup(void){
	cube.init();
	Serial.begin(115200);
	Serial.println("START");
}

void loop(void){
	if(flags.animate_f){
		cube.manage_animations();
		flags.animate_f = 0;
	}

	if(cube.log_msg().print_f){
		Serial.print("LOG: \nLast animation runtime = ");
		Serial.print((int)cube.log_msg().sec);
		Serial.print(" seconds.\nNumber of animations since start = ");
		Serial.println((int)cube.log_msg().number_played);
		Serial.print("Begin animation #");
		Serial.println((int)cube.log_msg().anim);
	}
}

extern "C"{
	void __ISR(_TIMER_5_VECTOR, ipl3) TimerHandler(void){
	    cube.multiplex_cube();

		mT5ClearIntFlag();	
	}
}