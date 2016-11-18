#include "cube.h"
#include <stdio.h>

int main(void){
    cube CUBE;

    while(true){
       //CUBE.sphere(0,0,0);
       CUBE.flow_animation();
       //CUBE.atom();
    }

    CUBE.killCube();
    return 1;
}
