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
/**     \file       LedMatrix_1_module.ino
 *      \brief      
 *
 *      \details    
 *                  
 *
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <MaxMatrix.h>
#include <avr/pgmspace.h>
#include <TimerOne.h>


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define UART_INPUT_LENGTH                   40
#define COMMAND_MAX_LENGTH                  10
#define MAXMATRIX_MAX_TEXT_LENGTH           (UART_INPUT_LENGTH - COMMAND_MAX_LENGTH)

#define MAXMATRIX_DATA_PIN                  12
#define MAXMATRIX_CS_PIN                    10
#define MAXMATRIX_CLOCK_PIN                 11

#define MAXMATRIX_SPEED_MIN_VALUE           1
#define MAXMATRIX_SPEED_MAX_VALUE           100

#define MAXMATRIX_INTENSITY_MIN_VALUE       1
#define MAXMATRIX_INTENSITY_MAX_VALUE       16

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
char UartInput[UART_INPUT_LENGTH];                  // buffer for incoming UART data
boolean UartInputComplete;                          // flag UART string is complete
char MaxMatrixText[MAXMATRIX_MAX_TEXT_LENGTH];      // current Matrix Text
bool Timer1Overflow;                                // flag Timer1 overflow
MaxMatrix Matrix(MAXMATRIX_DATA_PIN, MAXMATRIX_CS_PIN, MAXMATRIX_CLOCK_PIN);


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
void uartInputHandle();
void serialEvent();
void printHelp();
void stringCopy(char*, const char*, int);
void systTick();


/**********************************************************************************************************************
 *  ARDUINO setup
 *********************************************************************************************************************/
void setup()
{
    Timer1.initialize(200000);
    Timer1.attachInterrupt(systTick);
    Matrix.init();
    stringCopy(MaxMatrixText, "Andreas Burnickl", sizeof(MaxMatrixText));
    UartInputComplete = false;
    Timer1Overflow = false;
    Serial.begin(9600);
}


/**********************************************************************************************************************
 *  ARDUINO loop
 *********************************************************************************************************************/
void loop()
{ 
    if (UartInputComplete)
    {
        uartInputHandle();
        // clear the string:
        UartInput[0] = '\0';
        UartInputComplete = false;
    }

    if(Timer1Overflow)
    {
        if(Matrix.getState() == MAXMATRIX_STATE_STRING_SHIFT)
            Matrix.shiftTask();
        if(Matrix.getState() == MAXMATRIX_STATE_READY)
        {
            Matrix.setTextWithShift(MaxMatrixText);
        }
        Timer1Overflow = false;
    }
}


/**********************************************************************************************************************
  uartInputHandle()
**********************************************************************************************************************/
/*! \brief          handles the incoming UART data
 *  \details        this function handles and validates the incoming data. After splitting the data into
 *                  command and value a specific action will be called.
 *                 
 *********************************************************************************************************************/
void uartInputHandle()
{
    char command[COMMAND_MAX_LENGTH] = "";
    char valueASCII[MAXMATRIX_MAX_TEXT_LENGTH] = "";
    int value = 0;
    const char delimiter[] = "=";
    char *token = NULL;

    if(strchr(UartInput, '='))
    {
        token = strtok(UartInput, delimiter);
        if(token != NULL) stringCopy(command, token, sizeof(command));
        token = strtok(NULL, delimiter);
        if(token != NULL) stringCopy(valueASCII, token, sizeof(valueASCII));

        if(strcmp(command, "text") == 0) {
            stringCopy(MaxMatrixText, valueASCII, sizeof(MaxMatrixText));
            Matrix.clear();
            Matrix.setTextWithShift(MaxMatrixText);
        }
        else if(strcmp(command, "intensity") == 0) {
            value = atoi(valueASCII);
            if(value >= MAXMATRIX_INTENSITY_MIN_VALUE && value <= MAXMATRIX_INTENSITY_MAX_VALUE) {
                Matrix.setIntensity(value-1);
            } else {
                Serial.println(F("value out of range"));
                Serial.println(F("intensity range 1 ... 16"));
            }
        }
        else if(strcmp(command, "speed") == 0) {
            value = atoi(valueASCII);
            if(value >= MAXMATRIX_SPEED_MIN_VALUE && value <= MAXMATRIX_SPEED_MAX_VALUE) {
                Timer1.setPeriod((MAXMATRIX_SPEED_MAX_VALUE-value+1) * 10000);
            } else {
                Serial.println(F("value out of range"));
                Serial.println(F("speed range 1 ... 10"));
            }
        }
        else { 
            printHelp(); 
        }
    } else { 
        printHelp(); 
    }
}


/**********************************************************************************************************************
  serialEvent()
**********************************************************************************************************************/
/*! \brief          save incoming UART byte into UART buffer
 *  \details        this function saves incoming UART signs into UART buffer.
 *                  After receiving new line, string is complete and flag will be set.
 *  \pre            will be called at the end of the main function             
 *********************************************************************************************************************/
void serialEvent() 
{
    while (Serial.available()) 
    {
        // get the new byte from uart
        char inChar = (char) Serial.read();
        // if the incoming character is a newline, set a flag
        if (inChar == '\n') {
            UartInputComplete = true;
        } else {
            // check for free space in buffer
            int UartInputLength = strlen(UartInput);
            if(UartInputLength < UART_INPUT_LENGTH-1)
            {
                // save new char
                UartInput[UartInputLength] = inChar;
                UartInput[UartInputLength+1] = '\0';
            }
        }
    }
} /* serialEvent */


/**********************************************************************************************************************
  printHelp()
**********************************************************************************************************************/
/*! \brief          prints usage of Led Matrix
 *  \details        this function prints usage of Led Matrix over UART
 *                  
 *********************************************************************************************************************/
void printHelp()
{
    Serial.println(F("command does not match"));
    Serial.println(F("known commands are: text, intensity and speed"));
    Serial.println(F("command and value has to be separated by an equal sign"));
    Serial.println(F("Example: text=example text"));
} /* printHelp */


/**********************************************************************************************************************
  stringCopy()
**********************************************************************************************************************/
/*! \brief          copies source string to destination string
 *  \details        this function copies the first number characters of source to destination. Null-character is 
 *					implicitly appended at the end of destination if source is longer than length.
 *
 *  \param[out]		Destination			destination string
 *  \param[in]		Source				source string
 *  \param[in]		Length				maximum characters to copy
 *                  
 *********************************************************************************************************************/
inline void stringCopy(char *Destination, const char *Source, int Length)
{
    strncpy(Destination, Source, Length-1);
    Destination[Length-1] = '\0';
}


/**********************************************************************************************************************
  ISR systTick()
**********************************************************************************************************************/
/*! \brief          
 *  \details        
 *                  
 *********************************************************************************************************************/
void systTick()
{
    Timer1Overflow = true;
}


