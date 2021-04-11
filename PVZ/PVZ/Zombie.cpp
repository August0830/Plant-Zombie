#include <iostream>
#include "GardenBoard.h"
extern bool GAME_FAIL;
using namespace std;
void Zombie::get_hurted(int attack_value)
{
	life -= attack_value;
	//cout << "zombie life " << life<<endl;
}
bool Zombie::move(vector<vector<void*>>& garden_pos, int x0, int y0)
{
    int newy = y0 - speed;
    if (garden_pos[x0][newy] == NULL)//只能走空的地块 遇到非空地块被阻塞
    {
        this->col = newy;
        swap(garden_pos[x0][newy], garden_pos[x0][y0]);
        //花园的信息和僵尸的信息同时维护
        //garden_pos[x0][y0] = NULL;
    }
    if (newy <= 0)
        GAME_FAIL = true;
    return true;
}