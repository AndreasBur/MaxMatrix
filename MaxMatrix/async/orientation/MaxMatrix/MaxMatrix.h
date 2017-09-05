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
/**     \file       MaxMatrix.h
 *      \brief      Main header file of MaxMatrix library
 *
 *      \details    Arduino library to drive 8x8 LED display Matrix modules with MAX7219
 *                  
 *
 *****************************************************************************************************************************************************/
#ifndef _MAXMATRIX_H_
#define _MAXMATRIX_H_

/******************************************************************************************************************************************************
 * INCLUDES
 *****************************************************************************************************************************************************/
#include "Arduino.h"
#include "StandardTypes.h"


/******************************************************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *****************************************************************************************************************************************************/
/* MaxMatrix Configuration Parameter */
#define MAXMATRIX_NUMBER_OF_MODULES                                 4
#define MAXMATRIX_USE_SPI                                           STD_OFF
#define MAXMATRIX_USE_DIGITAL_WRITE_FAST                            STD_OFF
#define MAXMATRIX_SPACE_BETWEEN_CHARS_INIT_VALUE                    1
#define MAXMATRIX_INTENSITY_INIT_VALUE                              5

/* MaxMatrix Parameter */
#define MAXMATRIX_COLUMN_NUMBER_OF_MODULE                           8
#define MAXMATRIX_ROW_NUMBER_OF_MODULE                              8
#define MAXMATRIX_NUMBER_OF_COLUMNS                                 (MAXMATRIX_NUMBER_OF_MODULES * MAXMATRIX_COLUMN_NUMBER_OF_MODULE)
#define MAXMATRIX_SPRITE_TABLE_NUMBER_OF_COLUMNS                    7
#define MAXMATRIX_SPRITE_TABLE_NUMBER_OF_ROWS                       (sizeof(SpriteTable) / MAXMATRIX_SPRITE_TABLE_NUMBER_OF_COLUMNS)
#define MAXMATRIX_ASCII_TABLE_OFFSET                                -32
#define MAXMATRIX_ASCII_CHAR_MIN                                    32
#define MAXMATRIX_ASCII_CHAR_MAX                                    126
#define MAXMATRIX_ASCII_CHAR_MAX_WIDTH                              5
#define MAXMATRIX_BUFFER_SIZE                                       MAXMATRIX_NUMBER_OF_COLUMNS

/* Max7219 Register Address Table */
#define MAX7219_REG_NO_OP_ADDRESS                                   0x00
#define MAX7219_REG_DIGIT0_ADDRESS                                  0x01
#define MAX7219_REG_DIGIT1_ADDRESS                                  0x02
#define MAX7219_REG_DIGIT2_ADDRESS                                  0x03
#define MAX7219_REG_DIGIT3_ADDRESS                                  0x04
#define MAX7219_REG_DIGIT4_ADDRESS                                  0x05
#define MAX7219_REG_DIGIT5_ADDRESS                                  0x06
#define MAX7219_REG_DIGIT6_ADDRESS                                  0x07
#define MAX7219_REG_DIGIT7_ADDRESS                                  0x08
#define MAX7219_REG_DECODE_MODE_ADDRESS                             0x09
#define MAX7219_REG_INTENSITY_ADDRESS                               0x0A
#define MAX7219_REG_SCAN_LIMIT_ADDRESS                              0x0B
#define MAX7219_REG_SHUTDOWN_ADDRESS                                0x0C
#define MAX7219_REG_DISPLAY_TEST_ADDRESS                            0x0F

/* Content of Max7219 Decode Mode Register */
#define MAX7219_REG_DECODE_MODE_NO_DECODE                           0x00
#define MAX7219_REG_DECODE_MODE_CODE_B_DECODE_DIGIT_0               0x01
#define MAX7219_REG_DECODE_MODE_CODE_B_DECODE_DIGIT_3_TO_0          0x0F
#define MAX7219_REG_DECODE_MODE_CODE_B_DECODE_DIGIT_7_TO_0          0xFF

/* Content of Max7219 Intensity Register */
#define MAX7219_REG_INTENSITY_MIN_VALUE                             0x00
#define MAX7219_REG_INTENSITY_MAX_VALUE                             0x0F

/* Content of Max7219 Scan Limit Register */
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0                      0x00
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_1                 0x01
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_2                 0x02
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_3                 0x03
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_4                 0x04
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_5                 0x05
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_6                 0x06
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_7                 0x07

/* Content of Max7219 Shutdown Register */
#define MAX7219_REG_SHUTDOWN_MODE_SHUTDOWN_MODE                     0x00
#define MAX7219_REG_SHUTDOWN_MODE_NORMAL_OPERATION                  0x01

/* Content of Max7219 Display Test Register */
#define MAX7219_REG_DISPLAY_TEST_NORMAL_OPERATION                   0x00
#define MAX7219_REG_DISPLAY_TEST_DISPLAY_TEST_MODE                  0x01


/******************************************************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *****************************************************************************************************************************************************/


/******************************************************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *****************************************************************************************************************************************************/
/* Type which describes the internal state of the MaxMatrix */
typedef enum {
    MAXMATRIX_STATE_NONE,
    MAXMATRIX_STATE_INIT,
    MAXMATRIX_STATE_READY,
    MAXMATRIX_STATE_STRING_SHIFT,
    MAXMATRIX_STATE_CHAR_SHIFT
} MaxMatrixStateType;

/* Type which describes the internal state of the MaxMatrix shift task */
typedef enum {
    MAXMATRIX_SPRITE_SHIFT_STATE_READY,
    MAXMATRIX_SPRITE_SHIFT_STATE_RUNNING
} MaxMatrixShiftStateType;

/* Type which describes the orientation of the Matrix */
/*
typedef enum {
    MAXMATRIX_MATRIX_ORIENTATION_0,
    MAXMATRIX_MATRIX_ORIENTATION_180
} MaxMatrixMatrixOrientationType;
*/

/* Type which describes the orientation of the Modules */
typedef enum {
    MAXMATRIX_MODULE_ORIENTATION_0,
    MAXMATRIX_MODULE_ORIENTATION_90,
    MAXMATRIX_MODULE_ORIENTATION_180,
    MAXMATRIX_MODULE_ORIENTATION_270
} MaxMatrixModuleOrientationType;

/* sprite types */
typedef byte spriteIndexType;

/* sprite array type */
typedef byte spriteType[MAXMATRIX_SPRITE_TABLE_NUMBER_OF_COLUMNS];

/* row array type */
typedef byte rowType[MAXMATRIX_NUMBER_OF_MODULES];

/* types for sprite table columns */
enum {
    ASCII_TABLE_SPRITE_WIDTH,
    ASCII_TABLE_SPRITE_HEIGHT,
    ASCII_TABLE_SPRITE_COLUMN1,
    ASCII_TABLE_SPRITE_COLUMN2,
    ASCII_TABLE_SPRITE_COLUMN3,
    ASCII_TABLE_SPRITE_COLUMN4,
    ASCII_TABLE_SPRITE_COLUMN5
};

/* sprite table */
const char SpriteTable[] PROGMEM = {
  //width,  height, Column1,    Column2,    Column 3,   Column 4,   Column 5,
    3,      8,      B00000000,  B00000000,  B00000000,  B00000000,  B00000000, // space
    1,      8,      B01011111,  B00000000,  B00000000,  B00000000,  B00000000, // !
    3,      8,      B00000011,  B00000000,  B00000011,  B00000000,  B00000000, // "
    5,      8,      B00010100,  B00111110,  B00010100,  B00111110,  B00010100, // #
    4,      8,      B00100100,  B01101010,  B00101011,  B00010010,  B00000000, // $
    5,      8,      B01100011,  B00010011,  B00001000,  B01100100,  B01100011, // %
    5,      8,      B00110110,  B01001001,  B01010110,  B00100000,  B01010000, // &
    1,      8,      B00000011,  B00000000,  B00000000,  B00000000,  B00000000, // '
    3,      8,      B00011100,  B00100010,  B01000001,  B00000000,  B00000000, // (
    3,      8,      B01000001,  B00100010,  B00011100,  B00000000,  B00000000, // )
    5,      8,      B00101000,  B00011000,  B00001110,  B00011000,  B00101000, // *
    5,      8,      B00001000,  B00001000,  B00111110,  B00001000,  B00001000, // +
    2,      8,      B10110000,  B01110000,  B00000000,  B00000000,  B00000000, // ,
    4,      8,      B00001000,  B00001000,  B00001000,  B00001000,  B00000000, // -
    2,      8,      B01100000,  B01100000,  B00000000,  B00000000,  B00000000, // .
    4,      8,      B01100000,  B00011000,  B00000110,  B00000001,  B00000000, // /
    4,      8,      B00111110,  B01000001,  B01000001,  B00111110,  B00000000, // 0
    3,      8,      B01000010,  B01111111,  B01000000,  B00000000,  B00000000, // 1
    4,      8,      B01100010,  B01010001,  B01001001,  B01000110,  B00000000, // 2
    4,      8,      B00100010,  B01000001,  B01001001,  B00110110,  B00000000, // 3
    4,      8,      B00011000,  B00010100,  B00010010,  B01111111,  B00000000, // 4
    4,      8,      B00100111,  B01000101,  B01000101,  B00111001,  B00000000, // 5
    4,      8,      B00111110,  B01001001,  B01001001,  B00110000,  B00000000, // 6
    4,      8,      B01100001,  B00010001,  B00001001,  B00000111,  B00000000, // 7
    4,      8,      B00110110,  B01001001,  B01001001,  B00110110,  B00000000, // 8
    4,      8,      B00000110,  B01001001,  B01001001,  B00111110,  B00000000, // 9
    2,      8,      B01010000,  B00000000,  B00000000,  B00000000,  B00000000, // :
    2,      8,      B10000000,  B01010000,  B00000000,  B00000000,  B00000000, // ;
    3,      8,      B00010000,  B00101000,  B01000100,  B00000000,  B00000000, // <
    3,      8,      B00010100,  B00010100,  B00010100,  B00000000,  B00000000, // =
    3,      8,      B01000100,  B00101000,  B00010000,  B00000000,  B00000000, // >
    4,      8,      B00000010,  B01011001,  B00001001,  B00000110,  B00000000, // ?
    5,      8,      B00111110,  B01001001,  B01010101,  B01011101,  B00001110, // @
    4,      8,      B01111110,  B00010001,  B00010001,  B01111110,  B00000000, // A
    4,      8,      B01111111,  B01001001,  B01001001,  B00110110,  B00000000, // B
    4,      8,      B00111110,  B01000001,  B01000001,  B00100010,  B00000000, // C
    4,      8,      B01111111,  B01000001,  B01000001,  B00111110,  B00000000, // D
    4,      8,      B01111111,  B01001001,  B01001001,  B01000001,  B00000000, // E
    4,      8,      B01111111,  B00001001,  B00001001,  B00000001,  B00000000, // F
    4,      8,      B00111110,  B01000001,  B01001001,  B01111010,  B00000000, // G
    4,      8,      B01111111,  B00001000,  B00001000,  B01111111,  B00000000, // H
    3,      8,      B01000001,  B01111111,  B01000001,  B00000000,  B00000000, // I
    4,      8,      B00110000,  B01000000,  B01000001,  B00111111,  B00000000, // J
    4,      8,      B01111111,  B00001000,  B00010100,  B01100011,  B00000000, // K
    4,      8,      B01111111,  B01000000,  B01000000,  B01000000,  B00000000, // L
    5,      8,      B01111111,  B00000010,  B00001100,  B00000010,  B01111111, // M
    5,      8,      B01111111,  B00000100,  B00001000,  B00010000,  B01111111, // N
    4,      8,      B00111110,  B01000001,  B01000001,  B00111110,  B00000000, // O
    4,      8,      B01111111,  B00001001,  B00001001,  B00000110,  B00000000, // P
    4,      8,      B00111110,  B01000001,  B01000001,  B10111110,  B00000000, // Q
    4,      8,      B01111111,  B00001001,  B00001001,  B01110110,  B00000000, // R
    4,      8,      B01000110,  B01001001,  B01001001,  B00110010,  B00000000, // S
    5,      8,      B00000001,  B00000001,  B01111111,  B00000001,  B00000001, // T
    4,      8,      B00111111,  B01000000,  B01000000,  B00111111,  B00000000, // U
    5,      8,      B00001111,  B00110000,  B01000000,  B00110000,  B00001111, // V
    5,      8,      B00111111,  B01000000,  B00111000,  B01000000,  B00111111, // W
    5,      8,      B01100011,  B00010100,  B00001000,  B00010100,  B01100011, // X
    5,      8,      B00000111,  B00001000,  B01110000,  B00001000,  B00000111, // Y
    4,      8,      B01100001,  B01010001,  B01001001,  B01000111,  B00000000, // Z
    2,      8,      B01111111,  B01000001,  B00000000,  B00000000,  B00000000, // [
    4,      8,      B00000001,  B00000110,  B00011000,  B01100000,  B00000000, // \ backslash
    2,      8,      B01000001,  B01111111,  B00000000,  B00000000,  B00000000, // ]
    3,      8,      B00000010,  B00000001,  B00000010,  B00000000,  B00000000, // ^
    4,      8,      B01000000,  B01000000,  B01000000,  B01000000,  B00000000, // _
    2,      8,      B00000001,  B00000010,  B00000000,  B00000000,  B00000000, // `
    4,      8,      B00100000,  B01010100,  B01010100,  B01111000,  B00000000, // a
    4,      8,      B01111111,  B01000100,  B01000100,  B00111000,  B00000000, // b
    4,      8,      B00111000,  B01000100,  B01000100,  B00101000,  B00000000, // c
    4,      8,      B00111000,  B01000100,  B01000100,  B01111111,  B00000000, // d
    4,      8,      B00111000,  B01010100,  B01010100,  B00011000,  B00000000, // e
    3,      8,      B00000100,  B01111110,  B00000101,  B00000000,  B00000000, // f
    4,      8,      B10011000,  B10100100,  B10100100,  B01111000,  B00000000, // g
    4,      8,      B01111111,  B00000100,  B00000100,  B01111000,  B00000000, // h
    3,      8,      B01000100,  B01111101,  B01000000,  B00000000,  B00000000, // i
    4,      8,      B01000000,  B10000000,  B10000100,  B01111101,  B00000000, // j
    4,      8,      B01111111,  B00010000,  B00101000,  B01000100,  B00000000, // k
    3,      8,      B01000001,  B01111111,  B01000000,  B00000000,  B00000000, // l
    5,      8,      B01111100,  B00000100,  B01111100,  B00000100,  B01111000, // m
    4,      8,      B01111100,  B00000100,  B00000100,  B01111000,  B00000000, // n
    4,      8,      B00111000,  B01000100,  B01000100,  B00111000,  B00000000, // o
    4,      8,      B11111100,  B00100100,  B00100100,  B00011000,  B00000000, // p
    4,      8,      B00011000,  B00100100,  B00100100,  B11111100,  B00000000, // q
    4,      8,      B01111100,  B00001000,  B00000100,  B00000100,  B00000000, // r
    4,      8,      B01001000,  B01010100,  B01010100,  B00100100,  B00000000, // s
    3,      8,      B00000100,  B00111111,  B01000100,  B00000000,  B00000000, // t
    4,      8,      B00111100,  B01000000,  B01000000,  B01111100,  B00000000, // u
    5,      8,      B00011100,  B00100000,  B01000000,  B00100000,  B00011100, // v
    5,      8,      B00111100,  B01000000,  B00111100,  B01000000,  B00111100, // w
    5,      8,      B01000100,  B00101000,  B00010000,  B00101000,  B01000100, // x
    4,      8,      B10011100,  B10100000,  B10100000,  B01111100,  B00000000, // y
    3,      8,      B01100100,  B01010100,  B01001100,  B00000000,  B00000000, // z
    3,      8,      B00001000,  B00110110,  B01000001,  B00000000,  B00000000, // {
    1,      8,      B01111111,  B00000000,  B00000000,  B00000000,  B00000000, // |
    3,      8,      B01000001,  B00110110,  B00001000,  B00000000,  B00000000, // }
    4,      8,      B00001000,  B00000100,  B00001000,  B00000100,  B00000000, // ~
    4,      8,      B01111100,  B00010011,  B00010011,  B01111100,  B00000000, // Ä
    4,      8,      B00111100,  B01000011,  B01000011,  B00111100,  B00000000, // Ö
    4,      8,      B00111110,  B01000001,  B01000001,  B00111110,  B00000000, // Ü
    4,      8,      B00100000,  B01010110,  B01010110,  B01111000,  B00000000, // ä
    4,      8,      B00111000,  B01000110,  B01000110,  B00111000,  B00000000, // ö
    4,      8,      B00111100,  B01000010,  B01000010,  B01111100,  B00000000, // ü
};


/******************************************************************************************************************************************************
 *  CLASS  MaxMatrix
 *****************************************************************************************************************************************************/
class MaxMatrix
{
  private:
    byte DataInPin;
    byte ChipSelectPin;
    byte ClockPin;
    MaxMatrixStateType State;
    MaxMatrixModuleOrientationType Orientation;
    byte MatrixBuffer[MAXMATRIX_BUFFER_SIZE];
    spriteType SpriteBuffer;
    byte SpriteShiftCounter;
    byte SpaceBetweenChars;
    char* String;

    // functions
    void reload();
    void charShiftTask();
    void stringShiftTask();
    stdReturnType convertCharToSprite(char Char, spriteIndexType*);
    byte reverseByte(byte);


    // Low Level get functions
    stdReturnType getColumnLL(byte, byte*);
    stdReturnType getColumnLL(byte, byte, byte*);
    stdReturnType getRowLL(byte, rowType);
    stdReturnType getRowLL(byte, byte, byte*);

    
    // Low Level set functions
    stdReturnType setColumnLL(byte, byte);
    stdReturnType setColumnLL(byte, byte, byte);
    stdReturnType setColumnOnAllModulesLL(byte, byte);
    stdReturnType setRowLL(byte, const rowType);
    stdReturnType setRowLL(byte, byte, byte);
    stdReturnType setRowOnAllModulesLL(byte, byte);


    // Low Level functions
    void shiftLeftLL(bool = false, bool = true, bool = false);
    void shiftRightLL(bool = false, bool = true, bool = false);
    void shiftUpLL(bool = false, bool = false);
    void shiftDownLL(bool = false, bool = false);

  public:
    MaxMatrix(byte, byte, byte);
    ~MaxMatrix();


    // get methods
    MaxMatrixStateType getState() { return State; }
    byte getDataInPin() { return DataInPin; }
    byte getChipSelectPin() { return ChipSelectPin; }
    byte getClockPin() { return ClockPin; }
    MaxMatrixModuleOrientationType getOrientation() { return Orientation; }
    byte getSpaceBetweenChars() { return SpaceBetweenChars; }

    stdReturnType getColumn(byte, byte*);
    stdReturnType getColumn(byte, byte, byte*);
    stdReturnType getRow(byte, rowType);
    stdReturnType getRow(byte, byte, byte*);
    stdReturnType getDot(byte, byte, bool*);
    stdReturnType getSprite(spriteIndexType, spriteType*);


    // set methods
    void setDataInPin(byte sDataInPin) { DataInPin = sDataInPin; }
    void setChipSelectPin(byte sChipSelectPin) { ChipSelectPin = sChipSelectPin; }
    void setClockPin(byte sClockPin) { ClockPin = sClockPin; }
    stdReturnType setIntensity(byte);
    stdReturnType setColumn(byte, byte);
    stdReturnType setColumn(byte, byte, byte);
    stdReturnType setRow(byte, const rowType);
    stdReturnType setRow(byte, byte, byte);
    stdReturnType setDot(byte, byte, bool);
    stdReturnType setChar(int, int, char);
    stdReturnType setCharWithShift(char);
    stdReturnType setText(const char*);
    stdReturnType setTextWithShift(char*);
    stdReturnType setSpaceBetweenChars(byte);
    void setSprite(int, int, const spriteType);
    void setModuleOrientation(MaxMatrixModuleOrientationType);


    // methods
    void init();
    void shiftTask();
    void clear();
    void RegisterWrite(byte, byte);

    void shiftLeft(bool = false, bool = true);
    void shiftRight(bool = false, bool = true);
    void shiftUp(bool = false);
    void shiftDown(bool = false);
};

#endif

/******************************************************************************************************************************************************
 *  E N D   O F   F I L E
 *****************************************************************************************************************************************************/