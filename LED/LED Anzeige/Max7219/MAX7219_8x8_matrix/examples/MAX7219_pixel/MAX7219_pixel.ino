
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

//    MAX7219_8x8_matrix(DIN, CS, CLK);
MAX7219_8x8_matrix LEDmatrix(2,3,4);
int delay_time = 120;  //ms

void setup() {
  LEDmatrix.clear();
  delay(1000);
  LEDmatrix.setBrightness(8);
}

void loop() {
for (int x=0;x<8;x++){ 
  for (int y=0;y<8;y++){
   LEDmatrix.set_pixel(x,y,HIGH);
   delay(delay_time);
   LEDmatrix.set_pixel(x,y,LOW);
 //   LEDmatrix.invert_pixel(7,7);   
   delay_time-=12;
  }
   delay_time=120;

   delay(delay_time);
 }
 
for (int y=0;y<8;y++){
  for (int x=0;x<8;x++){ 
   LEDmatrix.set_pixel(x,y,HIGH);
   delay(delay_time);
   LEDmatrix.set_pixel(x,y,LOW);
 //   LEDmatrix.invert_pixel(7,7);   
   delay_time-=12;
  }
   delay_time=120;

   delay(delay_time);
 }
}
