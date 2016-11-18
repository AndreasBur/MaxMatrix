
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

//    MAX7219_8x8_matrix(DIN, CS, CLK);
MAX7219_8x8_matrix LEDmatrix(2,3,4);
int delay_time = 20;  //ms

void setup() {
  LEDmatrix.clear();
  delay(1000);
  LEDmatrix.setBrightness(4);
}

void loop() {
  
for (int num=0;num<8;num++){ 
   LEDmatrix.set_col(num,B11011011);
   delay(delay_time);
//   LEDmatrix.set_col(num,0x00);
//   delay(delay_time);
}
for (int num=0;num<8;num++){ 
   LEDmatrix.set_row(num,B11011011);
   delay(delay_time);
//   LEDmatrix.set_row(num,0x00);
//   delay(delay_time);
} 
//LEDmatrix.clear();
}
