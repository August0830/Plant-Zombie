#include <iostream>
#include "GardenBoard.h"
#include <stdio.h>  
#include <windows.h>  
#include <conio.h>  
#include <time.h>
using namespace std;
bool GAME_FAIL = false;//设置全局变量是便于通信，及时结束切换输出
extern const int Y_MAX;
void GardenBoard::generate_sun()
{
    sun_deposit += 10+10*sunflower_cnt;
}
void GardenBoard::SetPos(HANDLE HOutput,int x, int y)
{
    COORD point = { x, y };//光标要设置的位置x,y
    //HANDLE HOutput = GetStdHandle(STD_OUTPUT_HANDLE);//使用GetStdHandle(STD_OUTPUT_HANDLE)来获取标准输出的句柄
    SetConsoleCursorPosition(HOutput, point);//设置光标位置
}
void GardenBoard::print_garden()
{
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hOutput, &bInfo);
    COORD tst = { 0,0 };
    for (tst.X = 15; tst.X <= 115; tst.X += X_INTERVAL)
    {
        tst.Y = 0;
        do {
            SetConsoleCursorPosition(hOutput, tst);
            cout << "#";
            tst.Y++;
        } while (tst.Y <= 21);
    }
    tst.X = 0; tst.Y = 29;
    do {
        SetConsoleCursorPosition(hOutput, tst);
        cout << "#";
        tst.X++;
    } while (tst.X <= 119);
    for (tst.Y = 7; tst.Y <= 21; tst.Y += Y_INTERVAL)
    {
        tst.X = 0;
        do {
            SetConsoleCursorPosition(hOutput, tst);
            cout << "#";
            tst.X++;
        } while (tst.X <= 119);
    }//花园的划分  不封装输出是考虑到使用频繁而且担心句柄没有及时释放
    /*for (tst.X = 7; tst.X < 119; tst.X += X_INTERVAL)
    {
        for (tst.Y = 3; tst.Y < 22; tst.Y += Y_INTERVAL)
        {
            SetConsoleCursorPosition(hOutput, tst);
            cout << "#";
        }
    }//每个格子的中心位置 选择时把位置设置成中心位置 读取每一行pos中的格子数自动转换成指定位置*/
    
    for (int i = 0; i < row_total; ++i)
    {
        tst.Y = 3 + 7 * i;
        for (auto plant : garden_pos[i])
        {
            if (!plant || ((Plant*)plant)->type != 'p') continue;
            tst.X = 7 + 15 * ((Plant*)plant)->col;
            //tst.Y++;
            SetConsoleCursorPosition(hOutput, tst);
            cout << ((Plant*)plant)->plant_name;
        }
        for (auto zombie : garden_pos[i])
        {
            if (!zombie || ((Zombie*)zombie)->type != 'z') continue;
            tst.X = 7 + 15 * ((Zombie*)zombie)->col;
            //tst.Y--;
            SetConsoleCursorPosition(hOutput, tst);
            cout << "Zombie";// << " " << ((Zombie*)zombie)->col << " " << ((Zombie*)zombie)->life;
        }
    }

    tst.X = 0; tst.Y = 22;
    SetConsoleCursorPosition(hOutput, tst);
    cout << "Total Points: " << point_cnt;
    tst.X = 0; tst.Y = 23;
    SetConsoleCursorPosition(hOutput, tst);
    cout << "Sun Deposit: " << sun_deposit;
    tst.X = 30; tst.Y = 22;
    SetConsoleCursorPosition(hOutput, tst);
    cout << "Shop: ";//
    cout << "a Sunflower b Pea Shooter ";
    tst.X = 0; tst.Y = 29;
    SetConsoleCursorPosition(hOutput, tst);//最后的输出提示
    //CloseHandle(hOutput);
}
bool GardenBoard::ClearConsole(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO cinfo;//用于储存控制台缓冲区信息,在这里主要获取控制台缓冲区大小
    DWORD recnum;
    GetConsoleScreenBufferInfo(hOut, &cinfo);
    //cinfo.dwSize储存的是缓冲区大小
    //cinfo.dwSize.X * cinfo.dwSize.Y 即需填充的字符数
    COORD zero = { 0,0 };
    FillConsoleOutputCharacterW(hOut, L' ', cinfo.dwSize.X * cinfo.dwSize.Y, zero, &recnum);//从{0,0}处开始填充' '字符,成功填充个数为recnum
    FillConsoleOutputAttribute(hOut, 0, cinfo.dwSize.X * cinfo.dwSize.Y, zero, &recnum);//设置输出颜色,如果不是单一颜色,可能会有清除字符而背景色没有清除的现象
    SetConsoleCursorPosition(hOut, zero);//将光标设为{0,0}
    if (recnum != cinfo.dwSize.X * cinfo.dwSize.Y) return FALSE;
    return TRUE;
}
bool GardenBoard::random_generate_zom()
{
    int sum = 0;
    for (auto num : zombie_cnt)
    {
        //cout << num << " ";
        sum += num;
    }
    if (sum >= zombie_max)
        return false;
    srand((unsigned int)(time(NULL)));
    int row_ini = rand() % row_total;
    for (int i = 0; i < 2*row_total&& garden_pos[row_ini][col_total - 1]
        && zombie_cnt[row_ini] >= zombie_max / 2 != NULL; ++i)
        //避免一行集中太多僵尸
    {
            row_ini = rand() % row_total;
    }
    if (garden_pos[row_ini][col_total - 1])
        return false;
    else
    {
        //cout << row_ini << endl;
        Zombie* zm = new Zombie(row_ini,7); 
        garden_pos[zm->row][zm->col] = zm;
        zombie_cnt[zm->row]++;
        //cout << "generate zombie\n";
    }
}
void GardenBoard::refresh_state(HANDLE& hOutput, HANDLE& hIn, DWORD start)
{
    DWORD res;
    INPUT_RECORD Rec;
    while (1)//游戏主循环
    {
        generate_sun();
        ClearConsole(hOutput);
        if (GAME_FAIL)
        {
            ClearConsole(hOutput);
            //adjust format
            cout << "Game Failed!Zombies win!\n";
            break;
        }//在状态更新并显示完成后判断是否结束
        random_generate_zom();
        print_garden();
        for (int i = 0; i < row_total; ++i)
        {
            for (int j = 0; j < col_total; ++j)
            {
                if (!garden_pos[i][j]) continue;
                Plant* plt = (Plant*)(garden_pos[i][j]);
                Zombie* zom = (Zombie*)(garden_pos[i][j]);
                if (plt->type == 'p' && plt->plant_name == "Shooter")
                {
                    Shooter* shot = (Shooter*)plt;
                    for (int k = j + 1; k < col_total; ++k)
                    {
                        //该行有检测到僵尸就发射子弹
                        if (garden_pos[i][k] && ((Zombie*)(garden_pos[i][k]))->type == 'z')
                        {
                            //((Shooter*)plt)->attacking((Zombie*)(garden.garden_pos[i][k]));
                            shot->attacking();
                            for (vector<Bullet*>::iterator it = shot->bullet_set.begin(); it != shot->bullet_set.end();)
                            {
                                if ((*it)->col > Y_MAX)
                                {
                                    it = shot->bullet_set.erase(it);
                                }//清除飞出界外的子弹
                                else if ((*it)->col- 1 == ((Zombie*)(garden_pos[i][k]))->col
                                    || (*it)->col == ((Zombie*)(garden_pos[i][k]))->col)
                                {
                                    (*it)->attacking((Zombie*)(garden_pos[i][k]));
                                    // output = { 31,24 };
                                    //SetConsoleCursorPosition(hOutput, output);
                                    //cout << "Shooter attacked! ";
                                    it = shot->bullet_set.erase(it);
                                }//子弹与僵尸在同一个或者下一时刻僵尸与子弹即将交错时子弹发动攻击
                                else
                                {
                                    it++;
                                }//遍历中删除的方法
                            }
                        }
                    }
                }
                if (zom->type == 'z')
                {
                    if (garden_pos[zom->row][zom->col - 1])
                    {
                        Plant* plt = (Plant*)(garden_pos[zom->row][zom->col - 1]);
                        if (plt->type == 'p')
                        {
                            plt->get_hurt(zom);
                           // COORD output = { 32,24 };
                            //SetConsoleCursorPosition(hOutput, output);
                            //cout << "Zombie attacked! ";
                        }
                    }
                }
            }
        }//植物和僵尸互相攻击，基于花园提供的位置信息来调用攻击的函数

        for (int i = 0; i < row_total; ++i)
        {
            for (int j = 0; j < col_total; ++j)
            {
                if (!garden_pos[i][j]) continue;
                if (((Plant*)garden_pos[i][j])->type == 'p')
                {

                    if (((Plant*)garden_pos[i][j])->life <= 0)
                    {
                        delete garden_pos[i][j];
                        //cout << "Plant died";
                        garden_pos[i][j] = NULL;
                    }//清除死亡的植物
                    else if (((Shooter*)garden_pos[i][j])->plant_name == "Shooter")
                        //子弹的更新
                    {
                        Shooter* shot = ((Shooter*)garden_pos[i][j]);
                        for (vector<Bullet*>::iterator it = shot->bullet_set.begin(); it != shot->bullet_set.end();)
                        {
                            if ((*it)->col>= Y_MAX)
                            {
                                it = shot->bullet_set.erase(it);
                            }//清除已经飞出界外的子弹
                            else
                            {

                                (*it)->move(garden_pos);
                                (*it)->print_bullet(hOutput);
                                it++;
                            }//自动移动并且打印
                        }
                    }
                }
                else if (((Zombie*)garden_pos[i][j])->type == 'z')
                {
                    Zombie* zom = ((Zombie*)garden_pos[i][j]);
                    if (zom->life <= 0)
                    {
                        get_score(10);//之后可以设置成不同的分数值
                        zombie_counting(-1, i);
                        delete garden_pos[i][j];
                        COORD output = { 7+15*(j),3+7*i };
                        if (output.X <= 119)
                        {
                            SetConsoleCursorPosition(hOutput, output);
                            cout << "Zombie died!";
                        }
                        garden_pos[i][j] = NULL;
                    }//清除死亡的僵尸并且计分/更新僵尸数 
                    else
                    {
                        //COORD output = { 7 + 15 * j,3 + 7 * i+1 };
                        //SetConsoleCursorPosition(hOutput, output);
                        //cout << "life: "<<zom->life;
                        zom->move(garden_pos, i, j);
                    }
                }
            }
        }
        
        ReadConsoleInput(hIn, &Rec, 1, &res);
        if (Rec.EventType == KEY_EVENT && Rec.Event.KeyEvent.bKeyDown&&
            Rec.Event.KeyEvent.uChar.AsciiChar == 'b' || Rec.Event.KeyEvent.uChar.AsciiChar == 'B')
        {
            open_shop(hIn);
        }
        while (GetTickCount() - start < 1000);
        start = GetTickCount();//页面暂停
        SetConsoleCursorPosition(hOutput, COORD{ 30,26 });
    }
}
void GardenBoard::open_shop(HANDLE& hIn)
{
    DWORD res;
    INPUT_RECORD Rec;
    while (1)
    {
        ReadConsoleInput(hIn, &Rec, 1, &res);//读取一个输入操作
        if (Rec.EventType == KEY_EVENT && Rec.Event.KeyEvent.bKeyDown)
        {
            if (Rec.Event.KeyEvent.uChar.AsciiChar == 'b' || Rec.Event.KeyEvent.uChar.AsciiChar == 'B')
            {
                COORD display = { 30,25 };
                SetConsoleCursorPosition(hOutput, display);
                cout << "open shop\nUse keyboard to choose one:";
                //display.Y++;
                //SetConsoleCursorPosition(hOutput,display);
                char choice = 0;
                vector<char> shop_list{ 'a','b' };
                while (1)
                {
                    ReadConsoleInput(hIn, &Rec, 1, &res);
                    if (Rec.EventType == KEY_EVENT && Rec.Event.KeyEvent.bKeyDown)
                    {
                        choice = Rec.Event.KeyEvent.uChar.AsciiChar;
                        cout << "choice: " << choice << endl;
                        if (choice == 'x' || find(shop_list.begin(), shop_list.end(), choice) != shop_list.end())
                            break;
                    }
                }
                //display.Y++;
                //SetConsoleCursorPosition(hOutput, display);
                if (choice == 'x')
                {
                    cout << "Cancel buying\n";
                    break;
                }
                cout << "Choose block\n";
                while (1)
                {
                    ReadConsoleInput(hIn, &Rec, 1, &res);
                    if (Rec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)//如果点击了左键
                    {
                        int row = Rec.Event.MouseEvent.dwMousePosition.Y / 7;
                        int col = Rec.Event.MouseEvent.dwMousePosition.X / 15;
                        cout << "row " << row << " ";
                        cout << "col " << col << endl;

                        while (1)
                        {
                            ReadConsoleInput(hIn, &Rec, 1, &res);
                            if ((Rec.EventType == KEY_EVENT && Rec.Event.KeyEvent.bKeyDown)
                                || (Rec.EventType == MOUSE_EVENT && Rec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED))
                                break;
                        }
                        if (Rec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                        {
                            cout << "Copy that!\n";
                            switch (choice)
                            {
                            case 'a':
                            {

                                Sunflower* sunflower = new Sunflower(row, col);
                                if (sun_deposit > sunflower->sun_price)
                                {
                                    garden_pos[row][col] = sunflower;
                                    cout << "Sunflower!\n";
                                    sunflower_cnt++;
                                    sun_deposit -= sunflower->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete sunflower;
                                }
                                break;
                            }
                            case 'b':
                            {
                                Shooter* shot = new Shooter(row, col);
                                if (sun_deposit > shot->sun_price)
                                {
                                    garden_pos[row][col] = shot;
                                    cout << "Pea Shooter!\n";
                                    sun_deposit -= shot->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete shot;
                                }
                                break;
                            }
                            default:break;
                            }
                            break;
                        }
                        else if (Rec.Event.KeyEvent.uChar.AsciiChar == 'x')
                        {
                            cout << "Cancel buying\n";
                            break;
                        }
                    }
                    else if (Rec.Event.KeyEvent.uChar.AsciiChar == 'x')
                    {
                        cout << "Cancel buying\n";
                        break;
                    }
                }
            }
        }
        if (Rec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN || Rec.Event.KeyEvent.uChar.AsciiChar == 'x')
            break;
    }
}
