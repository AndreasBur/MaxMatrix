/*
	LED CUbe win32 Application
	Animations 
	By Nicholas K Schulze
	www.HowNotToEngineer.com
	12/3/2011
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "animations.h"
#include "serial.h"
#include "images.h"
#include "resource.h"

//constants
#define PI 3.14159265358979323846
#define size 8
#define centre 3.5

int cubeArray[8][8][8];
int dummyCube[8][8][8];
int ORArray[28][8];
char cubeChars[64];
animationData AMEM;

//SETS THE ANIMATION MEMORY STRUCTURE TO ITS DEFAULT VALUES
void defaultMem(void){
	AMEM.error = FALSE;
	AMEM.phase = 0;
	AMEM.wipePos = 0;
	AMEM.wipeDir = 1;
	AMEM.planePos = 0;
	AMEM.firstPass = TRUE;
	AMEM.snakeStart = TRUE;
	AMEM.buttonPress = 0;
	AMEM.pressHandled = FALSE;
	AMEM.snakeDie = FALSE;
	AMEM.disappearDone = TRUE;
	AMEM.typeStringPos = 0;
	AMEM.letter = 0x00;
	AMEM.rotatePos = 0;
	AMEM.cubeGrowDir = 2;
	AMEM.cubeGrowSize = 7;
	AMEM.randDir = 1;
}

//THIS IS RUN IN THE ANIMATION THREAD
void animate(animationData AIN, BOOL interact){
	//set the structure data that is used in other functions
	AMEM.speed = AIN.speed;
	AMEM.hwnd = AIN.hwnd;

	//call animation
	if(!AMEM.error){
		if(!interact){
			//ADD MORE ANIMATIONS HERE
			//make sure you change the numOfAnimations in animations.h
			switch(AIN.animation){
				case 0: //wipe X
					AMEM.wipeAxis = 0;
					wipeFullPlane();
					break;
				case 1: //wipe Y
					AMEM.wipeAxis = 1;
					wipeFullPlane();
					break;
				case 2: //wipe Z
					AMEM.wipeAxis = 2;
					wipeFullPlane();
					break;
				case 3: 
					sine(); 
					break;
				case 4:
					sideSine();
					break;
				case 5:
					cubePulse(); 
					break;
				case 6:
					rain();
					break;
				case 7:
					spiral();
					break;
				case 8:
					randomExpand();
					break;
				default:
					clear();
					clearDummy();
					break;
			}
		}else{
			//reset the the button handled flag
			if(AMEM.pressHandled && AIN.buttonPress == 0){
				AMEM.pressHandled = FALSE;
			}
			AMEM.buttonPress = AIN.buttonPress;
		
			switch(AIN.animation){
				case 1:
					snake();
					break;
				case 2:
					AMEM.disappearAxis = 1;
					mouse();
					break;
				case 3:
					AMEM.disappearAxis = 0;
					type(AIN.letter);
					break;
				case 4:
					clockA();
					break;
				default:
					clear();
					clearDummy();
					defaultMem();
					break;
			}
		}
		
		//increment the slow counter 
		//generate and send the characters
		//hold the image
		AMEM.slowCount++;
		genChars();
		sendChars();
		Sleep(5);
	}
}
//////////////
//ANIMATIONS//
//////////////

//USE wipeImage and loadPlane TO CREATE FULL PLANE WIPE ANIMATION IN DIRECTION OF wipeAxis
void wipeFullPlane(void){
	if(AMEM.firstPass){
		//if this is the initial call load the full plane
		loadPlane();
		AMEM.wipePos = AMEM.planePos;
		AMEM.firstPass = FALSE;
	}
	if(AMEM.slowCount > 10 - AMEM.speed){
		//animate
		wipeImage();
		AMEM.slowCount = 0;
	}
}

//FILLS A PLANE AT planePos IN DIRECTION OF wipeAxis
void loadPlane(void){
	//fills and entire plane at the the chosen location along the chosen axis
	int x, y, z;
	for(x = 0; x < size; x++){
		for(y = 0; y < size; y++){
			for(z = 0; z < size; z++){
				cubeArray[AMEM.wipeAxis==0?AMEM.planePos:x][AMEM.wipeAxis==1?AMEM.planePos:y][AMEM.wipeAxis==2?AMEM.planePos:z] = 1;
			}
		}
	}
}

//LOADS AN 8x8 ARRAY ONTO THE DUMMY CUBE
void loadArray(int *inArray){
    int i, j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            dummyCube[size - 1 - j][size-1][size - 1 -i] = *inArray++;
        }
    }
}

//LOADS BITMAP FILE TO A LAYER
//NOT FINISHED CURRENTLY UNDER DEVELOPMENT!!
void loadBMP(void){
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
	BYTE *aBitmapBits, *pixelData;
	FILE *in;
	DWORD bSize; 
	LONG byteWidth, padWidth;
	LONG width, height;
	DWORD bpp, diff;
	int offset, j, i;
	
	in = fopen("image_small_black.bmp","rb");
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, in);
	fread(&bmih, sizeof(BITMAPINFOHEADER), 1, in);
	
	bSize = bmfh.bfSize - bmfh.bfOffBits;
	width = bmih.biWidth;
	height = bmih.biHeight;
	bpp = bmih.biBitCount;
	
	byteWidth = (LONG)((float)width*(float)bpp/8.0);
	padWidth = byteWidth;
	
	while(padWidth%4 != 0){
		padWidth++;
	}
	
	aBitmapBits = (BYTE*)malloc(bSize);
	fread(aBitmapBits, sizeof(BYTE), bSize, in);
	
	diff = height*byteWidth;
	pixelData = (BYTE*)malloc(diff);
	
	if(height > 0){
		j = bSize - 3;
		offset = padWidth - byteWidth;
		for(i = 0; i < size; i+=3){
			if((i+1)%padWidth == 0){
				i += offset;
			}
			*(pixelData + j + 2) = *(aBitmapBits + i);
			*(pixelData + j + 1) = *(aBitmapBits + i + 1);
			*(pixelData + j) = *(aBitmapBits + i + 2);
			j++;
		}
	}else{
		height = height *= -1;
		offset = 0;
		do{
			memcpy(pixelData+(offset*byteWidth), aBitmapBits+(offset*padWidth), byteWidth);
			offset++;
		}while(offset < height);
	}
	
	fclose(in);
	free(aBitmapBits);
	free(pixelData);
}

//WIPES AN IMAGE BETWEEN 0 AND SIZE IN DIRECTION OF wipeAxis
void wipeImage(void){
	int x, y, z;
	switch(AMEM.wipeAxis){
		case 0: //x axis
			for(y = 0; y < size; y++){
				for(z = 0; z < size; z++){
					//flip direction at ends
					if(AMEM.wipePos+AMEM.wipeDir >= size){
						AMEM.wipeDir*=-1;
					}else if(AMEM.wipePos+AMEM.wipeDir < 0 ){
						AMEM.wipeDir *= -1;
					}
					//shift image
					cubeArray[AMEM.wipePos+AMEM.wipeDir][y][z] = cubeArray[AMEM.wipePos][y][z];
					cubeArray[AMEM.wipePos][y][z] = 0;
				}
			}
			//increment next position
			AMEM.wipePos+=AMEM.wipeDir;
			break;
		case 1: //y axis
			for(x = 0; x < size; x++){
				for(z = 0; z < size; z++){
					if(AMEM.wipePos+AMEM.wipeDir >= size){
						AMEM.wipeDir*=-1;
					}else if(AMEM.wipePos+AMEM.wipeDir < 0 ){
						AMEM.wipeDir *= -1;
					}
					cubeArray[x][AMEM.wipePos+AMEM.wipeDir][z] = cubeArray[x][AMEM.wipePos][z];
					cubeArray[x][AMEM.wipePos][z] = 0;
				}
			}
			AMEM.wipePos+=AMEM.wipeDir;
			break;
		case 2: //z axis
			for(x = 0; x < size; x++){
				for(y = 0; y < size; y++){
					if(AMEM.wipePos+AMEM.wipeDir >= size){
						AMEM.wipeDir*=-1;
					}else if(AMEM.wipePos+AMEM.wipeDir < 0 ){
						AMEM.wipeDir *= -1;
					}
					cubeArray[x][y][AMEM.wipePos+AMEM.wipeDir] = cubeArray[x][y][AMEM.wipePos];
					cubeArray[x][y][AMEM.wipePos] = 0;
				}
			}
			AMEM.wipePos+=AMEM.wipeDir;
			break;
	}
}

//disappear FUNCTION USED IN THE type FUNCTION
void disappear(void){
	int i, j, k;

	//shift cube contents along disappear axis
	for(i = 0; i < size-1; i++){
		for(j = 0; j < size; j++){
			for(k = 0; k < size; k++){
				switch(AMEM.disappearAxis){
					case 0:
						// check to see if plane has moved the entire cube and set dissapearDone
						// the 99 is for SPACE characters, these will not display but are required to distinquish between zeros
						if((cubeArray[j][i][k] == 1 || cubeArray[j][i][k] == 9) && i == 0){
							AMEM.disappearDone = TRUE;
						}
						cubeArray[j][i][k] = cubeArray[j][i+1][k];
						break;
					case 1:
						if((cubeArray[i][j][k] == 1 || cubeArray[i][j][k] == 9) && i == 0){
							AMEM.disappearDone = TRUE;
						}
						cubeArray[i][j][k] = cubeArray[i+1][j][k];
						break;
					case 2:
						if((cubeArray[j][k][i] == 1 || cubeArray[j][k][i] == 9) && i == 0){
							AMEM.disappearDone = TRUE;
						}
						cubeArray[j][k][i] = cubeArray[j][k][i+1];
						break;
				}
			}
		}
	}
	
	//load new image from dummy to cubeArray
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			switch(AMEM.disappearAxis){
				case 0:
					cubeArray[i][7][j] = dummyCube[i][7][j];
					break;
				case 1:
					cubeArray[7][i][j] = dummyCube[7][i][j];
					break;
				case 2:
					cubeArray[j][i][7] = dummyCube[j][i][7];
					break;
			}
			
		}
	}
	clearDummy();
}

//I was quite happy with these next two functions

//outerRotate and loop allow you to rotate an animated image about the outside fo the cube
//ORArray will wrap around the cube using outerRotate and be rotates using loop, any animations placed
//in ORArray will be animated and rotated.
//make sure you load the animation into the same position on the ORArray at each itteration, loop handles the rotation

//ROTATE data in ORArray AROUND OUTER PLANES
void outerRotate(void){
	int r, z;
	
	loop();

	//wrap the ORArray arrpound the outside of the cube
	for(r = 0; r < size*4-4; r++){
		for(z = 0; z < size; z++){
			if(r >= 0 && r < size){
				cubeArray[size - 1 - r][7][z] = ORArray[r][z];
			}else if(r >= size && r < size*2-1){
				cubeArray[0][size - (r-size) - 2][z] = ORArray[r][z];
			}else if(r >= size*2-1 && r < size*3-2){
				cubeArray[r-(2*size-2)][0][z] = ORArray[r][z];
			}else if(r >= size*3-2 && r < size*4-3){
				cubeArray[7][r-(3*size-3)][z] = ORArray[r][z];
			}
		}
	}
}

//Loop ORArray used for rotate function
void loop(void){
	int n, r, z;
	int temp[8];

	//shift and loop the ORArray until the correct position is reached
	for(n = 0; n < AMEM.rotatePos; n++){
		//shift first column into a temp array
		for(z = 0; z < size; z++){
			temp[z] = ORArray[0][z];
		}

		//shift all columns down
		for(r = 0; r < size*4-5; r++){
			for(z = 0; z < size; z++){
				ORArray[r][z] = ORArray[r+1][z];
			}
		}

		//move data from first column into last
		for(z = 0; z < size; z++){
			ORArray[size*4-5][z] = temp[z];
		}
	}

	//increment/reset rotatePos, this is what gives the rotation effect
	AMEM.rotatePos++;
	if(AMEM.rotatePos > 27){
		AMEM.rotatePos = 0;
	}
}

//SINE FUNCTION
void sine(void){
	int x, y;
	double Z;
	
	clear();
	
	//basic sine function
	for(x = 0; x < size; x++){
		for(y = 0; y < size; y++){
			Z = sin(AMEM.phase + sqrt(pow(map(x,0,size-1,-PI,PI),2) + pow(map(y,0,size-1,-PI,PI),2)));
            Z = map(Z,-1,0.95,0,size - 1); 	
			cubeArray[x][y][(int)Z] = 1;
		}
	}

	//handle the speen and increment phase
	AMEM.phase += map(AMEM.speed, 1, 10, PI/64, PI/18);
}

//SINE PLANE
void sideSine(void){
	int x, y;
	double Z = 0;

    clear();
    for(x = 0; x < size; x++){
       for(y = 0; y < size; y++){
            Z = sin(AMEM.phase + sqrt(pow(map(x,-8,size-1,-PI,PI),2) + pow(map(y,-8,size-1,-PI,PI),2)));
            Z = map(Z,-0.9,0.9,0,7);
            cubeArray[x][y][(int)Z] = 1;
        }
    }

    AMEM.phase += map(AMEM.speed, 1, 10, PI/32, PI/12);
    genChars();
    sendChars();
    Sleep(5);

}

//RANDOM RAIN EFFECT
void rain(void){
	int rX, rY, x, y, z;
	int count = 0;
	AMEM.disappearAxis = 2;

	 if(AMEM.rainDrops > rand()%4+0){
        rX = rand()%8+0;
        rY = rand()%8+0;
        while(cubeArray[rX][rY][7] != 0 && count < 100){
            rX = rand()%8+0;
            rY = rand()%8+0;
            count++;
        }
        cubeArray[rX][rY][7] = 1;
        AMEM.rainDrops = 0;
    }

	if(AMEM.slowCount > 6){
        for(x = 0; x < size; x++){
            for(y = 0; y < size; y++){
                for(z = 0; z < size-1; z++){
                    cubeArray[x][y][z] = cubeArray[x][y][z+1];
                    if(cubeArray[x][y][z] == 1){
                        cubeArray[x][y][7] = 5;
                    }
                }
            }
        }
        AMEM.slowCount = 0;
    }

	AMEM.slowCount++;
    AMEM.rainDrops++;
}

//PULSING CUBE
void cubePulse(void){
	int x, y, z;

	if(AMEM.slowCount > (int)map(AMEM.speed, 0, 10, 8, 2)){
		clear();

		//draw the 12 edges of a cube determained by cubeGrowSize
		for(x = 0; x < size; x++){
			for(y = 0; y < size; y++){
				for(z = 0; z < size; z++){
					//corners
					if(abs(2*(x-centre)) == AMEM.cubeGrowSize && abs(2*(y-centre)) == AMEM.cubeGrowSize && abs(2*(z-centre)) == AMEM.cubeGrowSize){
						cubeArray[x][y][z] = 1;
					}
					//sides
					if(abs(2*(x-centre)) == AMEM.cubeGrowSize && abs(2*(y-centre)) == AMEM.cubeGrowSize && abs(2*(z-centre)) < AMEM.cubeGrowSize){
						cubeArray[x][y][z] = 1;
					}
					if(abs(2*(x-centre)) == AMEM.cubeGrowSize && abs(2*(z-centre)) == AMEM.cubeGrowSize && abs(2*(y-centre)) < AMEM.cubeGrowSize){
						cubeArray[x][y][z] = 1;
					}
					if(abs(2*(z-centre)) == AMEM.cubeGrowSize && abs(2*(y-centre)) == AMEM.cubeGrowSize && abs(2*(x-centre)) < AMEM.cubeGrowSize){
						cubeArray[x][y][z] = 1;
					}
				}
			}
		}


		//increment and reset 
		AMEM.cubeGrowSize += AMEM.cubeGrowDir;

		if(AMEM.cubeGrowSize > 7){
			AMEM.cubeGrowSize = 5;
			AMEM.cubeGrowDir *= -1;
		}else if(AMEM.cubeGrowSize < 0){
			AMEM.cubeGrowSize = 3;
			AMEM.cubeGrowDir *= -1;
		}

		AMEM.slowCount = 0;
	}
}

//HELIX/SPIRAL
void spiral(void){
	int z, i;
	double X = 0;
	double Y = 0;
	clear();

	for(z = 0; z < size; z++){
		//the 3 itterations make the spiral thicker which looks better
		for(i = 0; i < 3; i++){
			Y = cos(AMEM.phase + i*PI/8 + map(z,0,size-1,0,2*PI));
			X = sin(AMEM.phase + i*PI/8 + map(z,0,size-1,0,2*PI));
			Y = map(Y,-1.1,0.9,0,size-1);
			X = map(X,-1.1,0.9,0,size-1);
			cubeArray[(int)X][(int)Y][z] = 1;
		}
	}

	AMEM.phase += map(AMEM.speed, 1, 10, PI/22, PI/8);
}

//FILL AND EMPTY CUBE RANDOM LEDS AT A TIME
void randomExpand(void){
    int count = 0;
	int rX, rY, rZ;

	srand(time(NULL));
    rX = rand()%8+0;
    rY = rand()%8+0;
    rZ = rand()%8+0;
	
	//find random off LED
    while(cubeArray[rX][rY][rZ] == 1 && AMEM.randDir == 1){
        rX = rand()%8+0;
        rY = rand()%8+0;
        rZ = rand()%8+0;
        count++;
		
		//if its taking to long to find a off LED its probably all on, start turning off
        if(count > 10000){
            AMEM.randDir *= -1;
        }
    }
    count = 0;

	//find random on LED
    while(cubeArray[rX][rY][rZ] == 0 && AMEM.randDir == -1){
        rX = rand()%8+0;
        rY = rand()%8+0;
        rZ = rand()%8+0;
        count++;

        if(count > 10000){
            AMEM.randDir *= -1;
        }
    }

	cubeArray[rX][rY][rZ] = AMEM.randDir==1?1:0;
	Sleep(5);
}

//////////////////////////
//INTERACTIVE ANIMATIONS//
//////////////////////////

//3D interactive snake game
//the snake puts an incrementally increaseing int in the array every move in the direction of travel, 
//this means the tail is always the lowest number in the cube and the head is always the largest number
void snake(void){
	int x, y, z;

	//first call or restart snake game
	if(AMEM.snakeStart){
		clear();
		clearDummy();
		AMEM.maxLength = 30;
		AMEM.headX = 0;
		AMEM.headY = 7;
		AMEM.headZ = 0;
		AMEM.tailX = AMEM.headX;
		AMEM.tailY = AMEM.headY;
		AMEM.tailZ = AMEM.headZ;
		AMEM.snakeLength = 1;
		AMEM.chopNum = 1;
		AMEM.snakeDir = 'u';
		AMEM.snakeAdd = TRUE;
		AMEM.changeDir = FALSE;
		AMEM.tailChop = FALSE;
		AMEM.snakeDie = FALSE;
		AMEM.snakeStart = FALSE;
	}

	if(AMEM.slowCount > 15 - AMEM.speed || AMEM.buttonPress != 0){
		//if snake died on last restart snake game
		if(AMEM.snakeDie){
			AMEM.snakeStart = TRUE;
			AMEM.snakeAdd = FALSE;
		}
		
		//add head
		if(AMEM.snakeAdd){
            dummyCube[AMEM.headX][AMEM.headY][AMEM.headZ] = AMEM.snakeLength;
            AMEM.snakeAdd = FALSE;
        }
		
        //chop tail
        if(AMEM.snakeLength > AMEM.maxLength && AMEM.tailChop){
            dummyCube[AMEM.tailX][AMEM.tailY][AMEM.tailZ] = 0;

            if(dummyCube[AMEM.tailX-1][AMEM.tailY][AMEM.tailZ] == AMEM.chopNum && AMEM.tailX-1 != -1){
                AMEM.tailX--;
            }else if(dummyCube[AMEM.tailX+1][AMEM.tailY][AMEM.tailZ] == AMEM.chopNum && AMEM.tailX+1 != size){
                AMEM.tailX++;
            }else if(dummyCube[AMEM.tailX][AMEM.tailY-1][AMEM.tailZ] == AMEM.chopNum && AMEM.tailY-1 != -1){
                AMEM.tailY--;
            }else if(dummyCube[AMEM.tailX][AMEM.tailY+1][AMEM.tailZ] == AMEM.chopNum && AMEM.tailY+1 != size){
                AMEM.tailY++;
            }else if(dummyCube[AMEM.tailX][AMEM.tailY][AMEM.tailZ-1] == AMEM.chopNum && AMEM.tailZ-1 != -1){
                AMEM.tailZ--;
            }else if(dummyCube[AMEM.tailX][AMEM.tailY][AMEM.tailZ+1] == AMEM.chopNum && AMEM.tailZ+1 != size){
                AMEM.tailZ++;
            }
            AMEM.chopNum++;
            AMEM.tailChop = FALSE;
        }
		
		//change direction
		switch(AMEM.buttonPress){
			case 1: //up
				//ignore a 180deg turn button press
				AMEM.snakeDir = AMEM.snakeDir!='d'?'u':'d';
				AMEM.pressHandled = TRUE;
				break;
			case 2: //down
				AMEM.snakeDir = AMEM.snakeDir!='u'?'d':'u';
				AMEM.pressHandled = TRUE;
				break;
			case 3: //east
				AMEM.snakeDir = AMEM.snakeDir!='w'?'e':'w';
				AMEM.pressHandled = TRUE;
				break;
			case 4: //west
				AMEM.snakeDir = AMEM.snakeDir!='e'?'w':'e';
				AMEM.pressHandled = TRUE;
				break;
			case 5: //north
				AMEM.snakeDir = AMEM.snakeDir!='s'?'n':'s';
				AMEM.pressHandled = TRUE;
				break;
			case 6: //south
				AMEM.snakeDir = AMEM.snakeDir!='n'?'s':'n';
				AMEM.pressHandled = TRUE;
				break;
		}

		//move current direction
		switch(AMEM.snakeDir){
			case 'u':
				AMEM.headZ++;
				AMEM.snakeLength++;
				AMEM.snakeAdd = TRUE;
				AMEM.tailChop = TRUE;
				break;
			case 'd':
				AMEM.headZ--;
				AMEM.snakeLength++;
				AMEM.snakeAdd = TRUE;
				AMEM.tailChop = TRUE;
				break;
			case 'n':
				AMEM.headY--;
				AMEM.snakeLength++;
				AMEM.snakeAdd = TRUE;
				AMEM.tailChop = TRUE;
				break;
			case 's':
				AMEM.headY++;
				AMEM.snakeLength++;
				AMEM.snakeAdd = TRUE;
				AMEM.tailChop = TRUE;
				break;
			case 'e':
				AMEM.headX--;
				AMEM.snakeLength++;
				AMEM.snakeAdd = TRUE;
				AMEM.tailChop = TRUE;
				break;
			case 'w':
				AMEM.headX++;
				AMEM.snakeLength++;
				AMEM.snakeAdd = TRUE;
				AMEM.tailChop = TRUE;
				break;
		}
		
		if(AMEM.headX > size-1 || AMEM.headX < 0){
			AMEM.snakeDie = TRUE;
		}else if(AMEM.headY > size-1 || AMEM.headY < 0){
			AMEM.snakeDie = TRUE;
		}else if(AMEM.headZ > size-1 || AMEM.headZ < 0){
			AMEM.snakeDie = TRUE;
		}else if(dummyCube[AMEM.headX][AMEM.headY][AMEM.headZ] != 0){
			AMEM.snakeDie = TRUE;
		}
		
		AMEM.slowCount = 0;
	}
	
    if(AMEM.snakeAdd){
        clear();
        for(x = 0; x < size; x++){
            for(y = 0; y < size; y++){
                for(z = 0; z < size; z++){
                    if(dummyCube[x][y][z] != 0){
                        cubeArray[x][y][z] = 1;
                    }
                }
            }
        }
    }
}

//VISUALISE MOUSE MOVEMENT
void mouse(void){
	POINT point;
	int I, J;
	int captureWidth = 1919;
	int captureHeight = 1079;
	
	GetCursorPos(&point);
	
	I = (int)point.x;
	J = (int)point.y;
	
	clearDummy();
	
	if(I >= 0 && I <= captureWidth  && J >= 0 && J <= captureHeight){
		I = map(I, 0, captureWidth , 0, size-1);
		J = map(J, 0, captureHeight , 0, size-1);
		switch(AMEM.disappearAxis){
			case 0:
				dummyCube[size - 1 - I][7][size - 1 - J] = 1;
				cubeArray[size - 1 - I][7][size - 1 - J] = 1;
				break;
			case 1:
				dummyCube[7][size - 1 - I][size - 1 - J] = 1;
				cubeArray[7][size - 1 - I][size - 1 - J] = 1;
				break;
			case 2:
				dummyCube[size - 1 - I][J][7] = 1;
				cubeArray[size - 1 - I][J][7] = 1;
				break;
		}
	}
	
	if(AMEM.slowCount > 2){
		disappear();
		AMEM.slowCount = 0;
	}
}	

//CAPTURE AND DISPLAY TEXT FROM KEYBOARD
void type(char in){
	//convert to upper case
	if(isalpha(in)){
		in -= 0x20;
	}

	if(isprint(in)){
		if(AMEM.disappearDone && AMEM.typeStringPos == 0){
			loadArray(IMG(in));
			AMEM.slowCount = 0;
			AMEM.disappearDone = FALSE;
			AMEM.pressHandled = TRUE;
		}else{
			AMEM.typeString[AMEM.typeStringPos] = in;
			AMEM.typeStringPos++;
			AMEM.disappearDone = FALSE;
			AMEM.pressHandled = TRUE;
		}
	}

	if(AMEM.disappearDone && AMEM.typeStringPos > 0){
		loadArray(IMG(pull()));
		AMEM.disappearDone = FALSE;
		AMEM.pressHandled = TRUE;
	}


	if(AMEM.slowCount > map(AMEM.speed, 0, 10, 5, 1)){
		disappear();
		AMEM.slowCount = 0;
	}
}

//ROTATING DIGITAL CLOCK
void clockA(void){
	int z, r, i, x, y;
	int offset = 0;
	int skip = 0;
	int *tempArP;
	int tempAr[8][8];
	
	//get the sytem time and date
	time_t rawtime = time(NULL);
	char *timeDate = ctime(&rawtime);
	char time[] = "xx:xx:xx";
	char hour[] = "xx";
	int hourD;
	
	//extract time and convert it to 12 hour time
	memcpy(time, timeDate+11, 8);
	memcpy(hour, time, 2);
	hourD = atoi(hour);

	if(hourD > 12){
		hourD -= 12;
		itoa(hourD, hour, 10);
		if(hourD >= 10){
			memcpy(time, hour, 2);
		}else{
			memcpy(time+1, hour, 1);
			memset(time, '0', 1);
		}
	}

	//load time into ORArray
	if(AMEM.slowCount > 10 - AMEM.speed){
		memset(ORArray, 0, sizeof(ORArray));

		for(i = 0; i < 5; i++){
			if(!(i == 0 && time[i] == '0')){
				tempArP = IMG(time[i]);
				for(x = 0; x < size; x++){
					for(y = 0; y < size; y++){
						tempAr[y][size - 1 - x] = *tempArP++;
					}
				}

				if(time[i] == '1' || time[i] == ':'){
					skip = 4;
				}else{
					skip = 2;
				}

				for(z = 0; z < size; z++){
					for(r = 0; r < size-skip; r++){
						ORArray[r+offset][z] = tempAr[r+skip][z];
					}
				}

				offset += size - skip - 2;
			}
		}

		//loop ORArray and load onto cubeArray
		outerRotate();
		AMEM.slowCount = 0;
	}
}

//////////////////
//HELPER METHODS//
//////////////////

//pull a value from the start of typeString and shift all other values down
char pull(void){
	int i;
	char ret = AMEM.typeString[0];

	for(i = 0; i < sizeof(AMEM.typeString) - 1; i++){
		AMEM.typeString[i] = AMEM.typeString[i+1];
	}

	AMEM.typeString[sizeof(AMEM.typeString) - 1] = 0x00;

	AMEM.typeStringPos--;
	return ret;
}

//LETS WIN APP KNOW WHEN ANIMATION HAS FINISHED HANDELING A BUTTON PRESS
BOOL buttonHandled(void){
	return AMEM.pressHandled;
}

//MAP RANGE OF VALUES
double map(double in, double inMin, double inMax, double outMin, double outMax){
    double out;
    out = (in-inMin)/(inMax-inMin)*(outMax-outMin) + outMin;
    return out;
}

//TRANSFER FROM DUMMY TO CUBE
void dummyToCube(void){
	memcpy(cubeArray, dummyCube, sizeof(dummyCube));
}

//CLEAR cubeArray
void clear(void){
    memset(cubeArray, 0, sizeof(cubeArray));
}

//CLEAR cummyCube
void clearDummy(void){
    memset(dummyCube, 0, sizeof(dummyCube));
}

//GENERATE CHARACTERS FOR SENDING
void genChars(void){
    int i = 0;
	int x, y, z;
    memset(cubeChars, 0x00, sizeof(cubeChars));
    for(z = 0; z < size; z++){
        for(y = 0; y < size; y++){
            for(x = 0; x < size; x++){
                if(cubeArray[x][y][z] == 1){
                    cubeArray[x][y][z] = 1;
                    cubeChars[i] |= 0x01<<((size-1)-x);
                }
            }
            i++;
        }
    }
}

//SEND GENERATED CHARACTERS
void sendChars(void){
	int i;
    for(i = 0; i < 64; i++) {
        serialWrite(cubeChars[i]);
    }
	serialWrite('e');
	serialWrite('n');
	serialWrite('d');

}
