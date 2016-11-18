/*
	LED CUbe win32 Application
	Serial Header
	By Nicholas K Schulze
	www.HowNotToEngineer.com
	12/3/2011
*/

#include <windows.h>

#ifndef _SERIAL_h_
#define _SERIAL_h_

extern BOOL serialBegin(LPCSTR gszPort);
extern BOOL serialWrite(char buf);
extern void serialSend(char data[]);
extern void serialClose(void);

#endif
