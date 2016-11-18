/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) Andreas Burnickl                                                                 All rights reserved.
 *
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  MaxMatrix.c
 *        \brief  Main file of MaxMatrix library
 *
 *      \details  
 *                  
 *                  
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Arduino.h"
#include "MaxMatrix.h"


/**********************************************************************************************************************
  CONSTRUCTOR OF MaxMatrix
**********************************************************************************************************************/
/*! \brief          MaxMatrix Constructor
 *  \details        Instantiation of the MaxMatrix library
 *  \param[in]      sDataInPin         number of the Data Input Pin
 *  \param[in]      sChipSelectPin     number of the Chip Select (CS) Pin
 *  \param[in]      sClockPin          number of the Clock Pin
 *  \param[in]      sNumberOfModules   number of the used Matrix modules
 *  \return         -
 *********************************************************************************************************************/
MaxMatrix::MaxMatrix(byte sDataInPin, byte sChipSelectPin, byte sClockPin, byte sNumberOfModules) 
{
	DataInPin = sDataInPin;
	ChipSelectPin = sChipSelectPin;
	ClockPin = sClockPin;
	NumberOfModules = sNumberOfModules;
	for (int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++)
		buffer[i] = 0;
} /* MaxMatrix */


/**********************************************************************************************************************
  DESTRUCTOR OF MaxMatrix
**********************************************************************************************************************/
MaxMatrix::~MaxMatrix()
{

} /* ~MaxMatrix */


void MaxMatrix::init()
{
	pinMode(DataInPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(ChipSelectPin, OUTPUT);
	digitalWrite(ClockPin, HIGH); 

	setCommand(MAX7219_REG_SCAN_LIMIT_ADDRESS, MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_7);    // display all digits  
	setCommand(MAX7219_REG_DECODE_MODE_ADDRESS, MAX7219_REG_DECODE_MODE_NO_DECODE);				// using an led matrix (not digits)
	setCommand(MAX7219_REG_SHUTDOWN_ADDRESS, MAX7219_REG_SHUTDOWN_MODE_NORMAL_OPERATION);		// not in shutdown mode
	setCommand(MAX7219_REG_DISPLAY_TEST_ADDRESS, MAX7219_REG_DISPLAY_TEST_NORMAL_OPERATION);	// no display test
	
	// empty registers, turn all LEDs off
	clear();
	
	setIntensity(0x04);    // the first 0x0f is the value you can set
}


stdReturnType MaxMatrix::setIntensity(byte Intensity)
{
	if(Intensity >= MAX7219_REG_INTENSITY_MIN_VALUE && Intensity <= MAX7219_REG_INTENSITY_MAX_VALUE) {
		setCommand(MAX7219_REG_INTENSITY_ADDRESS, Intensity);
		return E_OK;
	} else {
		return E_NOT_OK;
	}
}


void MaxMatrix::clear()
{
	for(int i = 0; i < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; i++)
		setColumnAll(i,0);
		
	for(int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++)
		buffer[i] = 0;
}


void MaxMatrix::setCommand(byte Command, byte Value)
{
	digitalWrite(ChipSelectPin, LOW);
	for(int i = 0; i<NumberOfModules; i++)
	{
		shiftOut(DataInPin, ClockPin, MSBFIRST, Command);
		shiftOut(DataInPin, ClockPin, MSBFIRST, Value);
	}
	digitalWrite(ChipSelectPin, LOW);
	digitalWrite(ChipSelectPin, HIGH);
}


stdReturnType MaxMatrix::setColumn(byte Column, byte Value)
{
	if(Column >= 0 && Column <= MAXMATRIX_NUMBER_OF_COLUMNS) {
		int Module = Column / MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
		int ModuleColumn = Column % MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
		digitalWrite(ChipSelectPin, LOW);
		for(int i = 0; i < NumberOfModules; i++)
		{
			if (i == Module) {
				shiftOut(DataInPin, ClockPin, MSBFIRST,  ModuleColumn + 1);
				shiftOut(DataInPin, ClockPin, MSBFIRST, Value);
				} else {
				shiftOut(DataInPin, ClockPin, MSBFIRST, 0);
				shiftOut(DataInPin, ClockPin, MSBFIRST, 0);
			}
		}
		digitalWrite(ChipSelectPin, LOW);
		digitalWrite(ChipSelectPin, HIGH);
		buffer[Column] = Value;
		return E_OK;
	} else {
		return E_NOT_OK;
	}
}


stdReturnType MaxMatrix::setColumnAll(byte Column, byte Value)
{
	if(Column >= 0 && Column <= MAXMATRIX_COLUMN_NUMBER_OF_MODULE) {
		digitalWrite(ChipSelectPin, LOW);
		for(int i = 0; i < NumberOfModules; i++)
		{
			shiftOut(DataInPin, ClockPin, MSBFIRST, Column + 1);
			shiftOut(DataInPin, ClockPin, MSBFIRST, Value);
			buffer[Column * i] = Value;
		}
		digitalWrite(ChipSelectPin, LOW);
		digitalWrite(ChipSelectPin, HIGH);
		return E_OK;
	} else {
		return E_NOT_OK;
	}

}


stdReturnType MaxMatrix::setDot(byte Column, byte Row, byte Value)
{
	if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS && Row >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE) {
	    bitWrite(buffer[Column], Row, Value);

	    int Module = Column / MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
	    int ModuleColumn = Column % MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
	    digitalWrite(ChipSelectPin, LOW);
	    for(int i = 0; i < NumberOfModules; i++)
	    {
		    if (i == Module) {
			    shiftOut(DataInPin, ClockPin, MSBFIRST, ModuleColumn + 1);
			    shiftOut(DataInPin, ClockPin, MSBFIRST, buffer[Column]);
			    } else {
			    shiftOut(DataInPin, ClockPin, MSBFIRST, 0);
			    shiftOut(DataInPin, ClockPin, MSBFIRST, 0);
		    }
	    }
	    digitalWrite(ChipSelectPin, LOW);
	    digitalWrite(ChipSelectPin, HIGH);
		return E_OK;
	} else {
		return E_NOT_OK;
	}
}


void MaxMatrix::printChar(int X, int Y, char Char)
{
	if(Char >= MAXMATRIX_ASCII_CHAR_MIN && Char <= MAXMATRIX_ASCII_CHAR_MAX) {
		Char += MAXMATRIX_ASCII_TABLE_OFFSET;
		byte SpriteBuffer[MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS];
		memcpy_P(SpriteBuffer, AsciiTable + MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS * Char, MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS);
		writeSprite(X, Y, SpriteBuffer);
	}
}


void MaxMatrix::printCharWithShift(char Char, int ShiftSpeed)
{
	if(Char >= MAXMATRIX_ASCII_CHAR_MIN && Char <= MAXMATRIX_ASCII_CHAR_MAX) {
		Char += MAXMATRIX_ASCII_TABLE_OFFSET;
		byte SpriteBuffer[MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS];
		memcpy_P(SpriteBuffer, AsciiTable + MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS * Char, MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS);

		for (int i=1; i <= SpriteBuffer[ASCII_TABLE_SPRITE_WIDTH]; i++)
		{
			delay(ShiftSpeed);
			shiftLeft(false, false);
			writeSprite(MAXMATRIX_NUMBER_OF_COLUMNS-i, 0, SpriteBuffer);
		}
	}
}


void MaxMatrix::printStringWithShift(char* String, int ShiftSpeed)
{
	while (*String != 0)
	{
		printCharWithShift(*String, ShiftSpeed);
		shiftLeft(false, true);
		String++;
	}
}


void MaxMatrix::shiftLeft(bool rotate, bool fill_zero)
{
	byte old = buffer[0];
	for(int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++)
		buffer[i] = buffer[i+1];
	if(rotate) buffer[MAXMATRIX_NUMBER_OF_COLUMNS-1] = old;
	else if(fill_zero) buffer[MAXMATRIX_NUMBER_OF_COLUMNS-1] = 0;
	
	reload();
}


void MaxMatrix::shiftRight(bool Rotate, bool fill_zero)
{
	int LastColumn = MAXMATRIX_NUMBER_OF_COLUMNS-1;
	byte old = buffer[LastColumn];
	for(int i = MAXMATRIX_NUMBER_OF_COLUMNS-1; i > 0; i--)
		buffer[i] = buffer[i-1];
	if(Rotate) buffer[0] = old;
	else if(fill_zero) buffer[0] = 0;
	
	reload();
}


void MaxMatrix::shiftUp(bool Rotate)
{
	for(int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++)
	{
		bool b = buffer[i] & 1;
		buffer[i] >>= 1;
		if(Rotate) bitWrite(buffer[i], 7, b);
	}
	reload();
}


void MaxMatrix::shiftDown(bool Rotate)
{
	for(int i=0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++)
	{
		bool b = buffer[i] & 128;
		buffer[i] <<= 1;
		if(Rotate) bitWrite(buffer[i], 0, b);
	}
	reload();
}


void MaxMatrix::writeSprite(int X, int Y, const byte* Sprite)
{
	int SpriteWidth = Sprite[ASCII_TABLE_SPRITE_WIDTH];
	int SpriteHeight = Sprite[ASCII_TABLE_SPRITE_HEIGHT];
	
	if(SpriteHeight == MAXMATRIX_ROW_NUMBER_OF_MODULE && Y == 0) {
		for(int i = 0; i < SpriteWidth; i++)
		{
			int Column = X + i;
			if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS)
			setColumn(Column, Sprite[i + ASCII_TABLE_SPRITE_COLUMN1]);
		} 
	} else {
		for(int i = 0; i < SpriteWidth; i++)
		{
			for(int j = 0; j < SpriteHeight; j++)
			{
				int Column = X + i;
				int Row = Y + j;
				if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS && Row >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
				setDot(Column, Row, bitRead(Sprite[i + ASCII_TABLE_SPRITE_COLUMN1], j));
			}
		}
	}
}


void MaxMatrix::reload()
{
	for(int i = 0; i < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; i++)
	{
		int Column = i;
		digitalWrite(ChipSelectPin, LOW);
		for(int j = 0; j < NumberOfModules; j++)
		{
			shiftOut(DataInPin, ClockPin, MSBFIRST, i + 1);
			shiftOut(DataInPin, ClockPin, MSBFIRST, buffer[Column]);
			Column += MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
		}
		digitalWrite(ChipSelectPin, LOW);
		digitalWrite(ChipSelectPin, HIGH);
	}
}