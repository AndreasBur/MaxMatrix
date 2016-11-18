
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

MAX7219_8x8_matrix LEDmatrix(2,3,4);

bool sample_a[8][8]=
 {{0,1,1,1,1,1,1,0},
  {0,0,1,1,1,1,0,0},
  {0,0,1,0,0,1,0,0},
  {0,0,1,0,0,1,0,0},
  {0,0,1,0,0,1,0,0},
  {0,0,1,0,0,1,0,0},
  {0,0,1,1,1,1,0,0},
  {0,1,1,1,1,1,1,0}};

uint8_t sample_c[8]=
 {B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00000000,
  B00000000,
  B00000000};

uint8_t sample_b[8]=
 {B00000001,
  B00000010,
  B00000100,
  B00011000,
  B00011000,
  B00000000,
  B00000000,
  B00000000};
  
void setup() {
 LEDmatrix.clear();
 delay(100);
 LEDmatrix.setBrightness(8); 
 LEDmatrix.matrix(sample_a);  
 delay(1000); 
 LEDmatrix.rotate_matrix_ccw();  //rotate clockwise
 delay(1000);  
 LEDmatrix.matrix(sample_b);
 delay(1000);   
}

void loop() {
  LEDmatrix.rotate_matrix_cw();  //rotate conterclockwise
  delay(1000);  
  
}
