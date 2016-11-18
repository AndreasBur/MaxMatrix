/*
    Serial.cpp
    Created by Nicholas K. Schulze, Febuary 11, 2011.
*/

#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//CONSTRUCTOR
serial::serial(void){
     baud = 9600;
     error = 0;
}

//SETUP SERIAL PORT AND SPEED
void serial::begin(char comport[], int _baud){

     //SELECT BAUD RATE
     baud = _baud;
     DWORD BR;
     switch (baud){
            case 2400:
                 BR = CBR_2400;
                 break;
            case 4800:
                 BR = CBR_4800;
                 break;
            case 9600:
                 BR = CBR_9600;
                 break;
            case 19200:
                 BR = CBR_19200;
                 break;
            case 38400:
                 BR = CBR_38400;
                 break;
            case 57600:
                 BR = CBR_57600;
                 break;
            case 115200:
                 BR = CBR_115200;
                 break;
            case 128000:
                 BR = CBR_128000;
                 break;
            case 256000:
                 BR = CBR_256000;
                 break;
            default:
                 BR = CBR_9600;
                 break;
     }

    //OPEN COM PORT
    serialHandle = CreateFile(comport, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    //Throw error if com port does not exist
    if(serialHandle == INVALID_HANDLE_VALUE){
    	if(GetLastError() == ERROR_FILE_NOT_FOUND){
    		printf("ERROR: serial port does not exist\n");
    	}else{
            printf("ERROR: invalid handle");
    	}
        error = 1;
    }

    //SET SERIAL PARAMETERS
    if(error == 0){
        DCB dcbParamaters= {0};
        dcbParamaters.DCBlength = sizeof(dcbParamaters);

        if(!GetCommState(serialHandle, &dcbParamaters)){
        	printf("ERROR: error getting state\n");
        	error = 1;
        }

        //use chosen baud rate and standart settings, change these if required
        dcbParamaters.BaudRate = BR;
        dcbParamaters.ByteSize = 8;
        dcbParamaters.StopBits = ONESTOPBIT;
        dcbParamaters.Parity = NOPARITY;

        if(!SetCommState(serialHandle, &dcbParamaters)){
        	printf("ERROR: error setting state\n");
        	error = 1;
        }
    }

    //SET TIMEOUTS
    if(error ==0){
        COMMTIMEOUTS timeout = {0};

        timeout.ReadIntervalTimeout = 40;
        timeout.ReadTotalTimeoutConstant = 40;
        timeout.ReadTotalTimeoutMultiplier = 10;
        timeout.WriteTotalTimeoutConstant = 40;
        timeout.WriteTotalTimeoutMultiplier = 10;

        if(!SetCommTimeouts(serialHandle, &timeout)){
        	printf("ERROR: timeout error\n");
        	error = 1;
        }
    }
}

//CLOSE PORT, do this at end of program otherwise the comport may get stuck open
void serial::close(void){
     CloseHandle(serialHandle);
}

//SEND A SINGLE CHARACTER
void serial::write(char data){
    char buf[1] = {data};
   	DWORD dwBytesWrite = 0;

	if(!WriteFile(serialHandle,buf,1,&dwBytesWrite,NULL)){
		printf("ERROR: write error\n");
	}
}

//SEND STRING, or any array of chars
void serial::send(char data[]){
    while(*data){
        write(*data++);
    }
}
