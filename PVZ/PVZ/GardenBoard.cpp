#include <iostream>
#include "GardenBoard.h"
#include <stdio.h>  
#include <windows.h>  
#include <conio.h>  
#include <time.h>
using namespace std;
bool GAME_FAIL = false;//设置全局变量是便于通信，及时结束切换输出
extern const int Y_MAX;
int turn = 0;
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
    map<pair<int, int>, int> adjust_pos;
    for (int i = 0; i < row_total; ++i)
    {
        //tst.Y = 3 + 7 * i;
        //tst.Y = 1+7 * i;
        for (auto plant : garden_pos[i])
        {
            if (!plant || ((Plant*)plant)->type != 'p') continue;
            if (adjust_pos.count(make_pair(i, ((Plant*)plant)->col)) == 0)
                adjust_pos[make_pair(i, ((Plant*)plant)->col)] = 1;
            else
                adjust_pos[make_pair(i, ((Plant*)plant)->col)]++;
            tst.X = 4 + 15 * ((Plant*)plant)->col;
            //tst.Y++;
            tst.Y = 7 * i + adjust_pos[make_pair(i, ((Plant*)plant)->col)];
            SetConsoleCursorPosition(hOutput, tst);
            cout << ((Plant*)plant)->plant_name<<" "<< ((Plant*)plant)->life;
        }
        for (auto zombie : garden_pos[i])
        {
            if (!zombie || ((Zombie*)zombie)->type != 'z') continue;
            if (adjust_pos.count(make_pair(i, (((Zombie*)zombie)->col) == 0)))
                adjust_pos[make_pair(i, ((Zombie*)zombie)->col)] = 1;
            else
                adjust_pos[make_pair(i, ((Zombie*)zombie)->col)]++;
            tst.X = 7 + 15 * ((Zombie*)zombie)->col;
            tst.Y = 7 * i + adjust_pos[make_pair(i, ((Zombie*)zombie)->col)];
            //tst.Y--;
            SetConsoleCursorPosition(hOutput, tst);
            //cout << "Zombie "<<((Zombie*)zombie)->life;// << " " << ((Zombie*)zombie)->col << " " << ;
            //cout << ((Zombie*)zombie)->zombie_name << " " << ((Zombie*)zombie)->life;
            if (((Zombie*)zombie)->zombie_name == "Zombie")
                ((Zombie_Normal*)zombie)->print_Z();
            else if (((Zombie*)zombie)->zombie_name == "Conhead Zombie")
                ((Conhead_Zombie*)zombie)->print_Z();
            else if (((Zombie*)zombie)->zombie_name == "Paper Zombie")
            {
                if(((Paper_Zombie*)zombie)->life<30)
                    ((Paper_Zombie*)zombie)->change_state();
                ((Paper_Zombie*)zombie)->print_Z();
            }
            else
                ((Zombie*)zombie)->print_Z();
        }
    }

    tst.X = 0; tst.Y = 22;
    SetConsoleCursorPosition(hOutput, tst);
    cout << "Total Points: " << point_cnt<< " Turn: "<<turn;
    tst.X = 0; tst.Y = 23;
    SetConsoleCursorPosition(hOutput, tst);
    cout << "Sun Deposit: " << sun_deposit;
    tst.X = 30; tst.Y = 22;
    SetConsoleCursorPosition(hOutput, tst);
    cout << "Shop: ";//
    cout << "a.Sunflower 10 ";
    cout << " b.Pea Shooter " << shooter_sun_price_val;
    cout<<" c.Double Shooter "<< double_shooter_sun_price_val;
    cout<<" d.Frozen Shooter "<< double_shooter_sun_price_val;
    cout << " e.Nut " << defense_sun_price_val;
    tst.X = 30; tst.Y = 23;
    SetConsoleCursorPosition(hOutput, tst);
    cout << "f.High Nut " << defense_sun_price_val + 10;
    cout << " g.Squash " << bomb_sun_price_val;
    cout << " h.Cherry Bomb " << bomb_sun_price_val + 10;
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
    for (int i = 0; i < 2*row_total&& garden_pos_cnt[row_ini][col_total - 1]<unit_max;
        ++i)
        //避免一行集中太多僵尸
    {
            row_ini = rand() % row_total;
    }
    if (garden_pos_cnt[row_ini][col_total - 1]==unit_max)
        return false;
    else
    {
        //cout << row_ini << endl;
        int type_z = rand();
        Zombie* zm = NULL;
        if (type_z % 7 == 0)
            zm = new Conhead_Zombie(row_ini, col_total - 1);
        else if (type_z % 7 == 1)
            zm = new Paper_Zombie(row_ini, col_total - 1);
        else if (type_z % 7 == 2)
            zm = new Jester_Zombie(row_ini, col_total - 1);
        else if (type_z % 7 == 3)
            zm = new Pole_Zombie(row_ini, col_total - 1);
        else if (type_z % 7 == 4)
            zm = new Stone_Zombie(row_ini, col_total - 1);
        else
            zm = new Stone_Zombie(row_ini,col_total-1);
        //garden_pos[zm->row][zm->col] = zm;
        garden_pos[zm->row].push_back(zm);
        garden_pos_cnt[zm->row][zm->col]++;
        zombie_cnt[zm->row]++;
        int val = rand() % 5;
        //cout << val << " v";
        if (val == 0)
        {
            Zombie* zm_more = new Zombie_Normal(row_ini, col_total - 1);
            garden_pos[zm_more->row].push_back(zm_more);
            garden_pos_cnt[zm_more->row][zm_more->col]++;
            zombie_cnt[zm_more->row]++;
        }//随机在一个地块出现两个僵尸
        //cout << "generate zombie\n";*/
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
        print_garden();
        if (GAME_FAIL)
        {
            SetPos(hOutput, 0, 27);
            cout << "Game Failed!Zombies win!\n";
            getchar(); getchar();
            ClearConsole(hOutput);
            cout << "Game End";
            //adjust format            
            break;
        }//在状态更新并显示完成后判断是否结束
        random_generate_zom();
        turn++;
        //print_garden();
        for (int i = 0; i < row_total; ++i)
        {
            //for (int j = 0; j < col_total; ++j)
            for(auto ptr:garden_pos[i])
            {
                
                //if (!garden_pos[i][j]) continue;
                if (!ptr) continue;
                Plant* plt = (Plant*)ptr;//(garden_pos[i][j]);
                Zombie* zom = (Zombie*)ptr;//(garden_pos[i][j]);
                if (plt->type == 'p' && plt->func_type == "Shooter")
                {
                    Shooter* shot = (Shooter*)plt;
                    //for (int k = j + 1; k < col_total; ++k)
                    for (auto nptr : garden_pos[i])
                    {
                        //该行有检测到僵尸且僵尸在植物右侧就发射子弹
                        //if (garden_pos[i][k] && ((Zombie*)(garden_pos[i][k]))->type == 'z')
                        if (nptr != ptr && nptr != NULL)
                        {
                            if (((Zombie*)nptr)->type != 'z' || ((Zombie*)nptr)->col < shot->col) 
                                continue;
                            //((Shooter*)plt)->attacking((Zombie*)(garden.garden_pos[i][k]));                                                   
                            shot->attacking();
                            //子弹
                            for (vector<Bullet*>::iterator it = shot->bullet_set.begin(); it != shot->bullet_set.end();)
                            {
                                if ((*it)->col > Y_MAX)
                                {
                                    it = shot->bullet_set.erase(it);
                                }//清除飞出界外的子弹
                                else if ((*it)->col - 1 == ((Zombie*)nptr)->col
                                    || (*it)->col == ((Zombie*)nptr)->col)
                                    //(garden_pos[i][k])
                                {
                                    (*it)->attacking((Zombie*)nptr);
                                    if (shot->plant_name == "Frozen Shooter")
                                        ((Zombie*)nptr)->reset_speed(10);
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
                if (plt->type == 'p' && plt->func_type == "Bomb")
                {
                    if (plt->plant_name == "Squash")
                        ((Squash*)plt)->attacking(this->garden_pos);
                    else if (plt->plant_name == "Cherry Bomb")
                        ((Cherry_Bomb*)plt)->attacking(this->garden_pos);
                }
                if (zom->type == 'z')
                {
                    if (zom->zombie_name == "Jester Zombie")
                        ((Jester_Zombie*)zom)->self_attacking(garden_pos);
                    if (garden_pos_cnt[zom->row][zom->col]!=0)
                        //(garden_pos[zom->row][zom->col - 1])
                    {
                        for (auto ptr : garden_pos[zom->row])
                        {
                            if (!ptr)
                                continue;
                            Plant* plt = (Plant*)ptr;
                                //(Plant*)(garden_pos[zom->row][zom->col - 1]);
                            //if (plt->type == 'p')
                            
                            //找到左侧的植物 
                            if (zom->zombie_name == "Stone Zombie" && plt->type == 'p')
                            {
                                plt->get_hurt((Stone_Zombie*)zom);
                            }
                            if( plt->type == 'p' && plt->col == zom->col - 1)//僵尸遇到了植物
                            {     
                                if (zom->zombie_name == "Pole Zombie")
                                {
                                    if (((Pole_Zombie*)zom)->pole > 0)
                                        ((Pole_Zombie*)zom)->jump(garden_pos_cnt);
                                }
                                //if
                                else
                                    plt->get_hurt(zom);
                                // COORD output = { 32,24 };
                                 //SetConsoleCursorPosition(hOutput, output);
                                 //cout << "Zombie attacked! ";
                            }
                        }
                        
                    }
                }
            }
        }//植物和僵尸互相攻击，基于花园提供的位置信息来调用攻击的函数

        //植物和僵尸的状态更新        
        for (int i = 0; i < row_total; ++i)
        {
            map<pair<int, int>, int> bullet_adjust;
            for (vector<void*>::iterator it = garden_pos[i].begin(); it != garden_pos[i].end();)
            {
                if ((*it) != NULL && ((Plant*)(*it))->type == 'p')//植物更新
                {
                    if (((Plant*)(*it))->life <= 0)
                    {
                        garden_pos_cnt[((Plant*)(*it))->row][((Plant*)(*it))->col]--;
                        it = garden_pos[i].erase(it);//清除死亡的植物
                    }                       
                    else if (((Shooter*)(*it))->func_type == "Shooter")//更新子弹位置
                    {
                        Shooter* shot = ((Shooter*)(*it));
                        for (vector<Bullet*>::iterator iter = shot->bullet_set.begin(); iter != shot->bullet_set.end();)
                        {
                            if ((*iter)->col >= Y_MAX)
                            {
                                iter = shot->bullet_set.erase(iter);
                            }//清除已经飞出界外的子弹
                            else
                            {
                                if (bullet_adjust.count(make_pair((*iter)->row, (*iter)->col)) == 0)
                                    bullet_adjust[make_pair((*iter)->row, (*iter)->col)] = 1;
                                else
                                    bullet_adjust[make_pair((*iter)->row, (*iter)->col)]++;
                                (*iter)->move(garden_pos);
                                (*iter)->print_bullet(hOutput, bullet_adjust[make_pair((*iter)->row, (*iter)->col)]);
                                iter++;
                            }//自动移动并且打印
                        }
                        it++;
                    }
                    else
                        it++;
                }
                else if ((*it) != NULL && ((Zombie*)(*it))->type == 'z')//僵尸更新
                {
                    if (((Zombie*)(*it))->life <= 0)
                    {
                        garden_pos_cnt[((Zombie*)(*it))->row][((Zombie*)(*it))->col]--;
                        get_score(10);//之后可以设置成不同的分数值
                        zombie_counting(-1, i);
                        COORD output = { 7 + 15 * ((Zombie*)(*it))->col,3 + 7 * i };
                        if (output.X <= 119)
                        {
                            SetConsoleCursorPosition(hOutput, output);
                            cout << "Zombie died!";
                        }
                        it = garden_pos[i].erase(it);//清除死亡的僵尸
                        
                    }//清除死亡的僵尸并且计分/更新僵尸数 
                    else
                    {
                        bool blocked = false;
                        for (auto ptr : garden_pos[i])
                        {
                            if (ptr != NULL && ptr!= (*it))
                            {
                                if (((Plant*)ptr)->col == ((Zombie*)(*it))->col - 1 && ((Plant*)ptr)->type == 'p')
                                {
                                    blocked = true;
                                    break;
                                }
                            }
                        }
                        if(!blocked)
                            ((Zombie*)(*it))->move(garden_pos_cnt, i, ((Zombie*)(*it))->col);
                        it++;
                    }//僵尸移动
                }
                else 
                {
                    it++;
                }
              
            }
        }
        ReadConsoleInput(hIn, &Rec, 1, &res);
        if (Rec.EventType == KEY_EVENT && Rec.Event.KeyEvent.bKeyDown &&
            Rec.Event.KeyEvent.uChar.AsciiChar == 'b' || Rec.Event.KeyEvent.uChar.AsciiChar == 'B')
        {
            open_shop(hIn);
        }//改进：利用多线程来避免类似”轮循“的操作              
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
                vector<char> shop_list{ 'a','b','c','d' ,'e','f','g','h'};
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
                                    //garden_pos[row][col] = sunflower;
                                    garden_pos[row].push_back(sunflower);
                                    garden_pos_cnt[row][col]++;
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
                                    //garden_pos[row][col] = shot;
                                    garden_pos[row].push_back(shot);
                                    garden_pos_cnt[row][col]++;
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
                            case 'c':
                            {
                                Double_Shooter* dshot = new Double_Shooter(row,col);
                                if (sun_deposit > dshot->sun_price)
                                {
                                    //garden_pos[row][col] = shot;
                                    garden_pos[row].push_back(dshot);
                                    garden_pos_cnt[row][col]++;
                                    cout << "Double Shooter!\n";
                                    sun_deposit -= dshot->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete dshot;
                                }
                                break;
                            }
                            case 'd':
                            {
                                Frozen_Shooter* dshot = new Frozen_Shooter(row, col);
                                if (sun_deposit > dshot->sun_price)
                                {
                                    //garden_pos[row][col] = shot;
                                    garden_pos[row].push_back(dshot);
                                    garden_pos_cnt[row][col]++;
                                    cout << "Frozen Shooter!\n";
                                    sun_deposit -= dshot->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete dshot;
                                }
                                break;
                            }
                            case 'e':
                            {
                                Nut* nt = new Nut(row, col);
                                if (sun_deposit > nt->sun_price)
                                {
                                    //garden_pos[row][col] = shot;
                                    garden_pos[row].push_back(nt);
                                    garden_pos_cnt[row][col]++;
                                    cout << "Nut!\n";
                                    sun_deposit -= nt->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete nt;
                                }
                                break;
                            }
                            case 'f':
                            {
                                High_Nut* nt = new High_Nut(row, col);
                                if (sun_deposit > nt->sun_price)
                                {
                                    //garden_pos[row][col] = shot;
                                    garden_pos[row].push_back(nt);
                                    garden_pos_cnt[row][col]++;
                                    cout << "High Nut!\n";
                                    sun_deposit -= nt->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete nt;
                                }
                                break;
                            }
                            case 'g':
                            {
                                Squash* nt = new Squash(row, col);
                                if (sun_deposit > nt->sun_price)
                                {
                                    //garden_pos[row][col] = shot;
                                    garden_pos[row].push_back(nt);
                                    garden_pos_cnt[row][col]++;
                                    cout << "Squash!\n";
                                    sun_deposit -= nt->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete nt;
                                }
                                break;
                            }
                            case 'h':
                            {
                                Cherry_Bomb* nt = new Cherry_Bomb(row, col);
                                if (sun_deposit > nt->sun_price)
                                {
                                    //garden_pos[row][col] = shot;
                                    garden_pos[row].push_back(nt);
                                    garden_pos_cnt[row][col]++;
                                    cout << "Cherry Bomb!\n";
                                    sun_deposit -= nt->sun_price;
                                }
                                else
                                {
                                    cout << "You are so poor!\n";
                                    delete nt;
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
