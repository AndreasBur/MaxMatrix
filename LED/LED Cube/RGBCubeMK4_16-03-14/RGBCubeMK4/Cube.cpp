/*
        RGB CUBE Animations MK4 
        Nick Schulze September 2013
        HowNotToEngineer.com
*/

#include "Cube.h"

Cube cube;
FLAGS flags;

Cube::Cube(){
	currentAnimation = 0;
	refresh_ratio = REFRESH_RATE/animationOrder[currentAnimation].frame_rate;
	frameCount = 0;
	fadeIn = 1;
	fadeOut = 0;
	log.print_f = 1;
	log.anim = animationOrder[currentAnimation].a;
	log.number_played = 0;
}

void Cube::init(void){
	asm volatile("di");
	
	colourWheel.fill_colour_wheel();

	//setup PORTD
	TRISDCLR = CLK|LE|SDIR|SDIG|SDIB|OE;
	LATDCLR = CLK|LE|SDIR|SDIG|SDIB|OE;
	
	//setup PORTE
	TRISECLR = LAYERS;
	LATESET = LAYERS;
	
	//TIMER
	T4CONSET = 0x0008;	//1:1 PRESCALE, 32bit MODE
	TMR4 = 0;			//CLEAR TIMER REG	
	
	//LOAD PERIOD REG
	bam.delay = minimum_interrupt_delay;
	PR4 = bam.delay;
	
	//ADC 
	AD1CON1 = 0x0000;    //TURN ADC OFF
    AD1PCFG = 0xFFF7;    //AN2 ANALOGUE IN
    AD1CHS = 0x000300000;  //CH0 POS INPUT IS AN2
    AD1CSSL = 0x0000;  //NO SCANNING
    AD1CON3 = 0x0002;  //4x PRESCALLER
    AD1CON2 = 0x0000;  //3.3V Vref 
    AD1CON1SET = 0x8000;  //TURN ON ADC
    AD1CON1SET = 0x0002;  //START SAMPLE

	//INTERRUPT
	IPC5SET = 0x05;			//SET INTERRUPT PRIORITY
	mT5ClearIntFlag(); 		//CLEAR INTERRUPT
	mT5IntEnable(1); 		//ENABLE INTERRUPT
	
	T4CONSET = 0x8000;     //START TIMER
    
	asm volatile("ei");

	srand(analogueRead()); 
}

void Cube::multiplex_cube(void){
	uint16_t bit = 0;
	COLOUR col;
	
	//DISABLE THE CUBE WHILE LOADING DATA ONTO LED DRIVERS
	DISABLE_CUBE;
	
	//MULTIPLEX
	if(++bam.bit_pos >= BAM_RESOLUTION){
		bam.delay = minimum_interrupt_delay;
		
		//SWITCH OFF CURRENT LAYER
		LATESET = LAYER[bam.layer];
		
		if(++bam.layer >= SIZE){
			if(++bam.animate_c > refresh_ratio){
				flags.animate_f = 1;
				bam.animate_c = 0;
			}
			bam.layer = 0;
		}
		
		//TURN ON NEXT LAYER
		LATECLR = LAYER[bam.layer];	
		bam.bit_pos = 0;
	}else{
		//DOUBLE NEXT BAM DELAY
		bam.delay <<= 1;
	}
	
	//LOAD THE NEXT BAM DELAY
	PR4 = bam.delay;
	
	//CALCULATE THE NEXT BIT VALUE
	bit = 1<<bam.bit_pos;
	
	//BLANK THE OUTPUT IOS
	LATDCLR = CLK|SDIR|SDIG|SDIB;
	
	//LOAD DATA ONTO LED DRIVERS
	for(uint8_t i = 0; i < SIZE2; i++){
		col = cubeArray[i + SIZE2*bam.layer];
		
		//CHECK RED VALUE
		if((col.r)&bit){
			LATDSET |= SDIR;
		}
		
		//CHECK GREEN VALUE
		if((col.g)&bit){
			LATDSET |= SDIG;
		}
		
		//CHECK BLUE VALUE
		if((col.b)&bit){
			LATDSET |= SDIB;
		}
		
		LATDSET = CLK;
		LATDCLR = CLK|SDIR|SDIG|SDIB;		
	}
	
	//LATCH THE DATA AND ENABLE THE CUBE
	LATCH;
	ENABLE_CUBE;
}

void Cube::manage_animations(void){
	uint8_t animationComplete = 0;

	switch (animationOrder[currentAnimation].a){
		case SINEWAVE:
			aPoint = &sineWave;
			break;
		case SNAKE:
			aPoint = &snake;
			break;
		case SPIRAL:
			aPoint = &spiral;
			break;
		case EXPLOSION:
			aPoint = &explosion;
			break;
		case ARROWS:
			aPoint = &arrows;
			break;
		case SPIN:
			aPoint = &spin;
			break;
		case WISPS:
			aPoint = &wisps;
			break;
		case RAINDROPS:
			aPoint = &raindrops;
			break;
		case ATOM:
			aPoint = &atom;
			break;
		case GAMEOFLIFE:
			aPoint = &gameOfLife;
			break;
		default:
			break;
	}

	animationComplete = aPoint->animate(cubeArray, fadeIn, fadeOut, buck);

	fadeIn = 0;

	if(!fadeOut){
		frameCount++;
	}

	if(++error_count >= animationOrder[currentAnimation].frame_rate*animationOrder[currentAnimation].time*2){
		animationComplete = 1;
		error_count = 0;
	}

	if(frameCount >= animationOrder[currentAnimation].frame_rate*animationOrder[currentAnimation].time){
		frameCount = 0;
		fadeOut = 1;
	}

	if(animationComplete){
		log.sec = error_count/animationOrder[currentAnimation].frame_rate;
		log.print_f = 1;
		log.number_played++;

		buck = aPoint->collect_buck();
		buck.lastAnimation = animationOrder[currentAnimation].a;
		if(++currentAnimation >= NUMBER_OF_ANIMATIONS){
			currentAnimation = 0;
		}

		log.anim = animationOrder[currentAnimation].a;

		refresh_ratio = REFRESH_RATE/animationOrder[currentAnimation].frame_rate;

		fadeIn = 1;
		fadeOut = 0;
		frameCount = 0;
		error_count = 0;
	}
}

MSG Cube::log_msg(void){
	MSG ret = log;

	log.print_f = 0;

	return ret;
}

uint16_t Cube::analogueRead(void){
	int res = 0;
    AD1CON1CLR = 0x0002;  //START CALCULATION
 
    while(!(AD1CON1 & 0x0001));  //WAIT TO CALC FINISH
 
    res = ADC1BUF0;  //GET RESULT
    AD1CON1SET = 0x0002;  //START SAMPLE
 
    return res;
}
