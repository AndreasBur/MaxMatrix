
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

MAX7219_8x8_matrix LEDmatrix(2,3,4);

char myString[] = "ARDUINO-PROJEKTE.DE ";


void setup() {
 LEDmatrix.clear();
 delay(100);
 LEDmatrix.setBrightness(8);
 delay(100);
// LEDmatrix.set_pixel(0,0,HIGH);
}

void loop() {
  
//  LEDmatrix.set_char('L');
//  delay(1000);  
  /*
for (char i=65;i<96;i++)
{  
  LEDmatrix.set_char(i);
  delay(1000);
}  
  */
 
for (int i=0;i<19;i++)
{  
  LEDmatrix.set_char(myString[i]);
  delay(500);
 // LEDmatrix.invert_matrix();
 // delay(500);
}


}
