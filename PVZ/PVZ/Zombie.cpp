#include <iostream>
#include "GardenBoard.h"
extern bool GAME_FAIL;
using namespace std;
int z_normal_att = 5;
int z_normal_life = 25;
int z_normal_speed = 2;
void Zombie::get_hurted(int attack_value)
{
	life -= attack_value;
	//cout << "zombie life " << life<<endl;
}
bool Zombie::move(vector<vector<int>>& garden_pos_cnt, int x0, int y0)
{
    //cout << " sin "<< speed;
    if (speed < 1)
    {
        double actual_speed = 1 / speed;
        int newy = y0 - (int)actual_speed;
        if (garden_pos_cnt[x0][newy] < unit_max)//遇到已经满了的地块被阻塞
        {
            garden_pos_cnt[x0][y0]--;
            garden_pos_cnt[x0][newy]++;
            this->col = newy;
            //swap(garden_pos_cnt[x0][newy], garden_pos[x0][y0]);
            //花园的信息和僵尸的信息同时维护
            //garden_pos[x0][y0] = NULL;
        }
        if (newy <= 0)
            GAME_FAIL = true;
    }
    if (turn % speed == 0)
    {
        int newy = y0 - 1;
        if (garden_pos_cnt[x0][newy] < unit_max)//遇到已经满了的地块被阻塞
        {
            garden_pos_cnt[x0][y0]--;
            garden_pos_cnt[x0][newy]++;
            this->col = newy;
            //swap(garden_pos_cnt[x0][newy], garden_pos[x0][y0]);
            //花园的信息和僵尸的信息同时维护
            //garden_pos[x0][y0] = NULL;
        }
        if (newy <= 0)
            GAME_FAIL = true;
    }
    
    return true;
}
void Paper_Zombie::print_Z()
{
    if (life <= 30)
        cout << "Angry Zombie " << life;
    else
        cout << "Paper Zombie " << life;
}
void Paper_Zombie::change_state()
{
    if (life <= 30)
        speed =  1;
}
void Jester_Zombie::self_attacking(vector<vector<void*>>& garden_pos)
{
    if (rand() % 7 == 0)//
    {
        for (auto line : garden_pos)
        {
            for (auto ptr : line)
            {
                if (ptr && ((Plant*)ptr)->type == 'p')
                {
                    if (((Plant*)ptr)->col <= col + 1 &&
                        ((Plant*)ptr)->col >= col - 1 &&
                        ((Plant*)ptr)->row >= row - 1 &&
                        ((Plant*)ptr)->row <= row + 1)
                        ((Plant*)ptr)->life = -1;
                }
            }
        }
        life = -1;
    }
}