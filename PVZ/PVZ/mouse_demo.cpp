#include <stdio.h>  
//#include <stdafx.h>
#include <windows.h>  
#include <conio.h>  
#include "GardenBoard.h"
#include <iostream>
#include <algorithm>
using namespace std;

int main2()
{
    INPUT_RECORD eventMsg;
    DWORD Pointer;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    /*GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE;  //移除快速编辑模式
    mode &= ~ENABLE_INSERT_MODE;      //移除插入模式
    mode &= ~ENABLE_MOUSE_INPUT;
    SetConsoleMode(hStdin, mode);*/
    SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    
    while (1) {
        /*GetAsyncKeyState(VK_LBUTTON & 0x8000);
        cout << "click left\n";*/
        ReadConsoleInput(hIn, &eventMsg, 1, &Pointer);//Read input msg
        if (eventMsg.EventType == MOUSE_EVENT && eventMsg.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            printf("Right button clicked.");
        }
        if (eventMsg.EventType == MOUSE_EVENT && eventMsg.Event.MouseEvent.dwEventFlags == RIGHTMOST_BUTTON_PRESSED) {
            printf("Left button double clicked.");
        }
    }
    CloseHandle(hIn);
    return 0;
}