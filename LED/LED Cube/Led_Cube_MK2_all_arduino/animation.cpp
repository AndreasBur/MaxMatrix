#include "animation.h"

animation::animation(){
    reset();
    
    //fill the sine array in memory
    for(uint16_t i = 0; i < 91; i++){
        sinA[i] = sin(i*PI/180);
    }    
}

//SINGLE PLANE BOUNCING
uint8_t animation::bouncePlane(cube &c, uint8_t axis){
    slow++;
    if(slow < 200){
        return 0;
    }
    slow = 0;
    
    //the X animation looks the same but actually clears everything in its path
    //rather than clear everything at the start, it makes a simple but cool 
    //transition between some animations
    if(axis != 'X'){
        c.clearVoxels();
    }
    
    switch(axis){
        case 'X':
            for(uint8_t z = 0; z < Zd; z++){
                for(uint8_t y = 0; y < Yd; y++){
                    for(uint8_t x = 0; x < Xd; x++){
                        if(x == pos - dir){
                            c.clearVoxel(x, y, z);
                        }
                        c.setVoxel(pos, y, z);
                    }
                }
            }
            break;
        case 'Y':
            for(uint8_t x = 0; x < Xd; x++){
                for(uint8_t z = 0; z < Zd; z++){
                    c.setVoxel(x, pos, z);
                }
            }
            break;
        case 'Z':
            for(uint8_t x = 0; x < Xd; x++){
                for(uint8_t y = 0; y < Yd; y++){
                    c.setVoxel(x, y, pos);
                }
            }
            break;
    }
    
    //bounce the pos variable between 0 and 7
    bouncePos();
    
    return 1;
}   

//RANDOM LIGHTS
uint8_t animation::randomExpand(cube &c){
    uint16_t count = 0;
	uint8_t rX, rY, rZ;
    
    slow++;
    if(slow < 20){
        return 0;
    }
    slow = 0;
    
    randomSeed(analogRead(0));
    rX = rand()%8+0;
    rY = rand()%8+0;
    rZ = rand()%8+0;
	
    //find an empty voxel
    while(c.getVoxel(rX, rY, rZ) == 1 && dir == 1){
        rX = random(0, 8);
        rY = random(0, 8);
        rZ = random(0, 8);
        count++;
        
        if(count > 200){
            dir *= -1;
        }
    }
    count = 0;

    //find a full voxel
    while(c.getVoxel(rX, rY, rZ) == 0 && dir == -1){
        rX = random(0, 8);
        rY = random(0, 8);
        rZ = random(0, 8);
        count++;

        if(count > 200){
            dir *= -1;
        }
    }

    //fill or clear the voxel found
    if(dir == 1){
        c.setVoxel(rX, rY, rZ);
    }else{
        c.clearVoxel(rX, rY, rZ);
    }
    
    return 1;
}

//HELIX ANIMATION 
uint8_t animation::helix(cube &c){
    float X = 0;
    float Y = 0;
    float Z = 0;
    
    slow++;
    if(slow < 100){
        return 0;
    }
    slow = 0;
    
    c.clearVoxels();
    
    //use my fancy pants sine function
    for(uint8_t i = 0; i < 3; i++){
        for(uint8_t z = 0; z < 4; z++){
            Z = z*52;
            X = get_sinA(Z + phase + 18*i);
            Y = get_sinA(Z + phase + 90 + 18*i);
            X = (X+1)*4;
            Y = (Y+1)*4;
            c.setVoxel((uint8_t)X, (uint8_t)Y, z);
            c.setVoxel((uint8_t)(8-X), (uint8_t)(8-Y), z+4);
        }
    }
    
    //increment the phase 
    phase+=18;
    
    if(phase > 360){
        phase -= 360;
    }
    
    return 1;
}

//RAIN FUNCTION
uint8_t animation::rain(cube &c){
    uint16_t count = 0;
    uint8_t rX = 0;
    uint8_t rY = 0;
    uint8_t rZ = 0;
    
    slow++;
    if(slow < 200){
        return 0;
    }
    slow = 0;
    
    randomSeed(analogRead(0));
    
    //shift the rain down
    for(int x = 0; x < Xd; x++){
        for(int y = 0; y < Yd; y++){
            for(int z = 0; z < Zd-1; z++){
                if(z == 0 && c.getVoxel(x, y, z)){
                    c.clearVoxel(x, y, z);
                }
                
                if(c.getVoxel(x, y, z+1)){
                    c.clearVoxel(x, y, z+1);
                    c.setVoxel(x, y, z);
                }
            }
        }
    }
    
    //create some raindrops
    rX = random(0,8);
    rY = random(0,8);
    while(c.getVoxel(rX, rY, 7) != 0 && count < 100){
        rX = random(0,8);
        rY = random(0,8);
        count++;
    }
    c.setVoxel(rX, rY, 7);

    return 1;
}

//CUBE PULSE
uint8_t animation::cubePulse(cube &c){
    uint8_t cubeSize = pos+1;
    uint8_t test = 0;
    uint8_t xCen = 0;
    uint8_t yCen = 0;
    uint8_t zCen = 0;
    
    slow++;
    if(slow < 150){
        return 0;
    }
    slow = 0;
    
    //build the cube with consitional statements, 
    //I feel like I could do this more elegantly but it works
    for(uint8_t x = 0; x < Xd; x++){
        for(uint8_t y = 0; y <Yd; y++){
            for(uint8_t z = 0; z < Zd; z++){
                test = 0;
                xCen = absA(2*(x-centre));
                yCen = absA(2*(y-centre));
                zCen = absA(2*(z-centre));
                
                if(xCen == cubeSize && yCen == cubeSize && zCen == cubeSize){
                    c.setVoxel(x, y, z);
                    test = 1;
                }
                if(xCen == cubeSize && yCen == cubeSize && zCen < cubeSize){
                    c.setVoxel(x, y, z);
                    test = 1;
                }
                if(xCen == cubeSize && zCen == cubeSize && yCen < cubeSize){
                    c.setVoxel(x, y, z);
                    test = 1;
                }
                if(zCen == cubeSize && yCen == cubeSize && xCen < cubeSize){
                    c.setVoxel(x, y, z);
                    test = 1;
                }
                
                if(!test && c.getVoxel(x, y, z)){
                    c.clearVoxel(x, y, z);
                }
            }
        }
    }
    
    pos += dir*2;
    
    if(pos > 6 || pos < 0){
        dir *= -1;
        pos += dir*2;
    }
    
    return 1;
}

//do the bounce
void animation::bouncePos(void){
    pos += dir;
    
    if(pos > 7 || pos < 0){
        dir*=-1;
        pos += dir;
    }
}

//reset all variables
void animation::reset(void){
    dir = 1;
    pos = 0;
    slow = 0;
    phase = 0;
}

//my awesome function to get data out of my sine array
//I use the fact that sine functions are periodic to reduce the size of the array
float animation::get_sinA(int16_t deg){
    while(deg > 360){
        deg -= 360;
    }
    
    if(deg <= 90){
        return sinA[deg];
    }else if(deg > 90 && deg <= 180){
        return sinA[90-(deg-90)];
    }else if(deg > 180 && deg <= 270){
        return -sinA[deg-180];
    }else if(deg > 270 && deg <= 360){
        return -sinA[90-(deg-270)];
    }
    return sinA[deg];
}

//absolute value function
int16_t animation::absA(int16_t in){
   int16_t out = in;
   if(out < 0){
        out = -out;
   }
   return out;
}