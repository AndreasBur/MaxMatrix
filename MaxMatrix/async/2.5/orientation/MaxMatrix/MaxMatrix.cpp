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
 #define _MAXMATRIX_SOURCE_

/******************************************************************************************************************************************************
 * INCLUDES
 *****************************************************************************************************************************************************/
#include "MaxMatrix.h"
#if(MAXMATRIX_USE_SPI == STD_ON)
	#include "SPI.h"
#endif
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
	#include "digitalWriteFast.h"
#endif


/******************************************************************************************************************************************************
 * P U B L I C   F U N C T I O N S
 *****************************************************************************************************************************************************/

/******************************************************************************************************************************************************
  CONSTRUCTOR OF MaxMatrix
******************************************************************************************************************************************************/
/*! \brief          MaxMatrix Constructor
 *  \details        Instantiation of the MaxMatrix library
 *
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
    String = NULL;
    SpriteShiftCounter = 0;
	Orientation = MAXMATRIX_MODULE_ORIENTATION_0;
    State = MAXMATRIX_STATE_NONE;
    
    for (int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++) MatrixBuffer[i] = 0;
    for (int i = 0; i < MAXMATRIX_SPRITE_TABLE_NUMBER_OF_COLUMNS; i++) SpriteBuffer[i] = 0;
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
    State = MAXMATRIX_STATE_INIT;
#if(MAXMATRIX_USE_SPI == STD_ON)
	SPI.begin();
	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
#else
    pinMode(DataInPin, OUTPUT);
    pinMode(ClockPin, OUTPUT);
	pinMode(ChipSelectPin, OUTPUT);
# if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
	digitalWriteFast(ClockPin, HIGH);
# else
	digitalWrite(ClockPin, HIGH);
# endif
#endif
    
    RegisterWrite(MAX7219_REG_SCAN_LIMIT_ADDRESS, MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_7);     // display all digits  
    RegisterWrite(MAX7219_REG_DECODE_MODE_ADDRESS, MAX7219_REG_DECODE_MODE_NO_DECODE);              // using an led matrix (not digits)
    RegisterWrite(MAX7219_REG_SHUTDOWN_ADDRESS, MAX7219_REG_SHUTDOWN_MODE_NORMAL_OPERATION);        // normal operation mode
    RegisterWrite(MAX7219_REG_DISPLAY_TEST_ADDRESS, MAX7219_REG_DISPLAY_TEST_NORMAL_OPERATION);     // no display test
    
    /* initialize registers, turn all LEDs off */
    clear();
    
    setIntensity(0x02);    // the first 0x0f is the value you can set
    State = MAXMATRIX_STATE_READY;
} /* init */


/******************************************************************************************************************************************************
  shiftTask()
******************************************************************************************************************************************************/
/*! \brief          shifts char or string over the matrix
 *  \details        this function has to be called periodically until state change from 
 *                  MAXMATRIX_STATE_CHAR_SHIFT or MAXMATRIX_STATE_STRING_SHIFT to MAXMATRIX_STATE_READY
 *  \return         -
 *  \pre            setCharWithShift or setTextWithShift must be called first
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftTask()
{
    if(MAXMATRIX_STATE_STRING_SHIFT == State) stringShiftTask();
    if(MAXMATRIX_STATE_CHAR_SHIFT == State) charShiftTask();
} /* shiftTask */


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
        setColumnOnAllModulesLL(i, 0);
        
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
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
    digitalWriteFast(ChipSelectPin, LOW);
#else
	digitalWrite(ChipSelectPin, LOW);
#endif
    for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++)
    {
#if(MAXMATRIX_USE_SPI == STD_ON)
		SPI.transfer(RegisterAddress);
		SPI.transfer(RegisterValue);
#else
        shiftOut(DataInPin, ClockPin, MSBFIRST, RegisterAddress);
        shiftOut(DataInPin, ClockPin, MSBFIRST, RegisterValue);
#endif
    }
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
    digitalWriteFast(ChipSelectPin, LOW);
    digitalWriteFast(ChipSelectPin, HIGH);
#else
    digitalWrite(ChipSelectPin, LOW);
    digitalWrite(ChipSelectPin, HIGH);
#endif
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
stdReturnType MaxMatrix::getColumn(byte Column, byte* Value)
{
	int Module = Column / MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
    int ModuleColumn = Column % MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
	byte ValueReversed;

	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) return getColumnLL(Column, Value);
	
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) return getRowLL(Module, ModuleColumn, Value);
	
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) {
		if(E_OK == getColumnLL(MAXMATRIX_NUMBER_OF_COLUMNS - Column - 1, &ValueReversed)) {
			*Value = reverseByte(ValueReversed);
			return E_OK;
		} else return E_NOT_OK;
	}
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) {
		if(E_OK == getRowLL(Module, MAXMATRIX_ROW_NUMBER_OF_MODULE - ModuleColumn - 1, &ValueReversed)) {
			*Value = reverseByte(ValueReversed);
			return E_OK;
		} else return E_NOT_OK;
	}
	return E_NOT_OK;
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
stdReturnType MaxMatrix::getColumn(byte Module, byte Column, byte* Value)
{
	return getColumn((Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE) + Column, Value);
} /* setColumn */


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
	int Module = Column / MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
    int ModuleColumn = Column % MAXMATRIX_COLUMN_NUMBER_OF_MODULE;

	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) return setColumnLL(Column, Value);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) return setRowLL(Module, ModuleColumn, Value);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) return setColumnLL(MAXMATRIX_NUMBER_OF_COLUMNS - Column - 1, reverseByte(Value));
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) return setRowLL(Module, MAXMATRIX_ROW_NUMBER_OF_MODULE - ModuleColumn - 1, reverseByte(Value));

	return E_NOT_OK;
} /* setColumn */


/******************************************************************************************************************************************************
  setColumn()
******************************************************************************************************************************************************/
/*! \brief          set a given column on whole matrix
 *  \details        this function sets a column on the LED matrix with the given values
 *
 *  \param[in]      Module      module where column should be set
 *  \param[in]      Column      column on module which contains eight LEDs
 *  \param[in]      Value       led value in column each bit means one led
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setColumn(byte Module, byte Column, byte Value)
{
	return setColumn((Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE) + Column, Value);
} /* setColumn */


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
	rowType ValueReversed;
	
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) return getRowLL(Row, Value);

	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) {
		for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			if(E_NOT_OK == getColumnLL(Module, MAXMATRIX_COLUMN_NUMBER_OF_MODULE - Row - 1, &(*Value)[Module])) return E_NOT_OK;
		}
		return E_OK;
	}
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) {
		if(E_OK == getRowLL(MAXMATRIX_ROW_NUMBER_OF_MODULE - Row - 1, &ValueReversed)) {
			for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
				(*Value)[Module] = reverseByte(ValueReversed[Module]);
			}
			return E_OK;
		} else return E_NOT_OK;
	}
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) {
		for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			if(E_NOT_OK == getColumnLL(Module, Row, &ValueReversed[Module])) return E_NOT_OK;
		}
		for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			(*Value)[Module] = reverseByte(ValueReversed[Module]);
		}
		return E_OK;
	}
	return E_NOT_OK;
} /* getRow */


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
	byte ValueReversed;

	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) return getRowLL(Module, Row, Value);

	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) return getColumnLL(Module, MAXMATRIX_COLUMN_NUMBER_OF_MODULE - Row - 1, Value);

	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) {
		if(E_OK == getRowLL(Module, MAXMATRIX_ROW_NUMBER_OF_MODULE - Row - 1, &ValueReversed)) {
			*Value = reverseByte(ValueReversed);
			return E_OK;
		} else return E_NOT_OK;
	}
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) {
		if(E_OK == getColumnLL(Module, Row, &ValueReversed)) {
			*Value = reverseByte(ValueReversed);
			return E_OK;
		} else E_NOT_OK;
	}
	return E_NOT_OK;
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
	rowType ValueReversed;
	
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) return setRowLL(Row, Value);

	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) {
		for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			if(E_NOT_OK == setColumnLL(Module, MAXMATRIX_COLUMN_NUMBER_OF_MODULE - Row - 1, (*Value)[Module])) return E_NOT_OK;
		}
		return E_OK;
	}
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) {
		for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			ValueReversed[Module] = reverseByte((*Value)[Module]);
		}
		return setRowLL(MAXMATRIX_ROW_NUMBER_OF_MODULE - Row - 1, &ValueReversed);
	}
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) {
		for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			ValueReversed[Module] = reverseByte((*Value)[Module]);
		}
		for(int Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			if(E_NOT_OK == setColumnLL(Module, Row, ValueReversed[Module])) return E_NOT_OK;
		}
		return E_OK;
	}
	return E_NOT_OK;
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
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) return setRowLL(Module, Row, Value);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) return setColumnLL(Module, MAXMATRIX_COLUMN_NUMBER_OF_MODULE - Row - 1, Value); // FALSCH ? 
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) return setRowLL(Module, MAXMATRIX_ROW_NUMBER_OF_MODULE - Row - 1, reverseByte(Value));
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) return setColumnLL(Module, Row, reverseByte(Value)); // FALSCH ?

	return E_NOT_OK;
} /* setRow */


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
	byte ColumnValue;

    if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS && Row >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE) {
		getColumn(Column, &ColumnValue);
        bitWrite(ColumnValue, Row, Value);
		return setColumn(Column, ColumnValue);
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
stdReturnType MaxMatrix::setCharWithShift(char Char)
{
    spriteIndexType SpriteIndex;
    
    if(E_OK == convertCharToSprite(Char, &SpriteIndex)) {
        if(E_OK == getSprite(SpriteIndex, &SpriteBuffer)) {
            SpriteShiftCounter = 1;
            State = MAXMATRIX_STATE_CHAR_SHIFT;
            return E_OK;
        } else return E_NOT_OK;
    } else return E_NOT_OK;
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
stdReturnType MaxMatrix::setTextWithShift(char* sString)
{
    if(sString != NULL) {
        SpriteShiftCounter = 0;
        String = sString;
        State = MAXMATRIX_STATE_STRING_SHIFT;
        return E_OK;
    } else return E_NOT_OK;
} /* setTextWithShift */


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
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) shiftLeftLL(Rotate, FillWithZero, true);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) shiftUpLL(Rotate, true);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) shiftRightLL(Rotate, FillWithZero, true);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) shiftDownLL(Rotate, true);

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
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) shiftRightLL(Rotate, FillWithZero, true);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) shiftDownLL(Rotate, true);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) shiftLeftLL(Rotate, FillWithZero, true);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) shiftUpLL(Rotate, true);
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
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) shiftUpLL(Rotate, false);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) shiftLeftLL(Rotate, true, false);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) shiftDownLL(Rotate, false);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) shiftRightLL(Rotate, true, false);
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
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_0) shiftDownLL(Rotate, false);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_90) shiftRightLL(Rotate, true, false);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_180) shiftUpLL(Rotate, false);
	if(Orientation == MAXMATRIX_MODULE_ORIENTATION_270) shiftLeftLL(Rotate, true, false);
} /* shiftDown */


/******************************************************************************************************************************************************
  getSprite()
******************************************************************************************************************************************************/
/*! \brief          get an sprite from the sprite table
 *  \details        this function returns an sprite by an given sprite index
 *                  
 *  \param[in]      SpriteIndex     index of sprite from sprite table
 *  \param[out]     Sprite          sprite from sprite table (width, height, value column 1 ... 5)
 *  \return         -
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::getSprite(spriteIndexType SpriteIndex, spriteType* Sprite)
{
    if(SpriteIndex >= 0 && SpriteIndex < MAXMATRIX_SPRITE_TABLE_NUMBER_OF_ROWS) {
        memcpy_P(*Sprite, SpriteTable + MAXMATRIX_SPRITE_TABLE_NUMBER_OF_COLUMNS * SpriteIndex, MAXMATRIX_SPRITE_TABLE_NUMBER_OF_COLUMNS);
        return E_OK;
    } else return E_NOT_OK;
} /* getSprite */


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
  setOrientation()
******************************************************************************************************************************************************/
/*! \brief          set Orientation of matrix
 *  \details        this function sets the orientation of the matrix
 *                  known values are 0, 90, 180, 270 degrees
 *
 *  \param[in]      sOrientation	orientation in degree
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::setOrientation(MaxMatrixModuleOrientationType sOrientation)
{
	byte MatrixBufferRotated[MAXMATRIX_BUFFER_SIZE];
	byte ColumnRotated = 0, RowRotated = 0;

	if(sOrientation != Orientation)
	{
		// rotate 90 degree
		if((Orientation == MAXMATRIX_MODULE_ORIENTATION_0   && sOrientation == MAXMATRIX_MODULE_ORIENTATION_90)  ||
		   (Orientation == MAXMATRIX_MODULE_ORIENTATION_90  && sOrientation == MAXMATRIX_MODULE_ORIENTATION_180) ||
		   (Orientation == MAXMATRIX_MODULE_ORIENTATION_180 && sOrientation == MAXMATRIX_MODULE_ORIENTATION_270) ||
		   (Orientation == MAXMATRIX_MODULE_ORIENTATION_270 && sOrientation == MAXMATRIX_MODULE_ORIENTATION_0)) 
		{
			for(byte Module = 1; Module <= MAXMATRIX_NUMBER_OF_MODULES; Module++) {
				for(int Row = MAXMATRIX_ROW_NUMBER_OF_MODULE-1; Row >= 0; Row--) {
					RowRotated = MAXMATRIX_ROW_NUMBER_OF_MODULE-1;
					for(int Column = MAXMATRIX_COLUMN_NUMBER_OF_MODULE*Module-1; Column >= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*(Module-1); Column--) {
						bitWrite(MatrixBufferRotated[ColumnRotated], RowRotated, bitRead(MatrixBuffer[Column], Row));
						RowRotated--;
					}
					ColumnRotated++;
				}
			}
		}
		// rotate 180 degree
		if((Orientation == MAXMATRIX_MODULE_ORIENTATION_0  && sOrientation == MAXMATRIX_MODULE_ORIENTATION_180) ||
		  (Orientation == MAXMATRIX_MODULE_ORIENTATION_90  && sOrientation == MAXMATRIX_MODULE_ORIENTATION_270) ||
		  (Orientation == MAXMATRIX_MODULE_ORIENTATION_180 && sOrientation == MAXMATRIX_MODULE_ORIENTATION_0)   ||
		  (Orientation == MAXMATRIX_MODULE_ORIENTATION_270 && sOrientation == MAXMATRIX_MODULE_ORIENTATION_90))
		{
			for(byte Module = 1; Module <= MAXMATRIX_NUMBER_OF_MODULES; Module++) {
				for(int Column = MAXMATRIX_COLUMN_NUMBER_OF_MODULE*Module-1; Column >= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*(Module-1); Column--) {
					RowRotated = MAXMATRIX_ROW_NUMBER_OF_MODULE-1;
					for(int Row = 0; Row <= MAXMATRIX_ROW_NUMBER_OF_MODULE-1; Row++) {
						bitWrite(MatrixBufferRotated[ColumnRotated], RowRotated, bitRead(MatrixBuffer[Column], Row));
						RowRotated--;
					}
					ColumnRotated++;
				}
			}
		}
		// rotate 270 degree
		if((Orientation == MAXMATRIX_MODULE_ORIENTATION_0  && sOrientation == MAXMATRIX_MODULE_ORIENTATION_270) ||
		  (Orientation == MAXMATRIX_MODULE_ORIENTATION_90  && sOrientation == MAXMATRIX_MODULE_ORIENTATION_0)   ||
		  (Orientation == MAXMATRIX_MODULE_ORIENTATION_180 && sOrientation == MAXMATRIX_MODULE_ORIENTATION_90)  ||
		  (Orientation == MAXMATRIX_MODULE_ORIENTATION_270 && sOrientation == MAXMATRIX_MODULE_ORIENTATION_180))
		{
			for(byte Module = 1; Module <= MAXMATRIX_NUMBER_OF_MODULES; Module++) {
				for(int Row = 0; Row <= MAXMATRIX_ROW_NUMBER_OF_MODULE-1; Row++) {
					RowRotated = MAXMATRIX_ROW_NUMBER_OF_MODULE-1;
					for(int Column = MAXMATRIX_COLUMN_NUMBER_OF_MODULE*(Module-1); Column <= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*Module-1; Column++) {
					   bitWrite(MatrixBufferRotated[ColumnRotated], RowRotated, bitRead(MatrixBuffer[Column], Row));
					   RowRotated--;
					}
					ColumnRotated++;
			    }
			}
		}
		Orientation = sOrientation;
		for(int i = 0; i < MAXMATRIX_BUFFER_SIZE; i++) MatrixBuffer[i] = MatrixBufferRotated[i];
		reload();
	}
} /* setOrientation */


/******************************************************************************************************************************************************
 * P R I V A T E   F U N C T I O N S
 *****************************************************************************************************************************************************/
 
/******************************************************************************************************************************************************
  reverseByte()
******************************************************************************************************************************************************/
/*! \brief          reverse a given byte
 *  \details        this function returns a reversed byte from a given byte
 *                  
 *  \param[in]      Input           byte which should be reversed
 *  \return         reversed Byte
 *****************************************************************************************************************************************************/
byte MaxMatrix::reverseByte(byte Input)
{
   Input = (Input & 0xF0) >> 4 | (Input & 0x0F) << 4;
   Input = (Input & 0xCC) >> 2 | (Input & 0x33) << 2;
   Input = (Input & 0xAA) >> 1 | (Input & 0x55) << 1;
   
   return Input;
} /* reverseByte */


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
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
        digitalWriteFast(ChipSelectPin, LOW);
#else
        digitalWrite(ChipSelectPin, LOW);
#endif
        for(int j = 0; j < MAXMATRIX_NUMBER_OF_MODULES; j++)
        {
#if(MAXMATRIX_USE_SPI == STD_ON)
			SPI.transfer(i + 1);
			SPI.transfer(MatrixBuffer[Column]);
#else
            shiftOut(DataInPin, ClockPin, MSBFIRST, i + 1);
            shiftOut(DataInPin, ClockPin, MSBFIRST, MatrixBuffer[Column]);
#endif
            Column += MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
        }
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
        digitalWriteFast(ChipSelectPin, LOW);
        digitalWriteFast(ChipSelectPin, HIGH);
#else
        digitalWrite(ChipSelectPin, LOW);
        digitalWrite(ChipSelectPin, HIGH);
#endif
    }
} /* reload */


/******************************************************************************************************************************************************
  charShiftTask()
******************************************************************************************************************************************************/
/*! \brief          shifts char one column to the left
 *  \details        this function has to be called periodically until state change from 
 *                  MAXMATRIX_STATE_CHAR_SHIFT or MAXMATRIX_STATE_STRING_SHIFT to MAXMATRIX_STATE_READY
 *  \return         -
 *  \pre            has to be called from shiftTask()
 *****************************************************************************************************************************************************/
void MaxMatrix::charShiftTask()
{
    if(SpriteShiftCounter <= SpriteBuffer[ASCII_TABLE_SPRITE_WIDTH]) {
        shiftLeft(false, true);
        setSprite(MAXMATRIX_NUMBER_OF_COLUMNS-SpriteShiftCounter, 0, &SpriteBuffer);
        SpriteShiftCounter++;
    } else {
        if (MAXMATRIX_STATE_CHAR_SHIFT == State) State = MAXMATRIX_STATE_READY;
        SpriteShiftCounter = 0;
    }
} /* charShiftTask */


/******************************************************************************************************************************************************
  stringShiftTask()
******************************************************************************************************************************************************/
/*! \brief          shifts string one column to the left
 *  \details        this function has to be called periodically until state change from
 *                  MAXMATRIX_STATE_CHAR_SHIFT or MAXMATRIX_STATE_STRING_SHIFT to MAXMATRIX_STATE_READY
 *  \return         -
 *  \pre            has to be called from shiftTask()
 *****************************************************************************************************************************************************/
void MaxMatrix::stringShiftTask()
{
    spriteIndexType SpriteIndex;
    
    if(SpriteShiftCounter == 0)
    {
        if(*String != NULL) {
            convertCharToSprite(*String, &SpriteIndex);
            getSprite(SpriteIndex, &SpriteBuffer);
            if(SpriteShiftCounter != 0) shiftLeft(false, true);
            SpriteShiftCounter = 1;
            charShiftTask();
            String++;
        } else {
            State = MAXMATRIX_STATE_READY;
            shiftLeft(false, true);
        }
    } else {
        charShiftTask();
        if(SpriteShiftCounter == 0) shiftLeft(false, true);
    }
} /* stringShiftTask */


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


/******************************************************************************************************************************************************
 * P R I V A T E   F U N C T I O N S   L O W   L E V E L
 *****************************************************************************************************************************************************/

/******************************************************************************************************************************************************
  getColumnLL()
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
inline stdReturnType MaxMatrix::getColumnLL(byte Column, byte* Value)
{
    if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS) {
		*Value = MatrixBuffer[Column];
		return E_OK;
	} else return E_NOT_OK;
} /* getColumnLL */


/******************************************************************************************************************************************************
  getColumnLL()
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
inline stdReturnType MaxMatrix::getColumnLL(byte Module, byte Column, byte* Value)
{
	if(Module >= 0 && Module < MAXMATRIX_NUMBER_OF_MODULES && Column >= 0 && Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE) {
		*Value = MatrixBuffer[(Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE) + Column];
		return E_OK;
	} else return E_NOT_OK;
} /* getColumnLL */


/******************************************************************************************************************************************************
  getRowLL()
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
stdReturnType MaxMatrix::getRowLL(byte Module, byte Row, byte* Value)
{
	if(Module >= 0 && Module < MAXMATRIX_NUMBER_OF_MODULES && Row >= 0 && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE) {
		for(byte Column = 0; Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
		{
			bitWrite( *Value, MAXMATRIX_COLUMN_NUMBER_OF_MODULE - Column - 1,
			bitRead(MatrixBuffer[Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE + Column], Row));
		}
		return E_OK;
	} else return E_NOT_OK;
} /* getRowLL */


/******************************************************************************************************************************************************
  getRowLL()
******************************************************************************************************************************************************/
/*! \brief          get a given row from Matrix buffer
 *  \details        this function returns a row from the LED matrix
 *
 *  \param[in]      Row         row on matrix which contains eight LEDs
 *  \param[out]     Value       led value in row each bit means one led state
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::getRowLL(byte Row, rowType* Value)
{
	for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) 
		if(E_NOT_OK == getRow(Module, Row, &(*Value)[Module])) return E_NOT_OK;
	return E_OK;
} /* getRowLL */

 
/******************************************************************************************************************************************************
  setColumnLL()
******************************************************************************************************************************************************/
/*! \brief          set a given column on matrix
 *  \details        this function sets a column on the LED matrix with the given values 
 *                  column reaches from zero to number of modules multiplied by eight - 1
 *
 *  \param[in]      Column      column on matrix which contains eight LEDs
 *  \param[in]      Value       led value in column each bit means one led
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setColumnLL(byte Column, byte Value)
{
    if(Column >= 0 && Column < MAXMATRIX_NUMBER_OF_COLUMNS) {
        int Module = Column / MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
        int ModuleColumn = Column % MAXMATRIX_COLUMN_NUMBER_OF_MODULE;
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
        digitalWriteFast(ChipSelectPin, LOW);
#else
		digitalWrite(ChipSelectPin, LOW);
#endif
        for(int i = 0; i < MAXMATRIX_NUMBER_OF_MODULES; i++)
        {
            if (i == Module) {
#if(MAXMATRIX_USE_SPI == STD_ON)
                SPI.transfer(ModuleColumn + 1);
                SPI.transfer(Value);
#else
                shiftOut(DataInPin, ClockPin, MSBFIRST,  ModuleColumn + 1);
                shiftOut(DataInPin, ClockPin, MSBFIRST, Value);
#endif
            } else {
#if(MAXMATRIX_USE_SPI == STD_ON)
                SPI.transfer(MAX7219_REG_NO_OP_ADDRESS);
                SPI.transfer(0);
#else
                shiftOut(DataInPin, ClockPin, MSBFIRST, MAX7219_REG_NO_OP_ADDRESS);
                shiftOut(DataInPin, ClockPin, MSBFIRST, 0);
#endif
            }
        }
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
        digitalWriteFast(ChipSelectPin, LOW);
        digitalWriteFast(ChipSelectPin, HIGH);
#else
        digitalWrite(ChipSelectPin, LOW);
        digitalWrite(ChipSelectPin, HIGH);
#endif
        MatrixBuffer[Column] = Value;
        return E_OK;
    } else {
        return E_NOT_OK;
    }
} /* setColumnLL */


/******************************************************************************************************************************************************
  setColumnLL()
******************************************************************************************************************************************************/
/*! \brief          set a given column on matrix module
 *  \details        this function sets a column on the LED matrix module with the given values
 *                  column reaches from zero to number of columns of module
 *
 *  \param[in]      Module      matrix module where to set column
 *  \param[in]      Column      column on matrix which contains eight LEDs
 *  \param[in]      Value       led value in column each bit means one led
 *  \return         E_OK
 *                  E_NOT_OK
 *****************************************************************************************************************************************************/
stdReturnType MaxMatrix::setColumnLL(byte Module, byte Column, byte Value)
{
	return setColumnLL((Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE) + Column, Value);
} /* setColumnLL */


/******************************************************************************************************************************************************
  setColumnOnAllModulesLL()
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
stdReturnType MaxMatrix::setColumnOnAllModulesLL(byte Column, byte Value)
{
    if(Column >= 0 && Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE) {
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
        digitalWriteFast(ChipSelectPin, LOW);
#else
        digitalWrite(ChipSelectPin, LOW);
#endif
        for(int i = 0; i < MAXMATRIX_NUMBER_OF_MODULES; i++)
        {
#if(MAXMATRIX_USE_SPI == STD_ON)
			SPI.transfer(Column + 1);
			SPI.transfer(Value);
#else
            shiftOut(DataInPin, ClockPin, MSBFIRST, Column + 1);
            shiftOut(DataInPin, ClockPin, MSBFIRST, Value);
#endif
            MatrixBuffer[Column * i] = Value;
        }
#if(MAXMATRIX_USE_DIGITAL_WRITE_FAST == STD_ON)
        digitalWriteFast(ChipSelectPin, LOW);
        digitalWriteFast(ChipSelectPin, HIGH);
#else
        digitalWrite(ChipSelectPin, LOW);
        digitalWrite(ChipSelectPin, HIGH);
#endif
        return E_OK;
    } else {
        return E_NOT_OK;
    }
} /* setColumnOnAllModulesLL */


/******************************************************************************************************************************************************
  setRowLL()
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
stdReturnType MaxMatrix::setRowLL(byte Row, const rowType* Value)
{
	byte BitPos = MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1;
	
	if(Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
	{
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			for(byte Column = Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column < (Module + 1) * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
	        {
	            bitWrite(MatrixBuffer[Column], Row, bitRead((*Value)[Module], BitPos));
				setColumnLL(Column, MatrixBuffer[Column]);
	            BitPos--;
			}
		}
		return E_OK;
	} else {
		return E_NOT_OK;
	}
} /* setRowLL */


/******************************************************************************************************************************************************
  setRowLL()
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
stdReturnType MaxMatrix::setRowLL(byte Module, byte Row, byte Value)
{
	byte BitPos = MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1;
	
	if(Module < MAXMATRIX_NUMBER_OF_MODULES && Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
	{
		for(byte Column = Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column < (Module + 1) * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
	    {
			bitWrite(MatrixBuffer[Column], Row, bitRead(Value, BitPos));
			setColumnLL(Column, MatrixBuffer[Column]);
	        BitPos--;
		}
		return E_OK;
	} else {
		return E_NOT_OK;
	}
} /* setRowLL */


/******************************************************************************************************************************************************
  setRowOnAllModulesLL()
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
stdReturnType MaxMatrix::setRowOnAllModulesLL(byte Row, byte Value)
{
	byte BitPos = MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1;
	
	if(Row < MAXMATRIX_ROW_NUMBER_OF_MODULE)
	{
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			for(byte Column = Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column < (Module + 1) * MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
	        {
	            bitWrite(MatrixBuffer[Column], Row, bitRead(Value, BitPos));
				setColumnOnAllModulesLL(Column, MatrixBuffer[Column]);
	            BitPos--;
			}
		}
		return E_OK;
	} else {
		return E_NOT_OK;
	}
} /* setRowOnAllModulesLL */


/******************************************************************************************************************************************************
  shiftLeftLL()
******************************************************************************************************************************************************/
/*! \brief          shift the whole matrix led states one column to the left
 *  \details        this functions shifts the states of all LEDs on the matrix one column to the left.
 *                  By activating fill with zero, the column on the right side will be initialized with LED off.
 *                  By activating rotation, the last column will be first.
 *
 *  \param[in]      Rotate                      activate rotation of the matrix LED states
 *  \param[in]      FillWithZero                initialize the column on the right side with zero.
 *  \param[in]      ShiftToNeighbourModule      shift led states to neighbour module
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftLeftLL(bool Rotate, bool FillWithZero, bool ShiftToNeighbourModule)
{
	if(ShiftToNeighbourModule) {
		byte old = MatrixBuffer[0];
		for(int i = 0; i < MAXMATRIX_NUMBER_OF_COLUMNS; i++) MatrixBuffer[i] = MatrixBuffer[i + 1];
		if(Rotate) MatrixBuffer[MAXMATRIX_NUMBER_OF_COLUMNS - 1] = old;
		else if(FillWithZero) MatrixBuffer[MAXMATRIX_NUMBER_OF_COLUMNS - 1] = 0;
	} else {
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			byte old = MatrixBuffer[MAXMATRIX_COLUMN_NUMBER_OF_MODULE * Module];
			for(int i = 0; i < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; i++)
				MatrixBuffer[i + (MAXMATRIX_COLUMN_NUMBER_OF_MODULE * Module)] = MatrixBuffer[i + (MAXMATRIX_COLUMN_NUMBER_OF_MODULE * Module) + 1];
			if(Rotate) MatrixBuffer[MAXMATRIX_COLUMN_NUMBER_OF_MODULE * (Module + 1) - 1] = old;
			else if(FillWithZero) MatrixBuffer[Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1] = 0;
		}
	}
    reload();
} /* shiftLeftLL */


/******************************************************************************************************************************************************
  shiftRightLL()
******************************************************************************************************************************************************/
/*! \brief          shift the whole matrix led states one column to the right
 *  \details        this functions shifts the states of all LEDs on the matrix one column to the right.
 *                  By activating fill with zero, the column on the left side will be initialized with LED off.
 *                  By activating rotation, the last column will be first.
 *
 *  \param[in]      Rotate                      activate rotation of the matrix LED states
 *  \param[in]      FillWithZero                initialize the column on the left side with zero.
 *  \param[in]      ShiftToNeighbourModule      shift led states to neighbour module
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftRightLL(bool Rotate, bool FillWithZero, bool ShiftToNeighbourModule)
{
	if(ShiftToNeighbourModule) {
	    byte old = MatrixBuffer[MAXMATRIX_NUMBER_OF_COLUMNS - 1];
	    for(int i = MAXMATRIX_NUMBER_OF_COLUMNS - 1; i > 0; i--) MatrixBuffer[i] = MatrixBuffer[i-1];
	    if(Rotate) MatrixBuffer[0] = old;
	    else if(FillWithZero) MatrixBuffer[0] = 0;
	} else {
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++) {
			byte old = MatrixBuffer[MAXMATRIX_COLUMN_NUMBER_OF_MODULE * (Module + 1) - 1];
			for(int i = MAXMATRIX_COLUMN_NUMBER_OF_MODULE - 1; i > 0; i--) 
				MatrixBuffer[i + (MAXMATRIX_COLUMN_NUMBER_OF_MODULE * Module)] = MatrixBuffer[i + (MAXMATRIX_COLUMN_NUMBER_OF_MODULE * Module) - 1];
			if(Rotate) MatrixBuffer[Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE] = old;
			else if(FillWithZero) MatrixBuffer[Module * MAXMATRIX_COLUMN_NUMBER_OF_MODULE] = 0;
		}
	}
    reload();
} /* shiftRightLL */


/******************************************************************************************************************************************************
  shiftUpLL()
******************************************************************************************************************************************************/
/*! \brief          shift the whole matrix led states one row up to the top
 *  \details        this function shifts the states of all LEDS on the matrix one row up to the top.
 *                  The row on the bottom will be initialized with LED off.
 *                  By activating rotation, the last row will be first.
 
 *  \param[in]      Rotate                      activate rotation of the matrix LED states
 *  \param[in]      ShiftToNeighbourModule      shift led states to neighbour module
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftUpLL(bool Rotate, bool ShiftToNeighbourModule)
{	
	byte ModuleTopRow = 0;
	byte MatrixTopRow = 0;

	if(ShiftToNeighbourModule) {
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++)
		{
			if(Module == 0 && Rotate) getRowLL(0, 0, &MatrixTopRow);
			getRowLL(Module + 1, 0, &ModuleTopRow);
			
			for(int Column = 0; Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++) 
				MatrixBuffer[Column + (MAXMATRIX_COLUMN_NUMBER_OF_MODULE * Module)] >>= 1;
			
			if(Module == MAXMATRIX_NUMBER_OF_MODULES - 1) {
				if(Rotate) setRowLL(Module, MAXMATRIX_ROW_NUMBER_OF_MODULE - 1, MatrixTopRow);
			} else {
				setRowLL(Module, MAXMATRIX_ROW_NUMBER_OF_MODULE - 1, ModuleTopRow);
			}
		}
	} else {
		for(int Column = 0; Column < MAXMATRIX_NUMBER_OF_COLUMNS; Column++) 
		{
			bool TopBit = bitRead(MatrixBuffer[Column], 0);
			MatrixBuffer[Column] >>= 1;
			if(Rotate) bitWrite(MatrixBuffer[Column], MAXMATRIX_ROW_NUMBER_OF_MODULE - 1, TopBit);
		}		
	}
    reload();
} /* shiftModuleUpLL */


/******************************************************************************************************************************************************
  shiftDownLL()
******************************************************************************************************************************************************/
/*! \brief          shift the module or whole matrix led states one row down to the bottom
 *  \details        this function shifts the states of all LEDS on the matrix or on the module one row down to the bottom.
 *                  The row on the top will be initialized with LED off. By activating rotation, the last row will be first.
 *                  
 *  \param[in]      Rotate                      activate rotation of the matrix LED states
 *  \param[in]      ShiftToNeighbourModule      shift led states to neighbour module
 *  \return         -
 *****************************************************************************************************************************************************/
void MaxMatrix::shiftDownLL(bool Rotate, bool ShiftToNeighbourModule)
{
	byte ModuleBottomRow = 0;
	byte MatrixBottomRow = 0;

	if(ShiftToNeighbourModule) {
		for(byte Module = 0; Module < MAXMATRIX_NUMBER_OF_MODULES; Module++)
		{
			if(Module == 0 && Rotate) getRowLL(0, MAXMATRIX_ROW_NUMBER_OF_MODULE - 1, &MatrixBottomRow);
			getRowLL(Module + 1, MAXMATRIX_ROW_NUMBER_OF_MODULE - 1, &ModuleBottomRow);
			
			for(int Column = 0; Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++)
			MatrixBuffer[Column + (MAXMATRIX_COLUMN_NUMBER_OF_MODULE * Module)] <<= 1;
			
			if(Module == MAXMATRIX_NUMBER_OF_MODULES - 1) {
				if(Rotate) setRowLL(Module, 0, MatrixBottomRow);
			} else {
				setRowLL(Module, 0, ModuleBottomRow);
			}
		}
	} else {
	    for(int Column = 0; Column < MAXMATRIX_NUMBER_OF_COLUMNS; Column++) 
		{
		    bool BottomBit = bitRead(MatrixBuffer[Column], MAXMATRIX_ROW_NUMBER_OF_MODULE - 1);
		    MatrixBuffer[Column] <<= 1;
		    if(Rotate) bitWrite(MatrixBuffer[Column], 0, BottomBit);
		}
	}
    reload();
} /* shiftDownLL */

/******************************************************************************************************************************************************
 *  E N D   O F   F I L E
 *****************************************************************************************************************************************************/
 