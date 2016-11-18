/*
	LED CUbe win32 Application
	WinMain
	By Nicholas K Schulze
	www.HowNotToEngineer.com
	11/3/2011
*/


#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <commctrl.h>
#include "resource.h"
#include "serial.h"
#include "animations.h"

#define CreateWindow CreateWindowA

void doButtonCreation(HWND hwnd);

//WINDOW VARS
const LPCWSTR g_szClassName = L"INTERACTIVE LED CUBE";
const int width = 300;
const int height = 400;
HWND g_hToolbar = NULL;
BOOL showToolbar = FALSE;
char comName[] = "COM3";

//THREAD VARS 
HANDLE hThread = NULL;
DWORD threadID;
BOOL endThread = FALSE; 

//ANIMATION VARS
BOOL run = FALSE;
BOOL interact = FALSE;
animationData aDataIn;
BOOL captureMouse = FALSE;
BOOL captureType = FALSE;
BOOL clockOn = FALSE;
BOOL snakePlay = FALSE;
BOOL start = FALSE;
BOOL serialSuccess = FALSE;

void resetAnimation(void){
	clear();
	defaultMem();
	aDataIn.buttonPress = 0;
}

//ANIMATION THREAD
DWORD WINAPI ThreadProc(LPVOID lpParam){
	while(!endThread){
		if(start){
			serialClose();
			if(serialBegin((LPCSTR)comName)){
				serialSuccess = TRUE;
			}else{
				serialSuccess = FALSE;
			}
			start = FALSE;
		}

		if(run){
			//all animation inputs are hendled through an animationData structure 
			//defined in animations.h 
			animate(aDataIn, interact);
			if(buttonHandled()){
				//wait until the animations have handled the button before reseting it
				aDataIn.letter =  0x00;
				aDataIn.buttonPress = 0;
			}
		}
	}
	return 0;
}

//TOOLBAR PROCEDURE
BOOL CALLBACK ToolDlgProc(HWND hwnd, int Message, WPARAM wParam, LPARAM lParam){
	switch(Message){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDR_BUT_SNAKE: //snake game
					clear();
					clearDummy();
					defaultMem();
					snakePlay = !snakePlay;
					if(snakePlay){
						aDataIn.animation = 1;
					}else{
						aDataIn.animation = 0;
					}
					captureMouse = FALSE;
					captureType = FALSE;
					clockOn = FALSE;
					SetFocus(hwnd);
					break;
				case IDR_BUT_MOUSE: //mouse tracking
					clear();
					clearDummy();
					defaultMem();
					captureMouse = !captureMouse;
					captureType = FALSE;
					clockOn = FALSE;
					snakePlay = FALSE;
					if(captureMouse){
						aDataIn.animation = 2;
					}else{
						aDataIn.animation = 0;
					}
					SetFocus(hwnd);
					break;
				case IDR_BUT_TYPE: //keyboard input
					clear();
					clearDummy();
					defaultMem();
					captureMouse = FALSE;
					snakePlay = FALSE;
					clockOn = FALSE;
					aDataIn.letter = 0x00;
					captureType = !captureType;
					if(captureType){
						aDataIn.animation = 3;
					}else{
						aDataIn.animation = 0;
					}
					SetFocus(hwnd);
					break;
				case IDR_BUT_CLOCK: //digital clock
					clear();
					clearDummy();
					defaultMem();
					clockOn = !clockOn;
					captureMouse = FALSE;
					captureType = FALSE;
					snakePlay = FALSE;
					if(clockOn){
						aDataIn.animation = 4;
					}else{
						aDataIn.animation = 0;
					}
					break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

//DIALOG PROCEDURE
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch(message){
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case IDR_BUT_OK:
					EndDialog(hwnd, IDR_BUT_OK);
					break;
            }
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

//WINDOWS PROCEDURE
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
			endThread = TRUE;
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			serialClose();
            PostQuitMessage(0);
            break;
       case WM_KEYDOWN: //buttons for snake game
            switch(wParam){
                case VK_UP:
					aDataIn.buttonPress = 1;
					break;
				case VK_DOWN:
					aDataIn.buttonPress = 2;
					break;
				case VK_RIGHT:
					aDataIn.buttonPress = 3;
					break;
				case VK_LEFT:
					aDataIn.buttonPress = 4;
					break;
				case VK_TAB:
					aDataIn.buttonPress = 5;
					break;
				case VK_SHIFT:
					aDataIn.buttonPress = 6;
					break;
            }
            break;
	   case WM_CHAR: //printable characters for keyboard input
			aDataIn.letter = wParam;
			break;
        case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDR_MAIN_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				case IDR_MAIN_HELP_ABOUT:
					DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_DLG_ABOUT),hwnd, AboutDlgProc);
					break;
				case IDR_BUT_START:
					//Close serial port if it is open somehow then Attempt to re/start the serial, and enable all buttons
					start = TRUE;
					Sleep(100);
					if(serialSuccess){
						run = TRUE;
						resetAnimation();
						aDataIn.animation = 0;
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_STOP), TRUE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_START), FALSE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_RESET), TRUE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_NEXT), TRUE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_PREV), TRUE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_INTERACT), TRUE);
						aDataIn.hwnd = hwnd;
					}
					SetFocus(hwnd);
					break;
				case IDR_BUT_STOP:
					//stop and reset all animations, the animation thread will keep running but do nothing
					run = FALSE;
					interact = FALSE;
					resetAnimation();
					EnableWindow(GetDlgItem(hwnd,IDR_BUT_STOP), FALSE);
					EnableWindow(GetDlgItem(hwnd,IDR_BUT_START), TRUE);
					EnableWindow(GetDlgItem(hwnd,IDR_BUT_NEXT), FALSE);
					EnableWindow(GetDlgItem(hwnd,IDR_BUT_PREV), FALSE);
					EnableWindow(GetDlgItem(hwnd,IDR_BUT_RESET), FALSE);
					EnableWindow(GetDlgItem(hwnd,IDR_BUT_INTERACT), FALSE);
					SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_SETPOS,(WPARAM) TRUE,(LPARAM) 6);
					aDataIn.speed = 6;
					ShowWindow(g_hToolbar, SW_HIDE);
					interact = FALSE;
					showToolbar = FALSE;
					captureMouse = FALSE;
					snakePlay = FALSE;
					clockOn = FALSE;
					captureType = FALSE;
					SetFocus(hwnd);
					break;
				case IDR_BUT_INTERACT:
					//opens interactive animations toolbar
					interact = !interact;
					showToolbar = !showToolbar;
					if(showToolbar){
						ShowWindow(g_hToolbar, SW_SHOW);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_NEXT), FALSE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_PREV), FALSE);
						SetFocus(hwnd);
					}else{
						ShowWindow(g_hToolbar, SW_HIDE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_NEXT), TRUE);
						EnableWindow(GetDlgItem(hwnd,IDR_BUT_PREV), TRUE);
						captureMouse = FALSE;
						SetFocus(hwnd);
					}
					defaultMem();
					aDataIn.animation = 0;
					SetFocus(hwnd);
					break;
				case IDR_BUT_NEXT:
					aDataIn.animation++;
					if(aDataIn.animation == numOfAnimations){
						aDataIn.animation = 0;
					}
					resetAnimation();
					SetFocus(hwnd);
					break;
				case IDR_BUT_PREV:
					aDataIn.animation--;
					if(aDataIn.animation == -1){
						aDataIn.animation = numOfAnimations - 1;
					}
					resetAnimation();
					SetFocus(hwnd);
					break;
				case IDR_BUT_RESET:
					//resets current animation to initail state
					resetAnimation();
					SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_SETPOS,(WPARAM) TRUE,(LPARAM) 6);
					aDataIn.speed = SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_GETPOS, 0, 0);
					SetFocus(hwnd);
					break;
				default:
					//cheat way of getting comPort selection
					if(LOWORD(wParam) >= IDR_MAIN_SETUP_COM1 && LOWORD(wParam) <= IDR_MAIN_SETUP_COM8){
						serialSuccess = FALSE;
						memset(comName, ' ', sizeof(comName));
						itoa(LOWORD(wParam), comName, 10);
						memcpy(comName+3, comName+2, 1);
						memcpy(comName, "COM", 3);
					}
					break;
			}
			break;
		case WM_HSCROLL:
			//speed control
			switch(LOWORD(wParam)){
				case TB_THUMBTRACK:{
						aDataIn.speed = SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_GETPOS, 0, 0);
						SetFocus(hwnd);
					}
					break;
			}
			break;
        case WM_CREATE:
				doButtonCreation(hwnd);
				g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_TOOL_INT), hwnd, ToolDlgProc);
				hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &threadID);
				resetAnimation();
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

//MAIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //REGISTER WINDOWS CLASS
    wc.cbSize = sizeof(WNDCLASSEX);                 //size of the structure
    wc.style = 0;                                   //Class styles usually 0
    wc.lpfnWndProc = WndProc;                       //pointer to the windows procedure for this class
    wc.cbClsExtra = 0;                              //Extra data allocated for this class usually 0
    wc.cbWndExtra = 0;                              //Extra data allocated per window usually 0
    wc.hInstance = hInstance;                       //Handle to app instance
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);     //Alt tab icon 32x32
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       //Cursor over this window
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);    //background color
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);  //name of menu resource to use for the window
    wc.lpszClassName = g_szClassName;               //name to ID class
    wc.hIconSm = LoadIcon(GetModuleHandle(NULL),    //taskbar icon 16x16
                MAKEINTRESOURCE(IDR_MYICON));

    if(!RegisterClassEx(&wc)){
        MessageBox(NULL, L"Window Reg Failed", L"Win32 Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //CREATE WINDOW
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                        g_szClassName,
                        L"LED CUBE",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,width,height,
                        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL){
        MessageBox(NULL, L"Window Creation Failed", L"Win32 Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //MESSAGE LOOP
    while(GetMessage(&Msg, NULL, 0, 0) > 0){
        if(!IsDialogMessage(g_hToolbar, &Msg)){
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
    }

    return Msg.wParam;
}

//SETUP BUTTONS
void doButtonCreation(HWND hwnd){
	RECT rcClient;
	HWND bStart, bStop, bNext, bPrev, bInteract, bReset, bSlider;
	int cWidth, cHeight, cBottom, cTop;
	
	//get window size
	GetClientRect(hwnd, &rcClient);
	cBottom = rcClient.bottom;
	cTop = rcClient.top;
	cWidth = rcClient.right - rcClient.left;
	cHeight = rcClient.bottom - rcClient.top;
	
	//create buttons using windows size
	bStart = CreateWindow("BUTTON", "START", WS_TABSTOP|BS_PUSHBUTTON|WS_CHILDWINDOW|WS_VISIBLE, (cWidth/4 - 75/2), cBottom - 40, 75, 30, hwnd, (HMENU)IDR_BUT_START, GetModuleHandle(NULL), NULL);
	bStop = CreateWindow("BUTTON", "STOP", WS_TABSTOP|BS_PUSHBUTTON|WS_CHILDWINDOW|WS_VISIBLE, (3*cWidth/4 - 75/2), cBottom - 40, 75, 30, hwnd, (HMENU)IDR_BUT_STOP, GetModuleHandle(NULL), NULL);
	bNext = CreateWindow("BUTTON", "NEXT", WS_TABSTOP|BS_PUSHBUTTON|WS_CHILDWINDOW|WS_VISIBLE, (3*cWidth/4 - 75/2), 10, 75, 30, hwnd, (HMENU)IDR_BUT_NEXT, GetModuleHandle(NULL), NULL);
	bPrev = CreateWindow("BUTTON", "PREV", WS_TABSTOP|BS_PUSHBUTTON|WS_CHILDWINDOW|WS_VISIBLE, (cWidth/4 - 75/2), 10, 75, 30, hwnd, (HMENU)IDR_BUT_PREV, GetModuleHandle(NULL), NULL);
	bInteract = CreateWindow("BUTTON", "INTERACTIVE",WS_TABSTOP|BS_PUSHBUTTON|WS_CHILDWINDOW|WS_VISIBLE,(cWidth/2 - 100/2), 50, 100, 30, hwnd, (HMENU)IDR_BUT_INTERACT, GetModuleHandle(NULL), NULL);
	bReset = CreateWindow("BUTTON", "RESET",WS_TABSTOP|BS_PUSHBUTTON|WS_CHILDWINDOW|WS_VISIBLE,(cWidth/2 - 80/2), cBottom - 80, 80, 30, hwnd, (HMENU)IDR_BUT_RESET, GetModuleHandle(NULL), NULL);
	
	//create and set slider to default position
	bSlider = CreateWindow(TRACKBAR_CLASSA, "", WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS, (cWidth/2 - 200/2), 100, 200, 30, hwnd, (HMENU)IDR_SLIDE_SPEED, GetModuleHandle(NULL), NULL);
	SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM) MAKELONG(1, 10));
	SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_SETPAGESIZE, 1, (LPARAM) 10);
	SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_SETPOS,(WPARAM) TRUE,(LPARAM) 6);
	aDataIn.speed = SendMessage(GetDlgItem(hwnd, IDR_SLIDE_SPEED), TBM_GETPOS, 0, 0);
	
	//set default enabled conditions 
	EnableWindow(GetDlgItem(hwnd,IDR_BUT_STOP), FALSE);
	EnableWindow(GetDlgItem(hwnd,IDR_BUT_NEXT), FALSE);
	EnableWindow(GetDlgItem(hwnd,IDR_BUT_PREV), FALSE);
	EnableWindow(GetDlgItem(hwnd,IDR_BUT_RESET), FALSE);
	EnableWindow(GetDlgItem(hwnd,IDR_BUT_INTERACT), FALSE);
}
