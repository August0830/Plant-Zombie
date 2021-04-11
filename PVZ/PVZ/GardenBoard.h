#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <Windows.h>
#define WH_KEYBOARD_LL 13

//#include "Zombie.h"
//#include "Plant.h"
const int X_INTERVAL = 15;
const int  Y_INTERVAL = 7;
extern int shooter_attack_val;
extern int shooter_life_val;
extern int shooter_sun_price_val;
extern int bullet_fly_speed;
using namespace std;
class Zombie;
class Plant;
class Bullet;
class GardenBoard
{
	friend class Plant;
	friend class Zombie;
	friend class Shooter;
	friend class Bullet;
private:
	//vector<vector<void*>> garden_pos;调试方便暂时公开
	
	int row_total;//3 0~2
	int col_total;//8 0~7
	//int sun_deposit;//拥有的太阳数 暂时公开
	int point_cnt;//计分 
	//int sun_deposit;//拥有的太阳数
	vector<int> zombie_cnt;//现有僵尸总数  根据每一行来记录
	int zombie_max;//限定僵尸总数可以根据游戏的难度来调整
	//int sunflower_cnt;
public:
	int sunflower_cnt;
	int sun_deposit;
	HANDLE hOutput;
	HANDLE hIn;
	vector<vector<void*>> garden_pos;
	GardenBoard(int row_total, int col_total,HANDLE hIn,HANDLE hOutput) :point_cnt(0), sun_deposit(100),zombie_max(4),
		sunflower_cnt(0)
	{
		for (int i = 0; i < row_total; ++i)
		{
			vector<void*> tmp(col_total);
			garden_pos.push_back(tmp);
		}//此处填满了NULL指针
		zombie_cnt.resize(row_total, 0);
		this->row_total = row_total;
		this->col_total = col_total;
		this->hIn = hIn;
		this->hOutput = hOutput;
	};
	void print_garden();//打印面板 
	void SetPos(HANDLE HOutput,int x, int y);
	bool ClearConsole(HANDLE hOut);//refresh
	void refresh_state(HANDLE& hOutput, HANDLE& hIn, DWORD start);//刷新位置 让植物和僵尸互相攻击 位置判断在花园中完成
	//消除死去的植物和僵尸并且对僵尸计分
	
	//在捕获相应键盘之后 购买植物 这里的位置与row/col_total保持一致 是格子数的意思
	void generate_sun();//refresh时更新太阳数，一开始按一定数目递增 增加向日葵后与向日葵数成正比
	void get_score(int score) { point_cnt += score; }
	void zombie_counting(int cnt,int row){ zombie_cnt[row] += cnt; }
	bool random_generate_zom();//生成僵尸
	void open_shop(HANDLE& hIn);
};


class Plant
{
	friend class GardenBoard;
public:
	int row;
	int col;
	int life;
	int sun_price;
	char type;
	string plant_name;
	Plant() :row(0), col(0), life(30), sun_price(0), type('p'), plant_name("plant") {};
	int get_col()const { return col; }
	void get_hurt(Zombie* zombie);
	int price()const { return sun_price; }
};
class Sunflower :public Plant
{
	friend class GardenBoard;
public:
	Sunflower(int r, int c) {
		row = r; col = c; plant_name = "Sunflower";
		sun_price = 10;
		life = 20;
	}

};
class Shooter :public Plant//
{
	friend class GardenBoard;
	friend class Bullet;
private:
	int attack;
public:
	vector<Bullet*> bullet_set;//暂时public 之后给garden调用
	Shooter(int r, int c) :attack(shooter_attack_val)
	{
		row = r;
		col = c;
		life = shooter_life_val;
		sun_price = shooter_sun_price_val;
		plant_name = "Shooter";
		bullet_set.clear();
	}
	void attacking();//通过产生子弹来攻击
	int get_row()const { return row; }
	int get_col()const { return col; }	
};
class Bullet
{
	friend class Shooter;
	friend class GardenBoard;
private:
	int row;
	int col;
	int attack;//子弹的攻击值由植物（射手）决定
	int fly_speed;
public:
	Bullet() :row(0), col(0), attack(9),fly_speed(bullet_fly_speed) {};
	Bullet(Shooter* shot,DWORD fly) 
	{ 
		row = shot->row; 
		col = shot->col; 
		attack = shot->attack;
		fly_speed= fly;
	}
	void move(vector<vector<void*>>& garden_pos);
	void print_bullet(HANDLE hOutput);
	void attacking(Zombie* zombie);
	int get_col()const { return col; }
};

class Zombie
{
	friend class GardenBoard;
private:
	//int life;
	int attack;
	int speed;
	//有多种僵尸之后再命名

public:
	int row;
	int col;
	char type;
	int life;
	Zombie(int r, int c) :life(25), attack(5), speed(1), type('z')
	{
		row = r; col = c;
	}
	Zombie(int r, int c, int _life, int _att, int _speed) :type('z')
	{
		life = _life; attack = _att; speed = _speed; row = r; col = c;
	}
	void get_hurted(int attack_value);//便于之后增加其他带有攻击性的植物
	bool move(vector<vector<void*>>& garden_pos, int x0, int y0);
	int attacking()const
	{
		return attack;
	}
	int get_row()const { return row; }
	int get_col()const { return col; }
};