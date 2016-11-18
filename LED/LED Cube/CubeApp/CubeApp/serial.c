/*
	LED CUbe win32 Application
	Serial
	By Nicholas K Schulze
	www.HowNotToEngineer.com
	12/3/2011
*/

#include "serial.h"

BOOL comError = FALSE;
HANDLE hComm = NULL;

//INITIALISE SCI
BOOL serialBegin(LPCSTR gszPort){
	
	DCB dcbParamaters = {0};
	COMMTIMEOUTS timeout = {0};
	hComm = CreateFileA(gszPort, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	comError = FALSE;

	if(hComm == INVALID_HANDLE_VALUE){
		MessageBox(NULL, L"Error Opening Port", L"Serial Error", MB_OK | MB_ICONERROR);
		comError = TRUE;
		return FALSE;
	}
	
	dcbParamaters.DCBlength=sizeof(dcbParamaters);
	if (!GetCommState(hComm, &dcbParamaters)){
		MessageBox(NULL, L"Error Getting State", L"Serial Error", MB_OK | MB_ICONERROR);
		comError = TRUE;
		return FALSE;
	}

	dcbParamaters.BaudRate=CBR_256000;
	dcbParamaters.ByteSize=8;
	dcbParamaters.StopBits=ONESTOPBIT;
	dcbParamaters.Parity=NOPARITY;

	if(!SetCommState(hComm, &dcbParamaters)){
		MessageBox(NULL, L"Error Setting State", L"Serial Error", MB_OK | MB_ICONERROR);
		comError = TRUE;
		return FALSE;
	}

	return !comError;
}

//SEND ONE BYTE
BOOL serialWrite(char buf){
	OVERLAPPED osWrite = {0};
	DWORD dwWritten;
	DWORD dwToWrite = 1;
	LPCVOID lpBuf = &buf;

	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL){
		MessageBox(NULL, L"Error Creating Overlapped Event", L"Serial Error", MB_OK | MB_ICONERROR);
		comError = TRUE;
		return FALSE;
	}

	if (!WriteFile(hComm, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
		if (GetLastError() != ERROR_IO_PENDING) { 
			MessageBox(NULL, L"Write Failed", L"Serial Error", MB_OK | MB_ICONERROR);
			return FALSE;
		}else{
			if (!GetOverlappedResult(hComm, &osWrite, &dwWritten, TRUE)){
			return FALSE;
			}else{
			return TRUE;
			}
		}
	}else{
		return TRUE;
	}

	CloseHandle(osWrite.hEvent);
}

//SEND STRING VIA SCI
void serialSend(char data[]){
	while(*data){
        if(!serialWrite(*data++)){
			 MessageBox(NULL, L"Write Failed in Send", L"Serial Error", MB_OK | MB_ICONERROR);
			 break;
		}
    }
}

void serialClose(void){
     CloseHandle(hComm);
}

