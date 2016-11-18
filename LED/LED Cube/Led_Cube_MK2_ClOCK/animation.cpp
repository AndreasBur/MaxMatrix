#include "animation.h"
#include "DS1307.h"

uint8_t time[] = "00:00:00";

uint8_t Cclock[11][8] = {{0x00, 0x00, 0x7C, 0x44, 0x7C, 0x00, 0x00, 0x00}, 	//0
						{0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00},	//1
						{0x00, 0x00, 0x4C, 0x54, 0x64, 0x00, 0x00, 0x00},	//2
						{0x00, 0x00, 0x44, 0x54, 0x7C, 0x00, 0x00, 0x00},	//3
						{0x00, 0x00, 0x78, 0x08, 0x3C, 0x00, 0x00, 0x00},	//4
						{0x00, 0x00, 0x74, 0x54, 0x5C, 0x00, 0x00, 0x00},	//5
						{0x00, 0x00, 0x7C, 0x14, 0x1C, 0x00, 0x00, 0x00},	//6
						{0x00, 0x00, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00},	//7
						{0x00, 0x00, 0x7C, 0x54, 0x7C, 0x00, 0x00, 0x00},	//8
						{0x00, 0x00, 0x70, 0x50, 0x7C, 0x00, 0x00, 0x00},	//9
						{0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00}};	//:

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
    if(slow < 50){
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
    if(slow < 5){
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
    if(slow < 25){
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
    if(slow < 50){
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
    uint8_t cubeXd = pos+1;
    uint8_t test = 0;
    uint8_t xCen = 0;
    uint8_t yCen = 0;
    uint8_t zCen = 0;
    
    slow++;
    if(slow < 60){
        return 0;
    }
    slow = 0;
    
    //build the cube with conditional statements, 
    //I feel like I could do this more elegantly but it works
    for(uint8_t x = 0; x < Xd; x++){
        for(uint8_t y = 0; y <Yd; y++){
            for(uint8_t z = 0; z < Zd; z++){
                test = 0;
                xCen = absA(2*(x-centre));
                yCen = absA(2*(y-centre));
                zCen = absA(2*(z-centre));
                
                if(xCen == cubeXd && yCen == cubeXd && zCen == cubeXd){
                    c.setVoxel(x, y, z);
                    test = 1;
                }
                if(xCen == cubeXd && yCen == cubeXd && zCen < cubeXd){
                    c.setVoxel(x, y, z);
                    test = 1;
                }
                if(xCen == cubeXd && zCen == cubeXd && yCen < cubeXd){
                    c.setVoxel(x, y, z);
                    test = 1;
                }
                if(zCen == cubeXd && yCen == cubeXd && xCen < cubeXd){
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

void animation::initClock(uint8_t setTime){
	if(setTime){
		RTC.stop();
		RTC.set(DS1307_SEC,0);        //set the seconds
		RTC.set(DS1307_MIN,1);     //set the minutes
		RTC.set(DS1307_HR,9);       //set the hours
		RTC.set(DS1307_DOW,1);       //set the day of the week
		RTC.set(DS1307_DATE,8);       //set the date
		RTC.set(DS1307_MTH,7);        //set the month
		RTC.set(DS1307_YR,12);         //set the year
		RTC.start();
	}
}

uint8_t animation::clock(cube &c){
	int8_t offset = 0;
	int8_t skip = 0;
	int8_t num = 0;
	uint8_t *tempArP;
	uint8_t tempAr[8];
	
    slow++;
    if(slow < 85){
        return 0;
    }
    slow = 0;
    
	updateTime();
	
    memset(ORArray, 0, 28*sizeof(uint8_t));
    
	for(int8_t i = 0; i < 5; i++){
		if(!(i == 0 && time[i] == '0')){
			num = time[i]-48;
			for(int8_t j = 0; j < 8; j++){
				tempAr[j] = Cclock[num][j];
			}

			if(time[i] == '1' || time[i] == ':'){
				skip = 4;
			}else{
				skip = 2;
			}

			for(int8_t r = 0; r < Xd-skip; r++){
				ORArray[r+offset] = tempAr[r+skip];
			}

			offset += Xd - skip - 2;
		}
	}
	
    outerRotate(c);
}

void animation::updateTime(void){
	int hour = 0;
	int minute = 0;
	uint8_t h1, h2, m1, m2;
	
	hour = RTC.get(DS1307_HR,true);
	minute = RTC.get(DS1307_MIN,false);
	
	h2 = hour%10 + '0';
	hour /= 10;
	h1 = hour%10 + '0';
	
	m2 = minute%10 + '0';
	minute /= 10;
	m1 = minute%10 + '0';
	
	if(h1 != 'x'){
		time[0] = h1;
	}		
	if(h2 != 'x'){
		time[1] = h2;
	}
	if(m1 != 'x'){
		time[3] = m1;
	}
	if(m2 != 'x'){
		time[4] = m2;
	}
}

//ROTATE data in ORArray AROUND OUTER PLANES
void animation::outerRotate(cube &c){
	loop();

	//wrap the ORArray arrpound the outside of the cube
	for(int8_t r = 0; r < 28; r++){
		if(r >= 0 && r < Xd){
            c.copyColumn(Xd-1-r, 7, ORArray[27-r]);
        }else if(r >= Xd && r < Xd*2-1){
            c.copyColumn(0, Xd - (r-Xd) - 2, ORArray[27-r]);
        }else if(r >= Xd*2-1 && r < Xd*3-2){
            c.copyColumn(r-(2*Xd-2), 0, ORArray[27-r]);
        }else if(r >= Xd*3-2 && r < Xd*4-3){
            c.copyColumn(7, r-(3*Xd-3), ORArray[27-r]);
        }
	}
}

//Loop ORArray used for rotate function
void animation::loop(void){
	uint8_t temp = 0;

	//shift and loop the ORArray until the correct position is reached
	for(int8_t n = 0; n < rotatePos; n++){
		//shift first column into a temp array
		temp = ORArray[0];

		//shift all columns down
		for(int8_t r = 0; r < Xd*4-5; r++){
			ORArray[r] = ORArray[r+1];
		}

		//move data from first column into last
		ORArray[Zd*4-5] = temp;
	}

	//increment/reset rotatePos, this is what gives the rotation effect
	rotatePos++;
	if(rotatePos > 27){
		rotatePos = 0;
	}
}

//my awesome function to get data out of my sine array
//I use the fact that sine functions are periodic to reduce the Xd of the array
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
