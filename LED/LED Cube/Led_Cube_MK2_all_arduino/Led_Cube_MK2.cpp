/*  
NEW SIMPLIFIED LED CUBE SOFTWARE    

now you can run a few animations entirely from your Arduino, NO PC REQUIRED!
I have had to squeeze a lot out of the Arduino to do this, 
so no use of the clunky old Arduino libraries.
  
Feel free to use this in your own projects,
just please spread the love and reference me and my site

Cheers
Nick Schulze
www.HowNotToEngineer.com
  
*/

#include "animation.h"

#define aMax 7

animation animate;
cube ledCube;
int32_t count = 0;
uint8_t layer = 1;
uint8_t intr = 0;
volatile uint8_t lat = 0;
uint32_t sw = 0;
volatile uint8_t a = 4;

//MAIN
int main(void){
	ledCube.clear_cube();
    
	while(1){
        //switch the animations
        if(lat == 1){
            switch(a){
                case 1:
                    animate.bouncePlane(ledCube, 'X');
                    break;
                case 2:
                    animate.bouncePlane(ledCube, 'Y');
                    break;
                case 3:
                    animate.bouncePlane(ledCube, 'Z');
                    break;
                case 4:
                    animate.cubePulse(ledCube);
                    break;
                case 5:
                    animate.helix(ledCube);
                    break;
                case 6:
                    animate.rain(ledCube);
                    break;
                case 7:
                    animate.randomExpand(ledCube);
                    break;
            }
            lat = 0;
        }
	}

	return 0;
}

//constant frequency multiplexing to make sure all layers are the same brightness 
ISR(TIMER1_OVF_vect){  
    intr++;
    
    //multiplex at half the interrupt frequency
    if(intr > 1){
        sw++;
        //yes the layer function is called in the interrupt for a reason
        ledCube.fill_layer(layer);
        layer++;
        if(layer > 8){
            layer = 1;
        }
        intr = 0;
    }
    
    //change the animation with a big ugly conditional
    if(sw > 25000 || ((a == 2 || a == 3) && sw > 2500) || (a == 1 && sw > 3500)){
        a++;
        if(a > aMax){
            a = 1;
        }
        sw = 0;
        animate.reset();
        ledCube.clear_cube();
    }
    
    //set the animation update flag every interrupt
    lat = 1;
    
    //reset the timer
	TCNT1 = 0xFFF0;
}