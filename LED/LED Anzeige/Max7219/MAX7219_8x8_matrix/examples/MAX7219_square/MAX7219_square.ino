
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

MAX7219_8x8_matrix LEDmatrix(2,3,4);

void setup() {
 LEDmatrix.clear();
 delay(100);
 LEDmatrix.setBrightness(4);
}

void loop() {
  for (int i=0;i<9;i+=2)
  {
     LEDmatrix.square(i,HIGH);
     delay(100);
     LEDmatrix.square(i,LOW);
     delay(100);
  }  

}
