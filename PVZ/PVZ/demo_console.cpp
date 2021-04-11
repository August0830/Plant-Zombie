#include <stdio.h>  
#include <windows.h>  
#include <conio.h>  
#include "GardenBoard.h"
#include <iostream>
#include <algorithm>

using namespace std;
extern bool GAME_FAIL;
extern const int Y_MAX;
extern int shooter_attack_val;
extern int shooter_life_val;
extern int shooter_sun_price_val;
int main(int argc, char * argv[])
{
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    SMALL_RECT rc = { 0,0, 119, 29 }; // 坐标位置结构体初始化
    SetConsoleWindowInfo(hOutput, true, &rc);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hOutput, &bInfo);
    DWORD start = GetTickCount();
    GardenBoard garden(3, 8,hIn,hOutput);
    
    garden.print_garden();
    getchar();
    Shooter* shot = new Shooter(0, 1);
    garden.garden_pos[shot->row][shot->col]=shot;
    shot = new Shooter(1, 2);
    garden.garden_pos[shot->row][shot->col]=shot;
    shot = new Shooter(2, 1);
    garden.garden_pos[shot->row][shot->col] = shot;
    garden.print_garden();
    getchar();
    garden.refresh_state(hOutput, hIn,start);  
    CloseHandle(hOutput);
    CloseHandle(hIn);
    return 0;
}
