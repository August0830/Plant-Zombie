#include <iostream>
#include "GardenBoard.h"
extern bool GAME_FAIL;
using namespace std;
void Zombie::get_hurted(int attack_value)
{
	life -= attack_value;
	//cout << "zombie life " << life<<endl;
}
bool Zombie::move(vector<vector<int>>& garden_pos_cnt, int x0, int y0)
{
    //cout << " sin "<< speed;
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