
/********************************************************
**  Downloaded from:                                   **
**  http://www.arduino-projekte.de                     **
********************************************************/

#include <MAX7219_8x8_matrix.h>

//    MAX7219_8x8_matrix(DIN, CS, CLK);
MAX7219_8x8_matrix LEDmatrix(2,3,4);

uint8_t mask_a[8]=
 {B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001};

uint8_t mask_b[8]=
 {B11111111,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B11111111};

  
void setup() {
 LEDmatrix.clear();
 delay(100);
 LEDmatrix.setBrightness(4);
}

void loop() {
  LEDmatrix.mask(mask_a,HIGH);
  delay(1000);
  LEDmatrix.mask(mask_b,HIGH);
  delay(1000);
  LEDmatrix.mask(mask_a,LOW);
  delay(1000);
  LEDmatrix.mask(mask_b,LOW);
  delay(1000);
}
