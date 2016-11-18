#include "cube.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define PI 3.14159265358979323846
#define size 8
#define centre 3.5
#define g -9.81

//CONSTRUCTOR
cube::cube(void){
    clearAnimationVariables();
    S1.begin("COM3", 256000);
    I.setup();
}

//////////////////
//* ANIMATIONS *//
//////////////////

//linked together animations for video
void cube::flow_animation(void){
    switch(flow_count){
        case 1:
            //while(!spell("HOW NOT TO ENGINEER"));
            //clear();
            //hold(20);
            break;
        case 2: // SINGLE ARROW
            for(int i = 0; i < 232; i++){
                arrow(1,0,0,0);
            }
            break;
        case 3: // DOUBLE ARROW
            for(int i = 0; i < 225; i++){
                arrow(1,1,0,0);
            }
            break;
        case 4: // TRIPLE ARROW
            for(int i = 0; i < 256; i++){
                arrow(1,1,1,0);
            }
            break;
        case 5: // CLEAR ARROWS
            for(int i = 0; i < 110; i++){
                arrow(1,1,1,1);
            }
            break;
        case 6: // RANDOM FADE TO SPIN
            fade(0,1);
            hold(1);
            while(!spin(1,'z'));
            hold(1);
            break;
        case 7: // Y AXIS WIPE
            while(!wipe('y'));
            hold(1);
            break;
        case 8: // SPIN TO X AXIS WIPE
            while(!spin(2,'z'));
            hold(1);
            while(!wipe('x'));
            hold(1);
            break;
        case 9: // SPIN TO Z AXIS WIPE
            while(!spin(3,'x'));
            hold(1);
            while(!wipe('z'));
            hold(1);
            break;
        case 10: // RAMP UP TO SINE WAVE
            for(int i = 0; i < 100; i++){
                sine(map(i, 0, 100, 1, 7.2), 0);
            }
            for(int i = 0; i < 500; i++){
                sine(7, 0);
            }
            break;
        case 11: // DROP SINE WAVE TO TOP PANE
            fade(1,1);
            hold(5);
            break;
        case 12: // TOP PANE TO PULSING CUBE
            fade(2,1);
            hold(10);
            while(grow() < 6);
            break;
        case 13: //PULSING CUBE TO NOTHING
            fade(3,1);
            hold(5);
            break;
        case 14: //RAIN
            for(int i = 0; i < 700; i++){
                rain(0);
            }
            for(int i = 0; i < 50; i++){
                rain(1);
            }
            break;
        case 15: //SPIRAL
            for(int i = 0; i < 500; i++){
                spiral(0,0);
            }
            for(int i = 0; i < 250; i++){
                spiral(0,1);
            }
            hold(10);
            break;
        case 16:
            for(int i = 0; i < 2500; i++){
                snake(35, 8);
            }
            clear();
            hold(10);
            break;
        case 17:
            phase1 = 0;
            fade(0,2);
            fade(4,1);
            break;
        case 18:
            for(int i = 0; i < 900; i++){
                side_sine();
            }
            break;
        default:
            clear();
            hold(100);
            clearAnimationVariables();
            break;
    }
    flow_count++;
}

//SINGLE LED ON
void cube::ledON(int x, int y, int z){
    cubeArray[x][y][z] = 1;;
    genChars();
    sendChars();
    Sleep(5);
}

//load a 2x2 array onto front face of cube
void cube::loadArray(int array[8][8]){
    //clear();
    for(int z = 0; z < size; z++){
        for(int x = 0; x < size; x++){
            cubeArray[7-x][size-1][z] = array[7-z][x];
        }
    }
}

//BMP image to a 8x8 array
void cube::BMPtoARRAY(void){
}

//ARROW
void cube::arrow(int one, int two, int three, int cancel){
    if(slow_count > 3){
        //draw arrow rotationg about z
        if(one){
            int x1 = 0;
            int y1 = 0;
            switch(pass1){
                case 1:
                    cubeArray[x1][y1][3] = 1;
                    cubeArray[x1][y1][4] = 1;
                    break;
                case 2:
                    cubeArray[x1][y1][2] = 1;
                    cubeArray[x1][y1][5] = 1;
                    break;
                case 3:
                    cubeArray[x1][y1][1] = 1;
                    cubeArray[x1][y1][6] = 1;
                    break;
                case 4:
                    cubeArray[x1][y1][1] = 1;
                    cubeArray[x1][y1][6] = 1;
                    cubeArray[x1][y1][2] = 1;
                    cubeArray[x1][y1][5] = 1;
                    break;
                case 5:
                    cubeArray[x1][y1][2] = 1;
                    cubeArray[x1][y1][5] = 1;
                    break;
                case 6:
                    cubeArray[x1][y1][2] = 1;
                    cubeArray[x1][y1][5] = 1;
                    break;
                case 7:
                    cubeArray[x1][y1][2] = 1;
                    cubeArray[x1][y1][5] = 1;
                    break;
                case 8:
                    cubeArray[x1][y1][2] = 1;
                    cubeArray[x1][y1][3] = 1;
                    cubeArray[x1][y1][4] = 1;
                    cubeArray[x1][y1][5] = 1;
                    break;
            }
            pass1++;
        }

        //draw arrow rotating about x
        if(two){
            int y2 = 7;
            int z2 = 7;
            switch(pass2){
                case 1:
                    cubeArray[3][y2][z2] = 3;
                    cubeArray[4][y2][z2] = 3;
                    break;
                case 2:
                    cubeArray[2][y2][z2] = 3;
                    cubeArray[5][y2][z2] = 3;
                    break;
                case 3:
                    cubeArray[1][y2][z2] = 3;
                    cubeArray[6][y2][z2] = 3;
                    break;
                case 4:
                    cubeArray[1][y2][z2] = 3;
                    cubeArray[6][y2][z2] = 3;
                    cubeArray[2][y2][z2] = 3;
                    cubeArray[5][y2][z2] = 3;
                    break;
                case 5:
                    cubeArray[2][y2][z2] = 3;
                    cubeArray[5][y2][z2] = 3;
                    break;
                case 6:
                    cubeArray[2][y2][z2] = 3;
                    cubeArray[5][y2][z2] = 3;
                    break;
                case 7:
                    cubeArray[2][y2][z2] = 3;
                    cubeArray[5][y2][z2] = 3;
                    break;
                case 8:
                    cubeArray[2][y2][z2] = 3;
                    cubeArray[3][y2][z2] = 3;
                    cubeArray[4][y2][z2] = 3;
                    cubeArray[5][y2][z2] = 3;
                    break;
            }
            pass2++;
        }

        //draw arrow rotating about y
        if(three){
            int x3 = 7;
            int z3 = 3;
            switch(pass3){
                case 1:
                    cubeArray[x3][3][z3] = 4;
                    cubeArray[x3][4][z3] = 4;
                    break;
                case 2:
                    cubeArray[x3][2][z3] = 4;
                    cubeArray[x3][5][z3] = 4;
                    break;
                case 3:
                    cubeArray[x3][1][z3] = 4;
                    cubeArray[x3][6][z3] = 4;
                    break;
                case 4:
                    cubeArray[x3][1][z3] = 4;
                    cubeArray[x3][6][z3] = 4;
                    cubeArray[x3][2][z3] = 4;
                    cubeArray[x3][5][z3] = 4;
                    break;
                case 5:
                    cubeArray[x3][2][z3] = 4;
                    cubeArray[x3][5][z3] = 4;
                    break;
                case 6:
                    cubeArray[x3][2][z3] = 4;
                    cubeArray[x3][5][z3] = 4;
                    break;
                case 7:
                    cubeArray[x3][2][z3] = 4;
                    cubeArray[x3][5][z3] = 4;
                    break;
                case 8:
                    cubeArray[x3][2][z3] = 4;
                    cubeArray[x3][5][z3] = 4;
                    cubeArray[x3][3][z3] = 4;
                    cubeArray[x3][4][z3] = 4;
                    break;
            }
            pass3++;
        }

        //remove arrows
        if(cancel){
            for(int i = 0; i < size; i++){
                if(cubeArray[0][0][i] == 1){
                    cubeArray[0][0][i] = 0;
                }
                if(cubeArray[i][0][0] == 3){
                    cubeArray[i][0][0] = 0;
                }
                if(cubeArray[0][i][0] == 4){
                    cubeArray[0][i][0] = 0;
                }
            }
        }
        rotate();
        slow_count = 0;
    }
    slow_count++;
    genChars();
    sendChars();
    Sleep(5);
}

//ROTATE FRAME
//1's rotated about z axis
//3's rotated about x axis
//4's rotated about y axis
void cube::rotate(void){
    int j = 0;
    //reset dummy cube array to zero
    memset(dummyCube, 0, sizeof(dummyCube));

    while(skipX[j] != -1){
        cubeArray[skipX[j]][skipY[j]][skipZ[j]] = skipR[j];
        j++;
    }

    j = 0;

    for(int i = 0; i < 64; i++){
        skipX[i] = -1;
        skipY[i] = -1;
        skipZ[i] = -1;
        skipR[i] = 0;
    }
    //perform rotation about chosen axis
    for(int x = 0; x < size; x++){
        for(int y = 0; y < size; y++){
            for(int z = 0; z < size; z++){
                //Z ROTATION
                if(cubeArray[x][y][z] == 1){
                    if(x == 0 && y != 7){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y+1][z] == 0 || cubeArray[x][y+1][z] == 1){
                            dummyCube[x][y+1][z] = 1;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y+1;
                            skipZ[j] = z;
                            skipR[j] = 1;
                            j++;
                        }
                    }else if(y == 7 && x != 7){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x+1][y][z] == 0 || cubeArray[x+1][y][z] == 1){
                            dummyCube[x+1][y][z] = 1;
                        }else{
                            skipX[j] = x+1;
                            skipY[j] = y;
                            skipZ[j] = z;
                            skipR[j] = 1;
                            j++;
                        }
                    }else if(x == 7 && y != 0){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y-1][z] == 0 || cubeArray[x][y-1][z] == 1){
                            dummyCube[x][y-1][z] = 1;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y-1;
                            skipZ[j] = z;
                            skipR[j] = 1;
                            j++;
                        }
                    }else if(y == 0 && x != 0){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x-1][y][z] == 0 || cubeArray[x-1][y][z] == 1){
                            dummyCube[x-1][y][z] = 1;
                        }else{
                            skipX[j] = x-1;
                            skipY[j] = y;
                            skipZ[j] = z;
                            skipR[j] = 1;
                            j++;
                        }
                    }
                }
                //X ROTATION
                if(cubeArray[x][y][z] == 3){
                    if(z == 0 && y != 7){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y+1][z] == 0 || cubeArray[x][y+1][z] == 3){
                            dummyCube[x][y+1][z] = 3;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y+1;
                            skipZ[j] = z;
                            skipR[j] = 3;
                            j++;
                        }
                    }else if(y == 7 && z != 7){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y][z+1] == 0 || cubeArray[x][y][z+1] == 3){
                            dummyCube[x][y][z+1] = 3;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y;
                            skipZ[j] = z+1;
                            skipR[j] = 3;
                            j++;
                        }
                    }else if(z == 7 && y != 0){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y-1][z] == 0 || cubeArray[x][y-1][z] == 3){
                            dummyCube[x][y-1][z] = 3;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y-1;
                            skipZ[j] = z;
                            skipR[j] = 3;
                            j++;
                        }
                    }else if(y == 0 && z != 0){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y][z-1] == 0 || cubeArray[x][y][z-1] == 3){
                            dummyCube[x][y][z-1] = 3;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y;
                            skipZ[j] = z-1;
                            skipR[j] = 3;
                            j++;
                        }
                    }
                }
                //Y ROTATION
                if(cubeArray[x][y][z] == 4){
                    if(z == 0 && x != 0){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x-1][y][z] == 0 || cubeArray[x-1][y][z] == 4){
                            dummyCube[x-1][y][z] = 4;
                        }else{
                            skipX[j] = x-1;
                            skipY[j] = y;
                            skipZ[j] = z;
                            skipR[j] = 4;
                            j++;
                        }
                    }else if(x == 0 && z != 7){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y][z+1] == 0 || cubeArray[x][y][z+1] == 4){
                            dummyCube[x][y][z+1] = 4;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y;
                            skipZ[j] = z+1;
                            skipR[j] = 4;
                            j++;
                        }
                    }else if(z == 7 && x != 7){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x+1][y][z] == 0 || cubeArray[x+1][y][z] == 4){
                            dummyCube[x+1][y][z] = 4;
                        }else{
                            skipX[j] = x+1;
                            skipY[j] = y;
                            skipZ[j] = z;
                            skipR[j] = 4;
                            j++;
                        }
                    }else if(x == 7 && z != 0){
                        cubeArray[x][y][z] = 0;
                        if(cubeArray[x][y][z-1] == 0 || cubeArray[x][y][z-1] == 4){
                            dummyCube[x][y][z-1] = 4;
                        }else{
                            skipX[j] = x;
                            skipY[j] = y;
                            skipZ[j] = z-1;
                            skipR[j] = 4;
                            j++;
                        }
                    }
                }
            }
        }
    }

    //coppy rotated array from dummy to cubeArray
    memcpy(cubeArray, dummyCube, sizeof(cubeArray));
}

//COS WAVE SURFACE
void cube::wave(void){
    clear();
    X = 0;
    Y = 0;
    Z = 0;

    for(int x = 0; x < size; x++){
       for(int y = 0; y < size; y++){
           X = cos(phase2 + map(x,0,size-1,0,2*PI));
           Y = cos(phase3 + map(y,0,size-1,0,2*PI));
           Z = X + Y;
           Z = round(map(Z,-2,2,0,size-1));
           cubeArray[x][y][(int)Z] = 1;
       }
    }

    phase2 += PI/32;
    phase3 += PI/32;

    genChars();
    sendChars();
    Sleep(5);
}

void cube::sphere(int x_, int y_, int z_){
    clear();
    double R = 2;
    X = 0;
    Y = 0;
    Z = 0;

    for(int x = 0; x < size; x++){
        for(int y = 0; y < size; y++){
            for(int z = 0; z < size; z++){
                X = map(x,0,size-1,-4,4);
                Y = map(y,0,size-1,-4,4);
                Z = map(z,0,size-1,-4,4);

                if(X*X + Y*Y + Z*Z >= R*R-1 && X*X + Y*Y + Z*Z <= R*R+1){
                    if(x + x_ < size && y + y_ < size && z + z_ < size && x + x_ >= 0 && y + y_ >= 0 && z + z_ >= 0){
                        cubeArray[x + x_][y + y_][z + z_] = 1;
                    }
                }
            }
        }
    }

    genChars();
    sendChars();
    Sleep(6);
}

//3D sine wave from centre
int cube::sine(double height, int fade){
    int hold = 0;
    int done_count = 1;

    clear();
    Z = 0;
    for(int x = 0; x < size; x++){
       for(int y = 0; y < size; y++){
           hold = 0;
            Z = sin(phase1 + sqrt(pow(map(x,0,size-1,-PI,PI),2) + pow(map(y,0,size-1,-PI,PI),2)));
            Z = round(map(Z,-0.9,0.9,3.5-height/2,3.5+height/2));
            if(fade == 1 && dummyCube[x][y][(int)Z] == 1){
                dummyCube[x][y][(int)Z] = 2;
            }else if(fade == 1 && dummyCube[x][y][(int)Z] != 1){
                for(int z = 0; z < size; z++){
                    if(dummyCube[x][y][z] == 2){
                        cubeArray[x][y][z] = 1;
                        done_count++;
                        hold = 1;
                    }
                }
            }
            if(fade == 0 || (fade == 1 && hold == 0)){
                cubeArray[x][y][(int)Z] = 1;
            }
        }
    }
    phase1 += PI/24;

    genChars();
    sendChars();
    Sleep(10);

    if(done_count >= 64){
        fadeSet = 0;
        return 1;
    }else{
        return 0;
    }
}

void cube::side_sine(void){
    clear();
    Z = 0;
    for(int x = 0; x < size; x++){
       for(int y = 0; y < size; y++){
            Z = sin(phase1 + sqrt(pow(map(x,-8,size-1,-PI,PI),2) + pow(map(y,-8,size-1,-PI,PI),2)));
            Z = round(map(Z,-0.9,0.9,0,7));
            cubeArray[x][y][(int)Z] = 1;
        }
    }
    phase1 += PI/24;
    genChars();
    sendChars();
    Sleep(5);
}

//simply supported thin plate
void cube::plate_mode(int m, int n){
    int L = 7;
    clear();
    Z = 0;
    for(int x = 0; x < size; x++){
       for(int y = 0; y < size; y++){
            Z = sin((m*PI*x)/L)*sin((n*PI*y)/L);
            Z *= plate_ratio;
            Z = round(map(Z,-1.2,1.2,0,7));
            cubeArray[x][y][(int)Z] = 1;
        }
    }

    plate_ratio += plate_dir*0.1;

    if(abs(plate_ratio) >= 1){
        plate_dir *= -1;
    }
    genChars();
    sendChars();
    Sleep(5);
}

//fireworks
void cube::fireworks(void){

}

//WIPER
int cube::wipe(char axis){
    if(slow_count > 3){
        clear();
        for(int x = 0; x < size; x++){
            for(int y = 0; y < size; y++){
                for(int z = 0; z < size; z++){
                    if(x == wipePos && axis == 'x'){
                        cubeArray[x][y][z] = 1;
                    }else if(y == wipePos && axis == 'y'){
                        cubeArray[x][y][z] = 1;
                    }else if(z == wipePos && axis == 'z'){
                        cubeArray[x][y][z] = 1;
                    }
                }
            }
        }
        wipePos += wipeDir;
        slow_count = 0;

        if(wipePos >= size){
            wipePos = size - 2;
            wipeDir *= -1;
        }else if(wipePos < 0){
            wipePos = 1;
            wipeDir *= -1;
        }
        wipe_count++;
    }
    slow_count++;
    genChars();
    sendChars();
    Sleep(5);

    if(wipe_count > 22 && wipePos == 3){
        wipe_count = 0;
        return 1        ;
    }else{
        return 0;
    }
}

//SPIRAL/HELIX
void cube::spiral(double spiralR, int fade){
    clear();
    X = 0;
    Y = 0;
    Z = 0;

    for(int z = fade_step; z < spiral_step; z++){
        Y = cos(phase1 + map(z,0,size-1,0,2*PI));
        X = sin(phase1 + map(z,0,size-1,0,2*PI));
        Y = map(Y,-1.1,0.9,0+spiralR,size-1-spiralR);
        X = map(X,-1.1,0.9,0+spiralR,size-1-spiralR);
        cubeArray[(int)X][(int)Y][z] = 1;

        Y = cos(PI/8 + phase1 + map(z,0,size-1,0,2*PI));
        X = sin(PI/8 + phase1 + map(z,0,size-1,0,2*PI));
        Y = map(Y,-1.1,0.9,0+spiralR,size-1-spiralR);
        X = map(X,-1.1,0.9,0+spiralR,size-1-spiralR);
        cubeArray[(int)X][(int)Y][z] = 1;

        Y = cos(PI/5 + phase1 + map(z,0,size-1,0,2*PI));
        X = sin(PI/5 + phase1 + map(z,0,size-1,0,2*PI));
        Y = map(Y,-1.1,0.9,0+spiralR,size-1-spiralR);
        X = map(X,-1.1,0.9,0+spiralR,size-1-spiralR);
        cubeArray[(int)X][(int)Y][z] = 1;

    }

    if((phase1/(PI) - floor(phase1/(PI))) < 0.05 && spiral_step < size && fade == 0){
        spiral_step++;
        fade_step = 0;
    }

    if((phase1/(2*PI) - floor(phase1/(2*PI))) < 0.05 && fade == 1){
        fade_step++;
    }


    phase1 += PI/16;

    genChars();
    sendChars();
    Sleep(5);
}

//SPINNING PLANE
int cube::spin(int spin_mode, char axis){
    clear();
    X = 0;
    Y = 0;
    Z = 0;

    for(int x = 0; x < size; x++){
        for(int y = 0; y < size; y++){
            for(int z = 0; z < size; z++){
                if(axis == 'z'){
                    X = map(x,0,size-1,-100,100)*sin(spinAngle);
                    Y = map(y,0,size-1,-100,100)*cos(spinAngle);
                    X = map(X,-1,1,0,size-1);
                    Y = map(Y,-1,1,0,size-1);
                    if(abs(X-Y) < 50){
                        cubeArray[x][y][z] = 1;
                    }
                }else if(axis == 'x'){
                    X = map(x,0,size-1,-100,100)*sin(spinAngle);
                    Z = map(z,0,size-1,-100,100)*cos(spinAngle);
                    X = map(X,-1,1,0,size-1);
                    Z = map(Z,-1,1,0,size-1);
                    if(abs(X-Z) < 50){
                        cubeArray[x][y][z] = 1;
                    }
                }
            }
        }
    }

    spinAngle += PI/32;

    genChars();
    sendChars();
    Sleep(5);

    if(spinAngle > 3*PI && spin_mode == 1){
        return 1;
    }else if(spinAngle > 3.5*PI && spin_mode == 2){
        return 1;
    }else if(spinAngle > 4*PI && spin_mode == 3){
        return 1;
    }else{
        return 0;
    }
}

//spin an image array
void cube::spin_image(int array[8][8]){
}

//PULSING CUBE FROM CENTRE
int cube::grow(void){
    if(slow_count > 4){
        clear();
        for(int x = 0; x < size; x++){
            for(int y = 0; y < size; y++){
                for(int z = 0; z < size; z++){
                    //corners
                    if(abs(2*(x-centre)) == cubeGrowSize && abs(2*(y-centre)) == cubeGrowSize && abs(2*(z-centre)) == cubeGrowSize){
                        cubeArray[x][y][z] = 1;
                    }
                    //sides
                    if(abs(2*(x-centre)) == cubeGrowSize && abs(2*(y-centre)) == cubeGrowSize && abs(2*(z-centre)) < cubeGrowSize){
                        cubeArray[x][y][z] = 1;
                    }
                    if(abs(2*(x-centre)) == cubeGrowSize && abs(2*(z-centre)) == cubeGrowSize && abs(2*(y-centre)) < cubeGrowSize){
                        cubeArray[x][y][z] = 1;
                    }
                    if(abs(2*(z-centre)) == cubeGrowSize && abs(2*(y-centre)) == cubeGrowSize && abs(2*(x-centre)) < cubeGrowSize){
                        cubeArray[x][y][z] = 1;
                    }
                }
            }
        }

        if(cubeGrowSize == 7){
            NOC++;
        }

        cubeGrowSize += cubeGrowDir;
            if(cubeGrowSize > 7){
            cubeGrowSize = 5;
            cubeGrowDir *= -1;
        }else if(cubeGrowSize < 0){
            cubeGrowSize = 3;
            cubeGrowDir *= -1;
        }
        slow_count = 0;
    }

    slow_count++;
    genChars();
    sendChars();
    Sleep(7);
    return NOC;
}

//RANDOM RAIN EFFECT
void cube::rain(int fade){
    int count = 0;

    if(rainDrops > rand()%4+0 && fade == 0){
        rX = rand()%8+0;
        rY = rand()%8+0;
        while(cubeArray[rX][rY][7] != 0 && count < 100){
            rX = rand()%8+0;
            rY = rand()%8+0;
            count++;
        }
        cubeArray[rX][rY][7] = 1;
        rainDrops = 0;
    }

    if(slow_count > 3){
            for(int x = 0; x < size; x++){
                for(int y = 0; y < size; y++){
                    for(int z = 0; z < size-1; z++){
                        cubeArray[x][y][z] = cubeArray[x][y][z+1];
                        if(cubeArray[x][y][z] == 1){
                            cubeArray[x][y][7] = 5;
                        }
                    }
                }
            }
        slow_count = 0;
    }

    slow_count++;
    rainDrops++;

    genChars();
    sendChars();
    Sleep(5);
}

//swap random leds accross cube
void cube::random_swap(void){
    srand(time(NULL));
    if(swap_step == 0){
        for(int x = 0; x < size; x++){
            for(int z = 0; z < size; z++){
                cubeArray[x][0][z] = 1;
            }
        }
        hold(20);
        swap_step++;
    }else if(swap_step < 33){
        rX = rand()%8+0;
        rZ = rand()%8+0;
        while(cubeArray[rX][0][rZ] != 1){
            rX = rand()%8+0;
            rZ = rand()%8+0;
        }
        cubeArray[rX][1][rZ] = 3;
        cubeArray[rX][0][rZ] = 0;
        swap_step++;
    }
    if(swap_step < 33 && slow_count > 1){
        for(int x = 0; x < size; x++){
            for(int y = size-1; y > 1; y--){
                for(int z = 0; z < size; z++){
                    if(cubeArray[x][y-1][z] == 3){
                        cubeArray[x][y][z] = cubeArray[x][y-1][z];
                        cubeArray[x][y-1][z] = 0;
                        if(y == rand()%8+0){
                            cubeArray[x][y][z] = 4;
                        }
                    }
                }
            }
        }
        slow_count = 0;
    }

    slow_count++;
    genChars();
    sendChars();
    Sleep(5);
}

//snake game
void cube::snake(int initial_length, int speed){
    if(snakeStart == 1){
        clear();
        clear_dummy();
        maxLength = initial_length;
        tailX = headX;
        tailY = headY;
        tailZ = headZ;
        snakeStart = 0;
        snakeLength = 1;
        chopNum = 1;
        snakeAdd = 1;
        snakeCount = 0;
        changeDir = 0;
        tailChop = 0;
        srand(time(NULL));
    }

    if(slow_count > 10-speed){
        //add head
        if(snakeAdd == 1){
            dummyCube[headX][headY][headZ] = snakeLength;
            snakeAdd = 0;
        }

        //chop tail
        if(snakeLength > maxLength && tailChop == 1){
            dummyCube[tailX][tailY][tailZ] = 0;

            if(dummyCube[tailX-1][tailY][tailZ] == chopNum && tailX-1 != -1){
                tailX--;
            }else if(dummyCube[tailX+1][tailY][tailZ] == chopNum && tailX+1 != size){
                tailX++;
            }else if(dummyCube[tailX][tailY-1][tailZ] == chopNum && tailY-1 != -1){
                tailY--;
            }else if(dummyCube[tailX][tailY+1][tailZ] == chopNum && tailY+1 != size){
                tailY++;
            }else if(dummyCube[tailX][tailY][tailZ-1] == chopNum && tailZ-1 != -1){
                tailZ--;
            }else if(dummyCube[tailX][tailY][tailZ+1] == chopNum && tailZ+1 != size){
                tailZ++;
            }
            chopNum++;
            tailChop = 0;
        }

        //move current direction
        switch(snakeDir){
            case 'u':
                if(headZ-1 != -1 && dummyCube[headX][headY][headZ-1] == 0){
                    headZ--;
                    snakeLength++;
                    snakeAdd = 1;
                    snakeCount = 0;
                    tailChop = 1;
                }else{
                    changeDir = 1;
                }
                break;
            case 'd':
                if(headZ+1 != size && dummyCube[headX][headY][headZ+1] == 0){
                    headZ++;
                    snakeLength++;
                    snakeAdd = 1;
                    snakeCount = 0;
                    tailChop = 1;
                }else{
                    changeDir = 1;
                }
                break;
            case 'n':
                if(headY-1 != -1 && dummyCube[headX][headY-1][headZ] == 0){
                    headY--;
                    snakeLength++;
                    snakeAdd = 1;
                    snakeCount = 0;
                    tailChop = 1;
                }else{
                    changeDir = 1;
                }
                break;
            case 's':
                if(headY+1 != size && dummyCube[headX][headY+1][headZ] == 0){
                    headY++;
                    snakeLength++;
                    snakeAdd = 1;
                    snakeCount = 0;
                    tailChop = 1;
                }else{
                    changeDir = 1;
                }
                break;
            case 'e':
                if(headX+1 != size && dummyCube[headX+1][headY][headZ] == 0){
                    headX++;
                    snakeLength++;
                    snakeAdd = 1;
                    snakeCount = 0;
                    tailChop = 1;
                }else{
                    changeDir = 1;
                }
                break;
            case 'w':
                if(headX-1 != -1 && dummyCube[headX-1][headY][headZ] == 0){
                    headX--;
                    snakeLength++;
                    snakeAdd = 1;
                    snakeCount = 0;
                    tailChop = 1;
                }else{
                    changeDir = 1;
                }
                break;
        }

        //randomly change direction
        if(rand()%8+0 == 2){
            changeDir = 1;
        }

        //change direction
        if(changeDir){
            while(changeDir == 1){
                switch(rand()%6+1){
                    case 1: //u
                        if(snakeDir != 'd' && snakeDir != 'u'){
                            snakeDir = 'u';
                            changeDir = 0;
                        }else{
                            snakeCount++;
                        }
                        break;
                    case 2: //d
                        if(snakeDir != 'u' && snakeDir != 'd'){
                            snakeDir = 'd';
                            changeDir = 0;
                        }else{
                            snakeCount++;
                        }
                        break;
                    case 3: //n
                        if(snakeDir != 's' && snakeDir != 'n'){
                            snakeDir = 'n';
                            changeDir = 0;
                        }else{
                            snakeCount++;
                        }
                        break;
                    case 4: //e
                        if(snakeDir != 'w' && snakeDir != 'e'){
                            snakeDir = 'e';
                            changeDir = 0;
                        }else{
                            snakeCount++;
                        }
                        break;
                    case 5: //s
                        if(snakeDir != 'n' && snakeDir != 's'){
                            snakeDir = 's';
                            changeDir = 0;
                        }else{
                            snakeCount++;
                        }
                        break;
                    case 6: //w
                        if(snakeDir != 'e' && snakeDir != 'w'){
                            snakeDir = 'w';
                            changeDir = 0;
                        }else{
                            snakeCount++;
                        }
                        break;
                    default:
                        snakeCount++;
                        break;
                }

                if(snakeCount > 10){
                    snakeStart = 1;
                    break;
                }
            }
            snakeCount = 0;
        }
        slow_count = 0;
    }
    slow_count++;
    if(snakeAdd == 1){
        clear();
        for(int x = 0; x < size; x++){
            for(int y = 0; y < size; y++){
                for(int z = 0; z < size; z++){
                    if(dummyCube[x][y][z] != 0){
                        cubeArray[x][y][z] = 1;
                    }
                }
            }
        }
        genChars();
        sendChars();
        Sleep(5);
    }
}

//write a string of characters
int cube::spell(char data[]){
    if(newLetter == 1){
        switch(data[letter]){
            case 'A':
                loadArray(I.A);
                break;
            case 'B':
                loadArray(I.B);
                break;
            case 'C':
                loadArray(I.C);
                break;
            case 'D':
                loadArray(I.D);
                break;
            case 'E':
                loadArray(I.E);
                break;
            case 'F':
                loadArray(I.F);
                break;
            case 'G':
                loadArray(I.G);
                break;
            case 'H':
                loadArray(I.H);
                break;
            case 'I':
                loadArray(I.I);
                break;
            case 'J':
                loadArray(I.J);
                break;
            case 'K':
                loadArray(I.K);
                break;
            case 'L':
                loadArray(I.L);
                break;
            case 'M':
                loadArray(I.M);
                break;
            case 'N':
                loadArray(I.N);
                break;
            case 'O':
                loadArray(I.O);
                break;
            case 'P':
                loadArray(I.P);
                break;
            case 'Q':
                loadArray(I.Q);
                break;
            case 'R':
                loadArray(I.R);
                break;
            case 'S':
                loadArray(I.S);
                break;
            case 'T':
                loadArray(I.T);
                break;
            case 'U':
                loadArray(I.U);
                break;
            case 'V':
                loadArray(I.V);
                break;
            case 'W':
                loadArray(I.W);
                break;
            case 'X':
                loadArray(I.X);
                break;
            case 'Y':
                loadArray(I.Y);
                break;
            case 'Z':
                loadArray(I.Z);
                break;
            case ' ':
                loadArray(I.space);
                break;
        }
        newLetter = 0;
        letter++;
    }

    if(slow_count > 4 || (data[letter-1] == ' ' && slow_count > 1)){
        for(int x = 0; x < size; x++){
            for(int y = 0; y < size; y++){
                for(int z = 0; z < size; z++){
                    if((cubeArray[x][y][z] == 1 || cubeArray[x][y][z] == 5) && y > 0){
                        cubeArray[x][y-1][z] = cubeArray[x][y][z];
                        cubeArray[x][y][z] = 0;
                        if(y == 2){
                            newLetter = 1;
                        }
                    }else if((cubeArray[x][y][z] == 1 || cubeArray[x][y][z] == 5) && y == 0){
                        cubeArray[x][y][z] = 0;

                    }
                }
            }
        }
        slow_count = 0;
    }

    genChars();
    sendChars();
    Sleep(5);
    slow_count++;

    if(data[letter-1] == '\0'){
        return 1;
    }else{
        return 0;
    }
}

void cube::atom(void){
}

//fade between animations
/*
    a1 = 0 --- from NOTHING
        a2 = 1 --- to SPIN using RANDOM
        a2 = 2 --- to BOTTOM PLANE
    a1 = 1 --- from SINE
        a2 = 1 --- to TOP PLANE
    a1 = 2 --- from TOP PLANE
        a2 = 1 --- to GROW CUBE
    a1 = 3 --- from GROW CUBE
        a2 = 1 --- to NOTHING
    a1 = 4 --- from BOTTOM PLANE
        a2 = 1 --- to SIDE SINE
*/
void cube::fade(int a1, int a2){
    int intX = 3;
    int intY = 3;
    int x = intX;
    int y = intY;
    int clear = 1;
    int clear_dir = 1;
    clear_dummy();

    //from NOTHING
    if(a1 == 0){
        //to SPIN using RANDOM
        if(a2 == 1){
            for(int x = 0; x < size; x++){
                for(int y = 0; y < size; y++){
                    for(int z = 0; z < size; z++){
                        X = map(x,0,size-1,-100,100)*sin(spinAngle);
                        Y = map(y,0,size-1,-100,100)*cos(spinAngle);
                        X = map(X,-1,1,0,size-1);
                        Y = map(Y,-1,1,0,size-1);
                        if(abs(X-Y) < 50){
                            dummyCube[x][y][z] = 1;
                        }
                    }
                }
            }
            while(!random_expand(a2));
        }else if(a2 == 2){
            while(clear < 9){
                for(int Y = intY; Y < intY + clear; Y++){
                    if(x != -1 && x != 8 && y != -1 && y!= 8){
                        cubeArray[x][y][0] = 1;
                    }
                    y += clear_dir;
                    genChars();
                    sendChars();
                    Sleep(5);
                }
                for(int X = intX; X < intX + clear; X++){
                    if(x != -1 && x != 8 && y != -1 && y!= 8){
                        cubeArray[x][y][0] = 1;
                    }
                    x += clear_dir;
                    genChars();
                    sendChars();
                    Sleep(5);
                }
                intX += clear;
                intY += clear;
                clear_dir *= -1;
                clear++;
            }
        }
    }

    //from SINE
    if(a1 == 1){
        //to TOP PLANE
        if(a2 == 1 && fadeSet == 0){
            for(int x = 0; x < size; x++){
               for(int y = 0; y < size; y++){
                   dummyCube[x][y][7] = 1;
               }
            }
        }
        while(!sine(7,a2));
    }

    //from TOP PLANE
    if(a1 == 2){
        //to GROW CUBE
        if(a2 == 1){
            while(clear < 7){
                for(int Y = intY; Y < intY + clear; Y++){
                    if(x != 0 && x != 7 && y != 0 && y!= 7){
                        cubeArray[x][y][7] = 0;
                    }
                    y += clear_dir;
                    genChars();
                    sendChars();
                    Sleep(5);
                }
                for(int X = intX; X < intX + clear; X++){
                    if(x != 0 && x != 7 && y != 0 && y!= 7){
                        cubeArray[x][y][7] = 0;
                    }
                    x += clear_dir;
                    genChars();
                    sendChars();
                    Sleep(5);
                }
                intX += clear;
                intY += clear;
                clear_dir *= -1;
                clear++;
            }
            for(int z = 6; z >= 0; z--){
                cubeArray[0][0][z] = 1;
                cubeArray[0][7][z] = 1;
                cubeArray[7][0][z] = 1;
                cubeArray[7][7][z] = 1;
                genChars();
                sendChars();
                Sleep(5);
            }
            for(int i = 0; i <= 3; i++){
                cubeArray[0][7-i][0] = 1;
                cubeArray[0][i][0] = 1;
                cubeArray[7][7-i][0] = 1;
                cubeArray[7][i][0] = 1;
                cubeArray[7-i][0][0] = 1;
                cubeArray[i][0][0] = 1;
                cubeArray[7-i][7][0] = 1;
                cubeArray[i][7][0] = 1;
                genChars();
                sendChars();
                Sleep(5);
            }
        }
    }

    //from GROW CUBE
    if(a1 == 3){
        //to NOTHING
        if(a2 == 1){
            for(int i = 0; i <= 3; i++){
                cubeArray[0][7-i][0] = 0;
                cubeArray[0][i][0] = 0;
                cubeArray[7][7-i][0] = 0;
                cubeArray[7][i][0] = 0;
                cubeArray[7-i][0][0] = 0;
                cubeArray[i][0][0] = 0;
                cubeArray[7-i][7][0] = 0;
                cubeArray[i][7][0] = 0;
                genChars();
                sendChars();
                Sleep(5);
            }
            for(int z = 0; z < size-1; z++){
                cubeArray[0][0][z] = 0;
                cubeArray[0][7][z] = 0;
                cubeArray[7][0][z] = 0;
                cubeArray[7][7][z] = 0;
                genChars();
                sendChars();
                Sleep(5);
            }
            for(int i = 0; i <= 3; i++){
                cubeArray[0][7-i][7] = 0;
                cubeArray[0][i][7] = 0;
                cubeArray[7][7-i][7] = 0;
                cubeArray[7][i][7] = 0;
                cubeArray[7-i][0][7] = 0;
                cubeArray[i][0][7] = 0;
                cubeArray[7-i][7][7] = 0;
                cubeArray[i][7][7] = 0;
                genChars();
                sendChars();
                Sleep(10);
            }
        }
    }

    //from BOTTOM PLANE
    if(a1 == 4){
        int fc = 0;
        for(int x = 0; x < size; x++){
           for(int y = 0; y < size; y++){
                Z = sin(phase1 + sqrt(pow(map(x,-8,size-1,-PI,PI),2) + pow(map(y,-8,size-1,-PI,PI),2)));
                Z = round(map(Z,-0.9,0.9,0,7));
                dummyCube[x][y][(int)Z] = 1;
            }
        }
        for(int z = 0; z < size; z++){
            for(int x = 0; x < size; x++){
                for(int y = 0; y < size; y++){
                    if(cubeArray[x][y][z] == 1 && dummyCube[x][y][z] != 1){
                        cubeArray[x][y][z+1] = cubeArray[x][y][z];
                        cubeArray[x][y][z] = 0;
                        fc = 0;
                    }else if(cubeArray[x][y][z] == 1 && dummyCube[x][y][z] == 1){
                        fc++;
                    }
                }
            }
            genChars();
            sendChars();
            Sleep(5);
            if(fc > 64){
                break;
            }
        }
    }
}

//light up random leds until fully lit
int cube::random_expand(int fade){
    srand(time(NULL));
    int count = 0;
    rX = rand()%8+0;
    rY = rand()%8+0;
    rZ = rand()%8+0;
    while((cubeArray[rX][rY][rZ] == 1 || count < 5000) && randDir == 1){
        rX = rand()%8+0;
        rY = rand()%8+0;
        rZ = rand()%8+0;
        count++;
        if(count > 10000){
            randDir *= -1;
        }
    }
    count = 0;

    while((cubeArray[rX][rY][rZ] == 0 || count < 5000 || (fade == 1 && dummyCube[rX][rY][rZ] == 1 && !randFadeDone)) && randDir == -1){
        rX = rand()%8+0;
        rY = rand()%8+0;
        rZ = rand()%8+0;
        count++;
        if(count > 10000 && fade == 1){
            randFadeDone = 1;
            fadeSet = 0;
            break;
        }else if(count > 10000 && fade == 0){
            randDir *= -1;
        }
    }

    LED_on += randDir;
    cubeArray[rX][rY][rZ] = randDir + 1;

    genChars();
    sendChars();
    Sleep(10);

    return randFadeDone;
}

//ALL ON
void cube::ON(void){
    for(int x = 0; x < size; x++){
        for(int y = 0; y < size; y++){
            for(int z = 0; z < size; z++){
                cubeArray[x][y][z] = 1;
            }
        }
    }

    genChars();
    sendChars();
    //Sleep(2);
}

void cube::hold(int cycles){
    while(hold_count < cycles){
        genChars();
        sendChars();
        Sleep(5);
        hold_count++;
    }
}
//CLEAR CUBE
void cube::clear(void){
    memset(cubeArray, 0, sizeof(cubeArray));
}

void cube::clear_dummy(void){
    memset(dummyCube, 0, sizeof(dummyCube));
}

//MAP FUNCTION FOR ANIMATIONS
double cube::map(double in, double inMin, double inMax, double outMin, double outMax){
    double out;
    out = (in-inMin)/(inMax-inMin)*(outMax-outMin) + outMin;
    return out;
}

//GENERATE ARRAY OF CHARS TO BE SENT TO CUBE AS INSTRUCTIONS
void cube::genChars(void){
    int i = 0;
    memset(cubeChars, 0x00, sizeof(cubeChars));
    for(int z = 0; z < size; z++){
        for(int y = 0; y < size; y++){
            for(int x = 0; x < size; x++){
                //1's normal LED opperations and z rotations, 2's allow for easy direction change of random expand, 3's for x rotations, 4's for y rotation
                //dont print 5's used as no go memory in rain
                if(cubeArray[x][y][z] == 1 || cubeArray[x][y][z] == 2 || cubeArray[x][y][z] == 3 || cubeArray[x][y][z] == 4){
                    if(cubeArray[x][y][z] == 2){
                        cubeArray[x][y][z] = 1;
                    }
                    cubeChars[i] |= 0x01<<((size-1)-x);
                }
            }
            i++;
        }
    }
}

//SEND INSTRUCTIONS TO CUBE
void cube::sendChars(void){
    for(int i = 0; i < 64; i++) {
        S1.write(cubeChars[i]);
    }
    S1.write('e');
	S1.write('n');
	S1.write('d');
}

//CLOSE CONNECTIONS AND STOP ANIMATIONS
void cube::killCube(void){
    clear();
    S1.close();
}

//CLEAR ANIMATION VARIABLES
void cube::clearAnimationVariables(void){\
    flow_count = 0;
    pass1 = 1;
    pass2 = 1;
    pass3 = 1;
    phase1 = 0;
    phase2 = PI/3;
    phase3 = PI/4;
    wipePos = 3;
    wipeDir = 1;
    wipe_count = 0;
    spinAngle = PI/4;
    cubeGrowSize = 7;
    cubeGrowDir = -2;
    randDir = 1;
    fadeSet = 0;
    randFadeDone = 0;
    full_enough = 0;
    slow_count = 0;
    hold_count = 0;
    LED_on = 0;
    X = 0;
    Y = 0;
    Z = 0;
    rX = 0;
    rY = 0;
    rZ = 0;
    for(int i = 0; i < 64; i++){
        skipX[i] = -1;
        skipY[i] = -1;
        skipZ[i] = -1;
        skipR[i] = 0;
    }
    rainDrops = 0;
    NOC = 0;
    swap_step = 0;
    spiral_step = 0;
    fade_step = 0;
    headX = 3;
    headY = 3;
    headZ = 3;
    snakeStart = 1;
    snakeDir = 'n';
    snakeLength = 1;
    snakeAdd = 1;
    snakeCount = 0;
    changeDir = 0;
    tailChop = 0;
    chopNum = 1;
    maxLength = 0;
    letter = 0;
    newLetter = 1;
    plate_dir = 1;
    plate_ratio = 0.1;
    R = 0;
    clear();
    clear_dummy();
}

