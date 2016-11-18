/*
	LED CUbe win32 Application
	Animations Header
	By Nicholas K Schulze
	www.HowNotToEngineer.com
	13/3/2011
*/

#include <windows.h>

#ifndef _ANIMATIONS_h_
#define _ANIMATIONS_h_

//set this to make sure that an availiable animation is selected through the window 
#define numOfAnimations 9

//structure to handle all animation inputs and memory
//Im not sure if this is how structures are ment to be used, but I
//just learned about them so wanted to use one
typedef struct{
	//used everywhere
	HWND hwnd;
	int startX, startY, startZ;
	int animation;
	int slowCount;
	int speed;
	int buttonPress;
	BOOL pressHandled;
	BOOL error;
	
	//wipe animations
	BOOL firstPass;
	int wipePos, wipeDir, wipeAxis;
	int planePos;
	
	//sine animations
	double phase;
	
	//snake animation
	int maxLength;
	int tailX, tailY, tailZ;
	int headX, headY, headZ;
	int snakeLength;
	int chopNum;
	char snakeDir;
	BOOL snakeStart, snakeAdd, tailChop, changeDir, snakeDie;

	//mouse animation
	int disappearAxis;

	//type animation
	char letter;
	char typeString[512];
	char typeStringPos;
	BOOL disappearDone;

	//rotate animation
	int rotatePos;

	//pulsing cube animation
	int cubeGrowSize;
	int cubeGrowDir;

	//rain animation
	int rainDrops;

	//random
	int randDir;
}animationData;

extern void defaultMem(void);
extern void animate(animationData AIN, BOOL interact);
extern void wipeFullPlane(void);
extern void loadPlane(void);
extern void loadArray(int array[8][8]);
extern void dummyToCube(void);
extern void loadBMP(void);
extern void wipeImage(void);
extern void dissapear(void);
extern void outerRotate(void);
extern void loop(void);
extern void sine(void);
extern void sideSine(void);
extern void rain(void);
extern void cubePulse(void);
extern void spiral(void);
extern void randomExpand(void);
extern void snake(void);
extern void mouse(void);
extern void type(char in);
extern void clockA(void);
extern char pull(void);
extern BOOL buttonHandled(void);
extern double map(double in, double inMin, double inMax, double outMin, double outMax);
extern void clear(void);
extern void clearDummy(void);
extern void genChars(void);
extern void sendChars(void);


#endif
