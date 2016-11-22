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
/**     \file       LedMatrix_1_module.ino
 *      \brief      
 *
 *      \details    
 *                  
 *
 *****************************************************************************************************************************************************/

/******************************************************************************************************************************************************
 * INCLUDES
 *****************************************************************************************************************************************************/
#include <MaxMatrix.h>
#include <avr/pgmspace.h>
#include <TimerOne.h>
#include <StandardTypes.h>


/******************************************************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *****************************************************************************************************************************************************/
#define UART_INPUT_LENGTH                               100
#define COMMAND_MAX_LENGTH                              10
#define MAXMATRIX_MAX_TEXT_LENGTH                       (UART_INPUT_LENGTH - COMMAND_MAX_LENGTH)

#define MAXMATRIX_CS_PIN                                10
#define MAXMATRIX_CLOCK_PIN                             11
#define MAXMATRIX_DATA_PIN                              12

#define MAXMATRIX_SPEED_MIN_VALUE                       1
#define MAXMATRIX_SPEED_MAX_VALUE                       20

#define MAXMATRIX_INTENSITY_MIN_VALUE                   1
#define MAXMATRIX_INTENSITY_MAX_VALUE                   16

#define MAXMATRIX_SPACE_BETWEEN_CHARS_MIN_VALUE         0
#define MAXMATRIX_SPACE_BETWEEN_CHARS_MAX_VALUE         255


/******************************************************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *****************************************************************************************************************************************************/
#define TOSTRING(x) #x


/******************************************************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *****************************************************************************************************************************************************/
 /* Type which describes the internal state of the LedMatrix */
typedef enum {
    LEDMATRIX_STATE_NONE,
    LEDMATRIX_STATE_READY,
    LEDMATRIX_STATE_STRING_SHIFT
} LedMatrixStateType;


/******************************************************************************************************************************************************
 *  GLOBAL DATA
 *****************************************************************************************************************************************************/
char UartInput[UART_INPUT_LENGTH];                  // buffer for incoming UART data
boolean UartInputComplete;                          // flag UART string is complete
char MaxMatrixText[MAXMATRIX_MAX_TEXT_LENGTH];      // current Matrix Text
bool Timer1Overflow;                                // flag Timer1 overflow
MaxMatrix Matrix(MAXMATRIX_DATA_PIN, MAXMATRIX_CS_PIN, MAXMATRIX_CLOCK_PIN);
LedMatrixStateType State;


/******************************************************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *****************************************************************************************************************************************************/
void uartInputHandle();
void serialEvent();
void printHelp();
void stringCopy(char*, const char*, int);
void systTick();


/******************************************************************************************************************************************************
 *  ARDUINO setup
 *****************************************************************************************************************************************************/
void setup()
{
    Timer1.initialize(200000);
    Timer1.attachInterrupt(systTick);
    Serial.begin(9600);
    Matrix.init();
    
    Matrix.setModuleOrientation(MAXMATRIX_MODULE_ORIENTATION_90);
    Matrix.setText("Andreas");
    stringCopy(MaxMatrixText, "Andreas Burnickl", sizeof(MaxMatrixText));
    UartInputComplete = false;
    Timer1Overflow = false;
    
    State = LEDMATRIX_STATE_READY;
    //State = LEDMATRIX_STATE_STRING_SHIFT;
}


/******************************************************************************************************************************************************
 *  ARDUINO loop
 *****************************************************************************************************************************************************/
void loop()
{ 
    if (UartInputComplete)
    {
        uartInputHandle();
        // clear the string:
        UartInput[0] = STD_NULL_CHARACTER;
        UartInputComplete = false;
    }

    if(Timer1Overflow && State == LEDMATRIX_STATE_STRING_SHIFT)
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


/******************************************************************************************************************************************************
  uartInputHandle()
 *****************************************************************************************************************************************************/
/*! \brief          handles the incoming UART data
 *  \details        this function handles and validates the incoming data. After splitting the data into
 *                  command and value a specific action will be called.
 *                 
 *****************************************************************************************************************************************************/
void uartInputHandle()
{
    char command[COMMAND_MAX_LENGTH] = "";
    char valueASCII[MAXMATRIX_MAX_TEXT_LENGTH] = "";
    int value = 0;
    const char delimiterCommand[] = "=";
    const char delimiterValue[] = ",";
    char *token = NULL;

    if(strchr(UartInput, delimiterCommand[0]))
    {
        token = strtok(UartInput, delimiterCommand);
        if(token != NULL) stringCopy(command, token, sizeof(command));
        token = strtok(NULL, delimiterCommand);
        if(token != NULL) stringCopy(valueASCII, token, sizeof(valueASCII));

        if(strcmp(command, "textShift") == 0) {
            if(LEDMATRIX_STATE_STRING_SHIFT != State) State = LEDMATRIX_STATE_STRING_SHIFT;
            stringCopy(MaxMatrixText, valueASCII, sizeof(MaxMatrixText));
            Matrix.clear();
            Matrix.setTextWithShift(MaxMatrixText);
        }
        else if(strcmp(command, "text") == 0) {
            if(LEDMATRIX_STATE_READY != State) State = LEDMATRIX_STATE_READY;
            stringCopy(MaxMatrixText, valueASCII, sizeof(MaxMatrixText));
            Matrix.clear();
            Matrix.setText(MaxMatrixText);
        }
        else if(strcmp(command, "intensity") == 0) {
            value = atoi(valueASCII);
            if(value >= MAXMATRIX_INTENSITY_MIN_VALUE && value <= MAXMATRIX_INTENSITY_MAX_VALUE) {
                Matrix.setIntensity(value - 1);
            } else {
                Serial.println(F("value out of range"));
                Serial.println(F("intensity range "TOSTRING(MAXMATRIX_INTENSITY_MIN_VALUE)" ... "TOSTRING(MAXMATRIX_INTENSITY_MAX_VALUE)));
            }
        }
        else if(strcmp(command, "speed") == 0) {
            value = atoi(valueASCII);
            if(value >= MAXMATRIX_SPEED_MIN_VALUE && value <= MAXMATRIX_SPEED_MAX_VALUE) {
                Timer1.setPeriod(((long)(MAXMATRIX_SPEED_MAX_VALUE - value + 1) * 5) * 10000);
            } else {
                Serial.println(F("value out of range"));
                Serial.println(F("speed range "TOSTRING(MAXMATRIX_SPEED_MIN_VALUE)" ... "TOSTRING(MAXMATRIX_SPEED_MAX_VALUE)));
            }
        }
        else if(strcmp(command, "spaceBetweenChars") == 0) {
            value = atoi(valueASCII);
            if(value >= MAXMATRIX_SPACE_BETWEEN_CHARS_MIN_VALUE && value <= MAXMATRIX_SPACE_BETWEEN_CHARS_MAX_VALUE) {
                    Matrix.setSpaceBetweenChars(value);
                } else {
                Serial.println(F("value out of range"));
                Serial.println(F("spaceBetweenChars range "TOSTRING(MAXMATRIX_SPACE_BETWEEN_CHARS_MIN_VALUE)" ... "TOSTRING(MAXMATRIX_SPACE_BETWEEN_CHARS_MAX_VALUE)));
            }
        }
        else if(strcmp(command, "setDot") == 0) {
            if(LEDMATRIX_STATE_READY != State) {
                State = LEDMATRIX_STATE_READY;
                Matrix.clear();
            }
            char columnASCII[3] = "", rowASCII[3] = "", valueDotASCII[3] = "";
            token = strtok(valueASCII, delimiterValue);
            if(token != NULL) stringCopy(columnASCII, token, sizeof(columnASCII));
            token = strtok(NULL, delimiterValue);
            if(token != NULL) stringCopy(rowASCII, token, sizeof(rowASCII));
            token = strtok(NULL, delimiterValue);
            if(token != NULL) stringCopy(valueDotASCII, token, sizeof(valueDotASCII));
            Matrix.setDot(atoi(columnASCII), atoi(rowASCII), atoi(valueDotASCII));
        }
        else {
            Serial.println(command);
            printHelp(); 
        }
    } else {
        Serial.println(command);
        printHelp(); 
    }
}


/******************************************************************************************************************************************************
  serialEvent()
 *****************************************************************************************************************************************************/
/*! \brief          save incoming UART byte into UART buffer
 *  \details        this function saves incoming UART signs into UART buffer.
 *                  After receiving new line, string is complete and flag will be set.
 *  \pre            will be called at the end of the main function             
 *****************************************************************************************************************************************************/
void serialEvent() 
{
    while (Serial.available()) 
    {
        // get the new byte from UART
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
                UartInput[UartInputLength+1] = STD_NULL_CHARACTER;
            }
        }
    }
} /* serialEvent */


/******************************************************************************************************************************************************
  printHelp()
 *****************************************************************************************************************************************************/
/*! \brief          prints usage of Led Matrix
 *  \details        this function prints usage of Led Matrix over UART
 *                  
 *****************************************************************************************************************************************************/
void printHelp()
{
    Serial.println(F("command does not match"));
    Serial.println(F("known commands are: text, intensity and speed"));
    Serial.println(F("command and value has to be separated by an equal sign"));
    Serial.println(F("Example: text=example text"));
} /* printHelp */


/******************************************************************************************************************************************************
  stringCopy()
 *****************************************************************************************************************************************************/
/*! \brief          copies source string to destination string
 *  \details        this function copies the first number characters of source to destination. Null-character 
 *                  is implicitly appended at the end of destination if source is longer than length.
 *
 *  \param[out]     Destination         destination string
 *  \param[in]      Source              source string
 *  \param[in]      Length              maximum characters to copy
 *                  
 *****************************************************************************************************************************************************/
inline void stringCopy(char *Destination, const char *Source, int Length)
{
    strncpy(Destination, Source, Length - 1);
    Destination[Length-1] = STD_NULL_CHARACTER;
}


/******************************************************************************************************************************************************
  ISR systTick()
 *****************************************************************************************************************************************************/
/*! \brief          set timer1 flag
 *  \details        this function sets the timer1 overflow flag. 
 *                  
 *  \pre            will be called from interrupt context by an Hardware Timer
 *****************************************************************************************************************************************************/
void systTick()
{
    Timer1Overflow = true;
}


/******************************************************************************************************************************************************
 *  E N D   O F   F I L E
 *****************************************************************************************************************************************************/