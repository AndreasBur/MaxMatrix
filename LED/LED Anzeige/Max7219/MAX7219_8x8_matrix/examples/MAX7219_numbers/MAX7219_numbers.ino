
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

MAX7219_8x8_matrix LEDmatrix(2,3,4);

void setup() {
 LEDmatrix.clear();
 delay(100);
 LEDmatrix.setBrightness(8);
 delay(100);
}

void loop() {
 
 for (int i=0;i<10;i++)
  {  
    LEDmatrix.set_char(i);
    delay(999);
  }
}
