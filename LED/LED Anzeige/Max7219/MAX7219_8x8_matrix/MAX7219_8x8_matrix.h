
/***************************************************
** 						  **
**  Max7219 LED Matrix library for Arduino   	  **
** 						  **
**  Downloaded from:				  **
**  www.arduino-projekte.de			  **
**						  **
***************************************************/

#ifndef MAX7219_8x8_matrix_h
#define MAX7219_8x8_matrix_h

//#include <inttypes.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Sprite;

class MAX7219_8x8_matrix
{
  private:

    uint8_t _pinData;
    uint8_t _pinClock;
    uint8_t _pinLoad;

//    uint8_t* _buffer;
    bool _BUFFER[8][8];
    uint8_t _screens;
    uint8_t _maximumX;
    uint8_t _arraydata[8][8];

    void clock();    
    void send_data(uint8_t data);
    void send_data_array(bool data[8]);
    void setRegister(uint8_t, uint8_t);
    void refresh_row(uint8_t);
    void setScanLimit(uint8_t);
   // void buffer(uint8_t, uint8_t, bool);
    uint8_t convert_data(bool data[8]);



  public:

    MAX7219_8x8_matrix(uint8_t DIN, uint8_t CS, uint8_t CLK);
    void setBrightness(uint8_t value);
    void set_pixel(uint8_t x, uint8_t y, bool value);
    void set_pixel_orbital(uint8_t x, bool value);
    void invert_pixel(uint8_t x, uint8_t y);
    void set_row(uint8_t y, uint8_t value);
    void set_col(uint8_t x, uint8_t value);
    void matrix(bool a[8][8]);
    void matrix(uint8_t a[8]);
    void invert_matrix(void);
    void set_char(uint8_t CHAR);
    void mask(uint8_t mask[8], bool value);
    void square(uint8_t size, bool value);
    void clear(void);
    void fill(void);
    void rotate_matrix_cw();
    void rotate_matrix_ccw();

};

#endif

