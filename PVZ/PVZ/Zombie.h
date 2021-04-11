#pragma once
#include "Plant.h"
//#include "GardenBoard.h"
class Zombie
{
	friend class GardenBoard;
private:
	int life;
	int attack;
	int speed;
	//有多种僵尸之后再命名

public:
	int row;
	int col;
	char type;
	Zombie(int r, int c) :life(25), attack(5), speed(X_INTERVAL),type('z')
	{ row = r; col = c; }
	Zombie(int r,int c,int _life, int _att, int _speed):type('z')
	{
		life = _life; attack = _att; speed = _speed; row = r; col = c;
	}
	void get_hurted(int attack_value);//便于之后增加其他带有攻击性的植物
	bool move();
	int attacking()const
	{ return attack; }
	int get_row()const { return row; }
	int get_col()const { return col; }
};
