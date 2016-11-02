/******************************************************************************************************************************************************
 *  COPYRIGHT
 *  ---------------------------------------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) Andreas Burnickl                                                                                                 All rights reserved.
 *
 *  \endverbatim
 *  ---------------------------------------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------------------------------------*/
/**     \file       MaxMatrix.c
 *      \brief      Main file of MaxMatrix library
 *
 *      \details    Arduino library to drive 8x8 LED display Matrix modules with MAX7219
 *                  
 *
 *****************************************************************************************************************************************************/

#define MAXMATRIX_SOURCE

/******************************************************************************************************************************************************
 * INCLUDES
 *****************************************************************************************************************************************************/
#include "MaxMatrix.h"


/******************************************************************************************************************************************************
  CONSTRUCTOR OF MaxMatrix
 *****************************************************************************************************************************************************/
/*! \brief          MaxMatrix Constructor
 *  \details        Instantiation of the MaxMatrix library
 *  \param[in]      sDataInPin         number of the Data Input Pin
 *  \param[in]      sChipSelectPin     number of the Chip Select (CS) Pin
 *  \param[in]      sClockPin          number of the Clock Pin
 *  \return         -
 *****************************************************************************************************************************************************/
MaxMatrix::MaxMatrix(byte sDataInPin, byte sChipSelectPin, byte sClockPin) 
{
	DataInPin = sDataInPin;
	ChipSelectPin = sChipSelectPin;
	ClockPin = sClockPin;
	
	for (int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++) MatrixBuffer[i] = 0;
} /* MaxMatrix */


/******************************************************************************************************************************************************
  DESTRUCTOR OF MaxMatrix
******************************************************************************************************************************************************/
MaxMatrix::~MaxMatrix()
{

} /* ~MaxMatrix */


/******************************************************************************************************************************************************
  init()
******************************************************************************************************************************************************/
/*! \brief          initialization of the Matrix Hardware
 *  \details        this function initializes the IO Hardware and the MAX7219 Controller
 *                  all LEDs of the matrix will be cleared and intensity will be set
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::init()
{
	pinMode(DataInPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(ChipSelectPin, OUTPUT);
	digitalWrite(ClockPin, HIGH); 

    RegisterWrite(MAX7219_REG_SCAN_LIMIT_ADDRESS, MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_7);     // display all digits  
    RegisterWrite(MAX7219_REG_DECODE_MODE_ADDRESS, MAX7219_REG_DECODE_MODE_NO_DECODE);              // using an led matrix (not digits)
    RegisterWrite(MAX7219_REG_SHUTDOWN_ADDRESS, MAX7219_REG_SHUTDOWN_MODE_NORMAL_OPERATION);        // normal operation mode
    RegisterWrite(MAX7219_REG_DISPLAY_TEST_ADDRESS, MAX7219_REG_DISPLAY_TEST_NORMAL_OPERATION);     // no display test
    
    /* initialize registers, turn all LEDs off */
    clear();
    
    setIntensity(0x04);    // the first 0x0f is the value you can set
} /* init */


/******************************************************************************************************************************************************
  setIntensity()
******************************************************************************************************************************************************/
/*! \brief          set led intensity of matrix module
 *  \details        
 *                  
 *  \param[in]      Intensity       intensity value from 0 to 15
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setIntensity(byte Intensity)
{
    if(Intensity >= MAX7219_REG_INTENSITY_MIN_VALUE && Intensity <= MAX7219_REG_INTENSITY_MAX_VALUE) {
        RegisterWrite(MAX7219_REG_INTENSITY_ADDRESS, Intensity);
        return E_OK;
    } else {
        return E_NOT_OK; 
    }
} /* setIntensity */


/******************************************************************************************************************************************************
  clear()
******************************************************************************************************************************************************/
/*! \brief          clears all LEDs of matrix module
 *  \details        
 *                  
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::clear()
{
    for(int i = 0; i < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; i++)
        setColumnOnAllModules(i, 0);
        
    for(int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++)
        MatrixBuffer[i] = 0;
} /* clear */


/******************************************************************************************************************************************************
  RegisterWrite()
******************************************************************************************************************************************************/
/*! \brief          send SPI command to MAX7219
 *  \details        
 *                  
 *  \param[in]      RegisterAddress     Address of the MAX7219 Register
 *  \param[in]      Value               Register Value
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::RegisterWrite(byte RegisterAddress, byte RegisterValue)
{
    digitalWrite(ChipSelectPin, LOW);
    for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++)
    {
        shiftOut(DataInPin, ClockPin, MSBFIRST, RegisterAddress);
        shiftOut(DataInPin, ClockPin, MSBFIRST, RegisterValue);
    }
    digitalWrite(ChipSelectPin, LOW);
    digitalWrite(ChipSelectPin, HIGH);
} /* RegisterWrite */


/******************************************************************************************************************************************************
  getColumn()
******************************************************************************************************************************************************/
/*! \brief          get a given column from Matrix buffer
 *  \details        this function returns a column from the LED matrix
 *                  column reaches from zero to number of modules multiplied by eight
 *
 *  \param[in]      Column      column on matrix which contains eight LEDs
 *  \param[out]     Value       led value in column each bit means one led state
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
inline stdReturnType MaxMatrix::getColumn(byte Column, byte* Value)
{
    if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS) {
		*Value = MatrixBuffer[Column];
		return E_OK;
	} else return E_NOT_OK;
} /* getColumn */


/******************************************************************************************************************************************************
  getColumn()
******************************************************************************************************************************************************/
/*! \brief          get a given column from Matrix buffer
 *  \details        this function returns a column from the LED matrix
 *
 *  \param[in]      Module      module where column will be returned
 *  \param[in]      Column      column on module which contains eight LEDs
 *  \param[out]     Value       led value in column each bit means one led
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
inline stdReturnType MaxMatrix::getColumn(byte Module, byte Column, byte* Value)
{
	if(Module >= 0 && Module < MAXMATRIX_NUMBER_OF_MODULES && Column >= 0 && Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE) {
		*Value = MatrixBuffer[(Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE) + Column];
		return E_OK;
	} else return E_NOT_OK;
} /* getColumn */


/******************************************************************************************************************************************************
  setColumn()
******************************************************************************************************************************************************/
/*! \brief          set a given column on whole matrix
 *  \details        this function sets a column on the LED matrix with the given values
 *                  column reaches from zero to number of modules multiplied by eight
 *
 *  \param[in]      Column      column on matrix which contains eight LEDs
 *  \param[in]      Value       led value in column each bit means one led state
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setColumn(byte Column, byte Value)
{
    if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS) {
        int Module = Column / MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
        int ModuleColumn = Column % MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
        digitalWrite(ChipSelectPin, LOW);
        for(int i = 0; i < MAXMATRIX_NUMBER_OF_MODULES; i++)
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
        MatrixBuffer[Column] = Value;
        return E_OK;
    } else {
        return E_NOT_OK;
    }
} /* setColumn */


/******************************************************************************************************************************************************
  setColumnOnAllModules()
******************************************************************************************************************************************************/
/*! \brief          set a given column on all matrix modules
 *  \details        this function sets a column on all LED matrix modules with the given values
 *                  column reaches from zero to column number of a module
 *
 *  \param[in]      Column      column on matrix which contains eight LEDs
 *  \param[in]      Value       led value in column each bit means one LED
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setColumnOnAllModules(byte Column, byte Value)
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

} /* setColumnOnAllModules */


/******************************************************************************************************************************************************
  getRow()
******************************************************************************************************************************************************/
/*! \brief          get a given row from Matrix buffer
 *  \details        this function returns a row from the LED matrix
 *
 *  \param[in]      Module      module where Row should be get
 *  \param[in]      Row         row on matrix which contains eight LEDs
 *  \param[out]     Value       led value in row each bit means one led state
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::getRow(byte Module, byte Row, byte* Value)
{
	if(Module >= 0 && Module < MAXMATRIX_NUMBER_OF_MODULES && Column >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE) {
		for(byte Column = 0; Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
		{
			bitWrite( *Value, MAXMATRIX_COLUMN_NUMBER_OF_MODULE - Column - 1,
			bitRead(MatrixBuffer[Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE + Column], Row));
		}
		return E_OK;
	} else return E_NOT_OK;
} /* getRowLL */


/******************************************************************************************************************************************************
  getRow()
******************************************************************************************************************************************************/
/*! \brief          get a given row from Matrix buffer
 *  \details        this function returns a row from the LED matrix
 *
 *  \param[in]      Row         row on matrix which contains eight LEDs
 *  \param[out]     Value       led value in row each bit means one led state
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::getRow(byte Row, rowType* Value)
{
	for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) 
		if(E_NOT_OK == getRow(Module, Row, &(*Value)[Module])) return E_NOT_OK;
	return E_OK;
} /* getRow */


/******************************************************************************************************************************************************
  setRow()
******************************************************************************************************************************************************/
/*! \brief          set a given column on whole matrix
 *  \details        this function sets a column on the LED matrix with the given values
 *                  column reaches from zero to number of modules multiplied by eight
 *
 *  \param[in]      Row      	row on matrix which contains eight LEDs
 *  \param[in]      Value       led value in row each bit means one led
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setRow(byte Row, const rowType* Value)
{
	byte BitPos = MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1;
	
	if(Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
	{
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			for(byte Column = Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column < (Module + 1) * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
	        {
	            bitWrite(MatrixBuffer[Column], Row, bitRead((*Value)[Module], BitPos));
				setColumn(Column, MatrixBuffer[Column]);
	            BitPos--;
			}
		}
		return E_OK;
	} else {
		return E_NOT_OK;
	}
} /* setRow */


/******************************************************************************************************************************************************
  setRow()
******************************************************************************************************************************************************/
/*! \brief          set a given column on whole matrix
 *  \details        this function sets a column on the LED matrix with the given values
 *
 *  \param[in]      Module      Module where row should be set
 *  \param[in]      Column      column on matrix which contains eight LEDs
 *  \param[in]      Value       led value in column each bit means one led
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setRow(byte Module, byte Row, byte Value)
{
	byte BitPos = MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1;
	
	if(Module < MAXMATRIX_NUMBER_OF_MODULES && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
	{
		for(byte Column = Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column < (Module + 1) * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
	    {
			bitWrite(MatrixBuffer[Column], Row, bitRead(Value, BitPos));
			setColumn(Column, MatrixBuffer[Column]);
	        BitPos--;
		}
		return E_OK;
	} else {
		return E_NOT_OK;
	}
} /* setRow */


/******************************************************************************************************************************************************
  setRowOnAllModules()
******************************************************************************************************************************************************/
/*! \brief          set a given row on all matrix modules
 *  \details        this function sets a row on all LED matrix modules with the given values
 *                  row reaches from zero to row number of a module
 *
 *  \param[in]      Row         row on matrix which contains eight LEDs
 *  \param[in]      Value       led value in column each bit means one LED
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setRowOnAllModules(byte Row, byte Value)
{
	byte BitPos = MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1;
	
	if(Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
	{
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			for(byte Column = Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column < (Module + 1) * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
	        {
	            bitWrite(MatrixBuffer[Column], Row, bitRead(Value, BitPos));
				setColumnOnAllModules(Column, MatrixBuffer[Column]);
	            BitPos--;
			}
		}
		return E_OK;
	} else {
		return E_NOT_OK;
	}
} /* setRowOnAllModules */


/******************************************************************************************************************************************************
  getDot()
******************************************************************************************************************************************************/
/*! \brief          get a Pixel (LED) on the matrix
 *  \details        
 *                  
 *  \param[in]      Column		column where to get the pixel
 *  \param[in]      Row			row where to get the pixel
 *  \param[out]     Value		value of the pixel   
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::getDot(byte Column, byte Row, bool* Value)
{
	byte ColumnValue;

    if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS && Row >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE) {
		getColumn(Column, &ColumnValue);
		*Value = bitRead(ColumnValue, Row);
		return E_OK;
    } else {
        return E_NOT_OK;
    }
} /* getDot */


/******************************************************************************************************************************************************
  setDot()
******************************************************************************************************************************************************/
/*! \brief          set a Pixel (LED) on the matrix
 *  \details        
 *                  
 *  \param[in]      Column		column where to set the pixel
 *  \param[in]      Row			row where to set the pixel
 *  \param[in]      Value		value of the pixel
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setDot(byte Column, byte Row, bool Value)
{
    if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS && Row >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE) {
        bitWrite(MatrixBuffer[Column], Row, Value);
		return setColumn(Column, MatrixBuffer[Column]);
    } else {
        return E_NOT_OK;
    }
} /* setDot */


/******************************************************************************************************************************************************
  setChar()
******************************************************************************************************************************************************/
/*! \brief          prints a char on the matrix
 *  \details        
 *                  
 *  \param[in]      X           x coordinate
 *  \param[in]      Y           y coordinate
 *  \param[in]      Char        char to print on matrix
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setChar(int X, int Y, char Char)
{   
    spriteIndexType SpriteIndex;
    spriteType Sprite;
    
    if(E_OK == convertCharToSprite(Char, &SpriteIndex)) {
        if(E_OK == getSprite(SpriteIndex, &Sprite)) {
            setSprite(X, Y, &Sprite);
            return E_OK;
        } else return E_NOT_OK;
    } else return E_NOT_OK;
} /* setChar */


/******************************************************************************************************************************************************
  setCharWithShift()
******************************************************************************************************************************************************/
/*! \brief          shifts a char into the matrix and stops on the right side
 *  \details        this function shifts a given char into the matrix and stops shifting on the right side.
 *                  Every shift process will be triggered by calling the shiftMainFunction. 
 *                  
 *  \param[in]      Char        char to print on matrix
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setCharWithShift(char Char, int ShiftSpeed)
{
	if(Char >= MAXMATRIX_ASCII_CHAR_MIN && Char <= MAXMATRIX_ASCII_CHAR_MAX) {
		Char += MAXMATRIX_ASCII_TABLE_OFFSET;
		byte SpriteBuffer[MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS];
		memcpy_P(SpriteBuffer, AsciiTable + MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS * Char, MAXMATRIX_ASCII_TABLE_NUMBER_OF_COLUMNS);

		for (int i = 1; i <= SpriteBuffer[ASCII_TABLE_SPRITE_WIDTH]; i++)
		{
			delay(ShiftSpeed);
			shiftLeft(false, false);
			writeSprite(MAXMATRIX_NUMBER_OF_COLUMNS-i, 0, SpriteBuffer);
		}
	}
} /* setCharWithShift */


/******************************************************************************************************************************************************
  setText()
******************************************************************************************************************************************************/
/*! \brief          prints a string on the matrix
 *  \details        
 *                  
 *  \param[in]      String        string to print on matrix
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setText(const char* String)
{   
	spriteIndexType SpriteIndex;
	spriteType Sprite;
	int CharColumn = 0;
	
	while(*String != NULL)
	{
		if(E_NOT_OK == convertCharToSprite(*String, &SpriteIndex)) { return E_NOT_OK; }
		else if(E_NOT_OK == getSprite(SpriteIndex, &Sprite)) { return E_NOT_OK; }
        else {
			setSprite(CharColumn, 0, &Sprite);
			CharColumn += Sprite[ASCII_TABLE_SPRITE_WIDTH] + 1;
			if(CharColumn > MAXMATRIX_NUMBER_OF_COLUMNS) break;
		}
		String++;
	}
	return E_OK;
} /* setText */


/******************************************************************************************************************************************************
  setTextWithShift()
******************************************************************************************************************************************************/
/*! \brief          shifts a string into the matrix and stops after the last char on the right side
 *  \details        this function shifts a given string into the matrix and stops shifting on the right side,
 *                  when the last char appears on the matrix.
 *
 *  \param[in]      sString     string to print on matrix
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setTextWithShift(char* String, int ShiftSpeed)
{
	while (*String != 0)
	{
		printCharWithShift(*String, ShiftSpeed);
		shiftLeft(false, true);
		String++;
	}
}


/******************************************************************************************************************************************************
  shiftLeft()
******************************************************************************************************************************************************/
/*! \brief          shift the whole matrix led states one column to the left
 *  \details        this functions shifts the states of all LEDs on the matrix one column to the left.
 *                  By activating fill with zero, the column on the right side will be initialized with LED off.
 *                  By activating rotation, the last column will be first.
 *
 *  \param[in]      Rotate          activate rotation of the matrix LED states
 *  \param[in]      FillWithZero    initialize the column on the right side with zero.
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftLeft(bool Rotate, bool FillWithZero)
{
    byte old = MatrixBuffer[0];
    for(int Column = 0; Column < MAXMATRIX_NUMBER_OF_COLUMNS; Column++)
        MatrixBuffer[Column] = MatrixBuffer[Column + 1];
    if(Rotate) MatrixBuffer[MAXMATRIX_NUMBER_OF_COLUMNS - 1] = old;
    else if(FillWithZero) MatrixBuffer[MAXMATRIX_NUMBER_OF_COLUMNS - 1] = 0;
    
    reload();
} /* shiftLeft */


/******************************************************************************************************************************************************
  shiftRight()
******************************************************************************************************************************************************/
/*! \brief          shift the whole matrix led states one column to the right
 *  \details        this functions shifts the states of all LEDs on the matrix one column to the right.
 *                  By activating fill with zero, the column on the left side will be initialized with LED off.
 *                  By activating rotation, the last column will be first.
 *
 *  \param[in]      Rotate          activate rotation of the matrix LED states
 *  \param[in]      FillWithZero    initialize the column on the right side with zero.
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftRight(bool Rotate, bool FillWithZero)
{
    byte old = MatrixBuffer[MAXMATRIX_NUMBER_OF_COLUMNS - 1];
    for(int Column = MAXMATRIX_NUMBER_OF_COLUMNS - 1; Column > 0; Column--)
        MatrixBuffer[Column] = MatrixBuffer[Column - 1];
    if(Rotate) MatrixBuffer[0] = old;
    else if(FillWithZero) MatrixBuffer[0] = 0;
    
    reload();
} /* shiftRight */


/******************************************************************************************************************************************************
  shiftUp()
******************************************************************************************************************************************************/
/*! \brief          shift the whole matrix led states one row up to the top
 *  \details        this function shifts the states of all LEDS on the matrix one row up to the top.
 *                  The row on the bottom will be initialized with LED off.
 *                  By activating rotation, the last row will be first.
 
 *  \param[in]      Rotate          activate rotation of the matrix LED states
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftUp(bool Rotate)
{
	for(int Column = 0; Column < MAXMATRIX_NUMBER_OF_COLUMNS; Column++) 
    {
        bool TopBit = bitRead(MatrixBuffer[Column], 0);
        MatrixBuffer[Column] >>= 1;
        if(Rotate) bitWrite(MatrixBuffer[Column], MAXMATRIX_ROW_NUMBER_OF_MODULE - 1, TopBit);
    }
    reload();
} /* shiftUp */


/******************************************************************************************************************************************************
  shiftDown()
******************************************************************************************************************************************************/
/*! \brief          shift the whole matrix led states one row down to the bottom
 *  \details        this function shifts the states of all LEDS on the matrix one row down to the bottom.
 *                  The row on the top will be initialized with LED off.
 *                  By activating rotation, the last row will be first.
 
 *  \param[in]      Rotate          activate rotation of the matrix LED states
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftDown(bool Rotate)
{
	for(int Column = 0; Column < MAXMATRIX_NUMBER_OF_COLUMNS; Column++) 
    {
		bool BottomBit = bitRead(MatrixBuffer[Column], MAXMATRIX_ROW_NUMBER_OF_MODULE - 1);
        MatrixBuffer[Column] <<= 1;
        if(Rotate) bitWrite(MatrixBuffer[Column], 0, BottomBit);
    }
    reload();
} /* shiftDown */


/******************************************************************************************************************************************************
  setSprite()
******************************************************************************************************************************************************/
/*! \brief          print sprite on matrix
 *  \details        this function prints an sprite on the matrix located to the given coordinates
 *                  
 *  \param[in]      X           x coordinate
 *  \param[in]      Y           y coordinate
 *  \param[in]      Sprite      sprite from sprite table (width, height, value column 1 ... 5)
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::setSprite(int X, int Y, const spriteType* Sprite)
{
    int SpriteWidth = (*Sprite)[ASCII_TABLE_SPRITE_WIDTH];
    int SpriteHeight = (*Sprite)[ASCII_TABLE_SPRITE_HEIGHT];
    
    if(SpriteHeight == MAXMATRIX_ROW_NUMBER_OF_MODULE && Y == 0) {
        for(int i = 0; i < SpriteWidth; i++)
        {
            int Column = X + i;
            if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS)
                setColumn(Column, (*Sprite)[i + ASCII_TABLE_SPRITE_COLUMN1]);
        } 
    } else {
        for(int i = 0; i < SpriteWidth; i++)
        {
            for(int j = 0; j < SpriteHeight; j++)
            {
                int Column = X + i;
                int Row = Y + j;
                if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS && Row >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
                    setDot(Column, Row, bitRead((*Sprite)[i + ASCII_TABLE_SPRITE_COLUMN1], j));
            }
        }
    }
} /* setSprite */


/******************************************************************************************************************************************************
 * P R I V A T E   F U N C T I O N S
 *****************************************************************************************************************************************************/

/******************************************************************************************************************************************************
  reload()
******************************************************************************************************************************************************/
/*! \brief          reload whole matrix
 *  \details        this function reloads the whole matrix with the matrix buffer values
 *                  
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::reload()
{
    for(int i = 0; i < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; i++)
    {
        int Column = i;
        digitalWrite(ChipSelectPin, LOW);
        for(int j = 0; j < MAXMATRIX_NUMBER_OF_MODULES; j++)
        {
            shiftOut(DataInPin, ClockPin, MSBFIRST, i + 1);
            shiftOut(DataInPin, ClockPin, MSBFIRST, MatrixBuffer[Column]);
            Column += MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
        }
        digitalWrite(ChipSelectPin, LOW);
        digitalWrite(ChipSelectPin, HIGH);
    }
} /* reload */


/******************************************************************************************************************************************************
  convertCharToSprite()
******************************************************************************************************************************************************/
/*! \brief          converts char to sprite
 *  \details        this function converts the ASCII char to sprite index
 *                  
 *  \param[in]      Char            char to convert to sprite
 *  \param[out]     SpriteIndex     appropriate sprite
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::convertCharToSprite(char Char, spriteIndexType* SpriteIndex)
{
    stdReturnType ReturnValue = E_NOT_OK;

    if('Ä' == Char) { *SpriteIndex = 95; ReturnValue = E_OK; }
    else if('Ö' == Char) { *SpriteIndex = 96; ReturnValue = E_OK; }
    else if('Ü' == Char) { *SpriteIndex = 97; ReturnValue = E_OK; }
    else if('ä' == Char) { *SpriteIndex = 98; ReturnValue = E_OK; }
    else if('ö' == Char) { *SpriteIndex = 99; ReturnValue = E_OK; }
    else if('ü' == Char) { *SpriteIndex = 100; ReturnValue = E_OK; }
    else if(Char >= MAXMATRIX_ASCII_CHAR_MIN && Char <= MAXMATRIX_ASCII_CHAR_MAX)
        { *SpriteIndex = Char + MAXMATRIX_ASCII_TABLE_OFFSET; ReturnValue = E_OK; }

    return ReturnValue;
} /* convertCharToSprite */