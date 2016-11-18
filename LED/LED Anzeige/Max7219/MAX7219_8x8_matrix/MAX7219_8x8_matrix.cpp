
/***************************************************
** 						  **
**  Max7219 LED Matrix library for Arduino   	  **
** 						  **
**  Downloaded from:				  **
**  www.arduino-projekte.de			  **
**						  **
***************************************************/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "MAX7219_8x8_matrix.h"
#include "8x8_font.h"


// Register Address Map
#define REG_NOOP   	0x00
#define REG_DIGIT0 	0x01
#define REG_DIGIT1 	0x02
#define REG_DIGIT2 	0x03
#define REG_DIGIT3 	0x04
#define REG_DIGIT4 	0x05
#define REG_DIGIT5 	0x06
#define REG_DIGIT6 	0x07
#define REG_DIGIT7 	0x08
#define REG_DECODEMODE  0x09
#define REG_INTENSITY   0x0A
#define REG_SCANLIMIT   0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPLAYTEST 0x0F


//******************************************************************************
// Constructor

MAX7219_8x8_matrix::MAX7219_8x8_matrix(uint8_t DIN_pin, uint8_t CS_pin, uint8_t CLK_pin)
{ 
  _pinData  = DIN_pin;
  _pinClock = CLK_pin;
  _pinLoad  = CS_pin;

  pinMode(_pinClock, OUTPUT);
  pinMode(_pinData, OUTPUT);
  pinMode(_pinLoad, OUTPUT);

  _BUFFER [8][8];


  // initialize registers
  clear();             			// clear display
  setBrightness(0x0F); 			// maximum brightness

  setRegister(REG_SCANLIMIT, 0x07);   	//8x Pixel
  setRegister(REG_SHUTDOWN, 0x01);    	// normal operation
  setRegister(REG_DECODEMODE, 0x00);  	// pixels not integers
  setRegister(REG_DISPLAYTEST, 0x00); 	// not in test mode
}


//*************************************************************************
/*
void MAX7219_8x8_matrix::send_data(uint8_t reg, uint8_t data)
{	
  uint8_t mask;

  for (uint8_t i=8;i>0;i--)
  {
    mask = 0x01 << (i-1);
  digitalWrite(_pinClock, LOW);  
    if (reg & mask)
	{digitalWrite(_pinData, HIGH);}
    else 
	{digitalWrite(_pinData, LOW);} 
  digitalWrite(_pinClock, HIGH);   
  }

  for (uint8_t i=8;i>0;i--)
  {
    mask = 0x01 << (i-1);
    digitalWrite(_pinClock, LOW);
 
    if (data & mask)
	{digitalWrite(_pinData, HIGH);}
    else 
	{digitalWrite(_pinData, LOW);}  
    digitalWrite(_pinClock, HIGH);  
  }
}
*/
//*************************************************************************

void MAX7219_8x8_matrix::send_data(uint8_t data)
{	
  uint8_t mask;
  for (uint8_t i=8;i>0;i--)
  {
    mask = 0x01 << (i-1);
    digitalWrite(_pinClock, LOW); 
    if (data & mask)
      {digitalWrite(_pinData, HIGH);}
    else 
      {digitalWrite(_pinData, LOW);} 
    digitalWrite(_pinClock, HIGH);   
  }
}

//*************************************************************************

void MAX7219_8x8_matrix::send_data_array(bool data[8])
{	

  for (uint8_t i=8;i>0;i--)
  {
    digitalWrite(_pinClock, LOW); 
    digitalWrite(_pinData, data[i-1]);
    digitalWrite(_pinClock, HIGH);  
  }
}


//*************************************************************************

uint8_t MAX7219_8x8_matrix::convert_data(bool data[8])
{
  uint8_t export_data=0x00;

  for (uint8_t i=0;i<8;i++)
   {
     export_data |= data[i]<<i;
   }
  return export_data;
}


//*************************************************************************

void MAX7219_8x8_matrix::setRegister(uint8_t reg, uint8_t data)
{
  digitalWrite(_pinLoad, LOW); 
  send_data (reg);		//send register
  send_data (data);		//send 8-bit data
  digitalWrite(_pinLoad, HIGH); 
  digitalWrite(_pinLoad, LOW); 
}


//*************************************************************************

void MAX7219_8x8_matrix::clock()
{
  digitalWrite(_pinClock, LOW);   
  digitalWrite(_pinClock, HIGH);  
}


//*************************************************************************

void MAX7219_8x8_matrix::refresh_row(uint8_t row)
{
//  if (!_buffer) return;   //Error

 // if (row >= 8) return;   //Error

  bool _ROW[8];
 
  for (uint8_t i=0;i<8;i++)
  {
     _ROW[i] = _BUFFER[row][i];	
  }

  digitalWrite(_pinLoad, LOW); 		// begin
//  send_data(row+1, _buffer[row]);

  send_data(row+1);			//send register
  send_data_array(_ROW);		//send 8bit data

  digitalWrite(_pinLoad, HIGH);  	// latch in data
  digitalWrite(_pinLoad, LOW);   	// end

}


//*************************************************************************
/*
void MAX7219_8x8_matrix::buffer(uint8_t x, uint8_t y, bool value)
{
  if (!_buffer) return;
  if (x==7) {x=0;} else {x++;}    //Modulanpassung  

  if (x >= _maximumX || y >= 8) return;

  uint8_t offset = x; // record x
  x %= 8;             // make x relative to a single matrix
  offset -= x;        // calculate buffer offset

  // wrap shift relative x for nexus module layout
  if (x == 0){
    x = 8;
  }
  --x;

  // record value in buffer
  if(value){
    _buffer[y] |= 0x01 << x;
  }
  else{
    _buffer[y] &= ~(0x01 << x);
  }

}

*/

//*************************************************************************

void MAX7219_8x8_matrix::set_pixel(uint8_t x, uint8_t y, bool value)
{
  _BUFFER[x][y] = value;

  refresh_row(x);
}


//*************************************************************************

void MAX7219_8x8_matrix::invert_pixel(uint8_t x, uint8_t y)
{
  if (_BUFFER[x][y])
   {   set_pixel(x,y,LOW);  }
  else
   {   set_pixel(x,y,HIGH); }

}



//*************************************************************************

void MAX7219_8x8_matrix::matrix(bool a[8][8])
{
  for(uint8_t i=0;i<8;i++)
  {
    for(uint8_t j=0;j<8;j++)
    {
      _BUFFER[i][j] = a[i][7-j];
    }
  }

  for(uint8_t i=0;i<8;i++)
  {
    refresh_row(i);
  }

}

//*************************************************************************

void MAX7219_8x8_matrix::matrix(uint8_t a[8])
{
  for(uint8_t i=0;i<8;i++)
  {
    for(uint8_t j=0;j<8;j++)
    {    
      _BUFFER[i][j] = a[i] & (0x01<<j);
    }
  }

  for(uint8_t i=0;i<8;i++)
  {
    refresh_row(i);
  }

}


//*************************************************************************

void MAX7219_8x8_matrix::mask(uint8_t mask[8], bool value)
{
  for(uint8_t i=0;i<8;i++)
  {
    for(uint8_t j=0;j<8;j++)
    {    
 	if (mask[i]&(0x01<<j))
	 { _BUFFER[i][j] = value; }
    }
  }

  for(uint8_t i=0;i<8;i++)
  {
    refresh_row(i);
  }

}

//*************************************************************************

void MAX7219_8x8_matrix::invert_matrix(void)
{
  for(uint8_t i=0;i<8;i++)
  {
    for(uint8_t j=0;j<8;j++)
    {
 //     _buffer[i+(8*j)] = ~(_buffer[i+(8*j)]);
      _BUFFER[i][j] = !(_BUFFER[i][j]);
    }
  }

  for(uint8_t i=0;i<8;i++)
  {
    refresh_row(i);
  }

}

//*************************************************************************

void MAX7219_8x8_matrix::rotate_matrix_cw()
{
//HIGH: counterclockwise
//LOW:  clockwise

  uint8_t _BUFFER_TEMP [8][8];

  for(uint8_t i=0;i<8;i++)
  {
    for(uint8_t j=0; j<8;j++)
    {
      _BUFFER_TEMP[i][j] = _BUFFER[j][i];
    }
  }


  for(uint8_t i=0;i<8;i++)
   {
       for(uint8_t j=0; j<8;j++)
        { _BUFFER[i][j] = _BUFFER_TEMP[7-i][j];}
   } 


  for(uint8_t num=0;num<8;num++)
    { refresh_row(num); }

}

//*************************************************************************

void MAX7219_8x8_matrix::rotate_matrix_ccw()
{
//HIGH: counterclockwise
//LOW:  clockwise

  uint8_t _BUFFER_TEMP [8][8];

  for(uint8_t i=0;i<8;i++)
  {
    for(uint8_t j=0; j<8;j++)
    {
      _BUFFER_TEMP[i][j] = _BUFFER[j][i];
    }
  }

  for(uint8_t i=0;i<8;i++)
   {
       for(uint8_t j=0; j<8;j++)
        { _BUFFER[i][j] = _BUFFER_TEMP[i][7-j];}
   }    
 


  for(uint8_t num=0;num<8;num++)
    { refresh_row(num); }

}



//*************************************************************************

void MAX7219_8x8_matrix::setBrightness(uint8_t value)
{
  setRegister(REG_INTENSITY, value);
}


//*************************************************************************
// clears screens and buffers

void MAX7219_8x8_matrix::set_char(uint8_t CHAR){
  switch (CHAR) 
   {
    case 0:
      matrix(char_0);     
      break;
    case 1:
      matrix(char_1);     
      break;
    case 2:
      matrix(char_2);     
      break;
    case 3:
      matrix(char_3);     
      break;
    case 4:
      matrix(char_4);     
      break;
    case 5:
      matrix(char_5);     
      break;
    case 6:
      matrix(char_6);     
      break;
    case 7:
      matrix(char_7);     
      break;
    case 8:
      matrix(char_8);     
      break;
    case 9:
      matrix(char_9);     
      break;
    case 45:
      matrix(char_line);     
      break;
    case 46:
      matrix(char_dot);  
    case 48:
      matrix(char_0);     
      break;
    case 49:
      matrix(char_1);     
      break;
    case 50:
      matrix(char_2);     
      break;
    case 51:
      matrix(char_3);     
      break;
    case 52:
      matrix(char_4);     
      break;
    case 53:
      matrix(char_5);     
      break;
    case 54:
      matrix(char_6);     
      break;
    case 55:
      matrix(char_7);     
      break;
    case 56:
      matrix(char_8);     
      break;
    case 57:
      matrix(char_9);     
      break;
    case 65:
      matrix(char_A);
      break;
    case 66:
      matrix(char_B);     
      break;
    case 67:
      matrix(char_C);     
      break;
    case 68:
      matrix(char_D);     
      break;
    case 69:
      matrix(char_E);     
      break;
    case 70:
      matrix(char_F);     
      break;
    case 71:
      matrix(char_G);     
      break;
    case 72:
      matrix(char_H);     
      break;
    case 73:
      matrix(char_I);     
      break;
    case 74:
      matrix(char_J);     
      break;
    case 75:
      matrix(char_K);     
      break;
    case 76:
      matrix(char_L);     
      break;
    case 77:
      matrix(char_M);     
      break;
    case 78:
      matrix(char_N);     
      break;
    case 79:
      matrix(char_O);     
      break;
    case 80:
      matrix(char_P);     
      break;
    case 81:
      matrix(char_Q);     
      break;
    case 82:
      matrix(char_R);     
      break;
    case 83:
      matrix(char_S);     
      break;
    case 84:
      matrix(char_T);     
      break;
    case 85:
      matrix(char_U);     
      break;
    case 86:
      matrix(char_V);     
      break;
    case 87:
      matrix(char_W);     
      break;
    case 88:
      matrix(char_X);     
      break;
    case 89:
      matrix(char_Y);     
      break;
    case 90:
      matrix(char_Z);     
      break;
    case 95:
      matrix(char_underline);     
      break;
//    case '_':
//      matrix(char_underline);     
//      break;
    default: 
      matrix(char_space);
  }

}


//*************************************************************************

void MAX7219_8x8_matrix::square(uint8_t size, bool value)
{
 uint8_t sq8[8]=
 {B11111111,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B11111111};

 uint8_t sq6[8]=
 {B00000000,
  B01111110,
  B01000010,
  B01000010,
  B01000010,
  B01000010,
  B01111110,
  B00000000};

 uint8_t sq4[8]=
 {B00000000,
  B00000000,
  B00111100,
  B00100100,
  B00100100,
  B00111100,
  B00000000,
  B00000000};

 uint8_t sq2[8]=
 {B00000000,
  B00000000,
  B00000000,
  B00011000,
  B00011000,
  B00000000,
  B00000000,
  B00000000};



 switch(size)
 {
    case 8:
 	mask(sq8,value);	
	break;
    case 7:
 	mask(sq8,value);	
 	mask(sq6,value);	
	break;
    case 6:
 	mask(sq6,value);	
	break;
    case 5:
 	mask(sq6,value);	
 	mask(sq4,value);	
	break;
    case 4:
 	mask(sq4,value);	
	break;
    case 3:
 	mask(sq4,value);	
 	mask(sq2,value);	
	break;
    case 2:
 	mask(sq2,value);	
	break; 
    default:
     //do nothing
       break;
  }
}

//*************************************************************************

void MAX7219_8x8_matrix::clear(void)
{
  if (!_BUFFER) return;		//Error

  for(uint8_t i=0;i<8;i++) {
    for(uint8_t j=0;j<8;j++) { 
	 _BUFFER[j][i] = 0x00; }
  }

  for(uint8_t i=0;i<8;i++)
    { refresh_row(i); }
}

//*************************************************************************

void MAX7219_8x8_matrix::fill(void)
{
  if (!_BUFFER) return;		//Error 

  for(uint8_t i=0;i<8;i++)  {
    for(uint8_t j=0;j<8;j++) {
	_BUFFER[j][i] = 0x01;
    }
  }

  for(uint8_t i=0;i<8;i++)
  {  refresh_row(i);  }
}


//*************************************************************************

void MAX7219_8x8_matrix::set_row(uint8_t y, uint8_t value)
{
  for(uint8_t i=0;i<8;i++)
   {
      _BUFFER[y][i] = value & (0x01<<i);
   }


    refresh_row(y);


}


//*************************************************************************
void MAX7219_8x8_matrix::set_col(uint8_t x, uint8_t value)
{
  for(uint8_t j=0;j<8;j++)
   {
     _BUFFER[j][x] = value & (0x01<<j);
   }

  for(uint8_t i=0;i<8;i++)
   {
    refresh_row(i);
   }

}


//*************************************************************************
void MAX7219_8x8_matrix::set_pixel_orbital(uint8_t num, bool value)
{
  struct coord { int y;  
                 int x;};
               
  coord ConvertTable[] =  { {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},
			    {1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},
			    {7,6},{7,5},{7,4},{7,3},{7,2},{7,1},{7,0},
			    {6,0},{5,0},{4,0},{3,0},{2,0},{1,0},
		            {1,1},{1,2},{1,3},{1,4},{1,5},{1,6},
			    {2,6},{3,6},{4,6},{5,6},{6,6},
			    {6,5},{6,4},{6,3},{6,2},{6,1},
			    {5,1},{4,1},{3,1},{2,1},
			    {2,2},{2,3},{2,4},{2,5},
			    {3,5},{4,5},{5,5},
			    {5,4},{5,3},{5,2},
			    {4,2},{3,2},
			    {3,3},{3,4},
			    {4,4},
			    {4,3}
			   };

  uint8_t x = ConvertTable [num].x;
  uint8_t y = ConvertTable [num].y;

  set_pixel(x,y,value);

}



