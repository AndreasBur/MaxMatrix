
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

//    MAX7219_8x8_matrix(DIN, CS, CLK);
MAX7219_8x8_matrix LEDmatrix(2,3,4);
int number=0;            

void setup() {
  LEDmatrix.clear();
  delay(1000);
  LEDmatrix.setBrightness(8);
}

void loop() {
  LEDmatrix.set_char(number);
  delay(500);
  LEDmatrix.clear();
  delay(200);
  LEDmatrix.fill();
  LEDmatrix.square(2,LOW);
  
  for (int i=0;i<60;i++){ 
  LEDmatrix.set_pixel_orbital(i,LOW);
  delay(10);
  }
  delay(100);
  number++;
  if (number>9) number=0;
}
