#include <iostream>
#include "GardenBoard.h"
using namespace std;
extern const int Y_MAX = 7;
int shooter_attack_val = 9;
int shooter_life_val = 25;
int shooter_sun_price_val = 10;
int bullet_fly_speed = 1;
void Plant::get_hurt(Zombie* zombie)
{
		life -= zombie->attacking();
		//cout << "Plant life " << life<<endl;
}
void Shooter::attacking()//通过产生子弹来完成
{
	Bullet* bul = new Bullet(this,1);
	bullet_set.push_back(bul);
}
void Bullet::move(vector<vector<void*>>& garden_pos)
{
	for (int i = 0; i < fly_speed; ++i)
	{
		if (col <= Y_MAX &&(!garden_pos[row][col] ||((Zombie*)garden_pos[row][col])->type != 'z'))
			col++;
	}
}//判断交给花园，先移动再判断是否需要攻击
void Bullet::print_bullet(HANDLE hOutput)
{
	COORD pos = { 7 + 15 * col-1,3 + 7 * row };
	SetConsoleCursorPosition(hOutput, pos);
	cout << "*";// << row << " " << col;
}
void Bullet::attacking(Zombie* zombie)
{
	zombie->get_hurted(attack);
}