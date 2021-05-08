#include <iostream>
#include "GardenBoard.h"
using namespace std;
extern const int Y_MAX = 7;
int shooter_attack_val = 5;
int shooter_life_val = 30;
int shooter_sun_price_val = 10;
int double_shooter_sun_price_val = 25;
int defense_life_val = 45;
int defense_sun_price_val = 25;
int bullet_fly_speed = 1;
int bomb_sun_price_val = 35;
void Plant::get_hurt(Zombie* zombie)
{
	if (zombie->zombie_name == "Stone Zombie")
		life -= ((Stone_Zombie*)zombie)->attacking();
	else
		life -= zombie->attacking();
		//cout << "Plant life " << life<<endl;
}
void Shooter::attacking()//通过产生子弹来完成
{
	Bullet* bul = new Bullet(this,1);
	bullet_set.push_back(bul);
}
void Double_Shooter::attacking()//通过产生两颗子弹来完成
{
	Bullet* bul = new Bullet(this, 1);
	bullet_set.push_back(bul);
	Bullet* bul2 = new Bullet(this, 1);
	bullet_set.push_back(bul2);
}
void Bullet::move(vector<vector<void*>>& garden_pos)
{
	int max_col = fly_speed;
	for (auto ptr : garden_pos[row])
	{
		if (ptr!=NULL && ((Zombie*)ptr)->type == 'z' && ((Zombie*)ptr)->col < max_col)
			max_col = ((Zombie*)ptr)->col;//离子弹最近的僵尸
			
	}
	for (int i = 0; i < max_col; ++i)
	{
		if (col <= Y_MAX)// &&(((Zombie*)garden_pos[row][col])->type != 'z'))
			col++;
	}
}//判断交给花园，先移动再判断是否需要攻击
void Bullet::print_bullet(HANDLE hOutput,int duplicated)
{
	COORD pos = { 7 + 15 * col-1, 7 * row+duplicated+1 };
	SetConsoleCursorPosition(hOutput, pos);
	cout << "*";// << row << " " << col;
}
void Bullet::attacking(Zombie* zombie)
{
	zombie->get_hurted(attack);
}
void Squash::attacking(vector<vector<void*>> &garden_pos)
{
	for (auto line : garden_pos)
	{
		for (auto ptr : line)
		{
			if (ptr && ((Zombie*)ptr)->type == 'z')
			{
				if (((Zombie*)ptr)->col == col && ((Zombie*)ptr)->row == row)
					((Zombie*)ptr)->get_hurted(1000);
			}
		}
	}
	life =-1;
}
void Cherry_Bomb::attacking(vector<vector<void*>>& garden_pos)
{
	for (auto line : garden_pos)
	{
		for (auto ptr : line)
		{
			if (ptr && ((Zombie*)ptr)->type == 'z')
			{
				if (((Zombie*)ptr)->col <= col+1 &&
					((Zombie*)ptr)->col >= col - 1 &&
					((Zombie*)ptr)->row >= row-1 &&
					((Zombie*)ptr)->row <= row+1)
					((Zombie*)ptr)->get_hurted(1000);
			}
		}
	}
	life = -1;
}