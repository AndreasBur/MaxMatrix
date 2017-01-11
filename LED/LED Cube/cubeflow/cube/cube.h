#ifndef _CUBE_H_
#define _CUBE_H_

#include "serial.h"
#include "image.h"

class cube{
    public:
        cube(void);
        void flow_animation(void);
        void ledON(int x, int y, int z);
        void loadArray(int array[8][8]);
        void BMPtoARRAY(void);
        void arrow(int one, int two, int three, int cancel);
        void rotate(void);
        void wave(void);
        void sphere(int x_, int y_, int z_);
        int sine(double height, int fade);
        void side_sine(void);
        void plate_mode(int m, int n);
        void fireworks(void);
        int wipe(char axis);
        void spiral(double spiralR, int fade);
        int spin(int spin_mode, char axis);
        void spin_image(int array[8][8]);
        int grow(void);
        void rain(int fade);
        void random_swap(void);
        void snake(int initial_length, int speed);
        int spell(char data[]);
        void atom(void);
        void fade(int a1, int a2);
        int random_expand(int fade);
        void ON(void);
        void hold(int cycles);
        void clear(void);
        void clear_dummy(void);
        double map(double in, double inMin, double inMax, double outMin, double outMax);
        void genChars(void);
        void sendChars(void);
        void killCube(void);
        void clearAnimationVariables(void);
    private:
        int flow_count;
        int pass1, pass2, pass3;
        int size;
        double centre;
        int cubeArray[8][8][8];
        int dummyCube[8][8][8];
        char cubeChars[64];
        double X, Y, Z;
        double R;
        double phase1, phase2, phase3, spinAngle;
        int wipeDir, wipePos, wipe_count;
        int slow_count;
        int cubeGrowSize;
        int cubeGrowDir;
        int LED_on;
        int full_enough;
        int hold_count;
        int rX, rY, rZ;
        int randDir;
        int fadeSet;
        int randFadeDone;
        int skipX[64], skipY[64], skipZ[64], skipR[64];
        int rainDrops;
        int NOC;
        int swap_step;
        int spiral_step;
        int fade_step;
        int snakeStart;
        int snakeAdd;
        int headX, headY, headZ;
        int tailX, tailY, tailZ;
        int tailChop;
        char snakeDir;
        int changeDir;
        int snakeLength;
        int maxLength;
        int snakeCount;
        int chopNum;
        int letter;
        int newLetter;
        double plate_ratio;
        int plate_dir;
        double t;
        int g;
        serial S1;
        image I;
};

#endif /*_CUBE_H_*/

