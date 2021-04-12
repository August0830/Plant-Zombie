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
	//花园承担了与所有对象交互的功能，需要频繁访问他们的数据，因此设置成友元有助于提高程序运行效率
private:
	int row_total;//3 0~2
	int col_total;//8 0~7
	int sun_deposit;//拥有的太阳数 暂时公开
	int point_cnt;//计分 
	vector<int> zombie_cnt;//现有僵尸总数  根据每一行来记录
	int zombie_max;//限定僵尸总数 可以根据游戏的难度来调整
	int sunflower_cnt;//向日葵数量统计，阳光生成速率与之成正比
public:
	//int sunflower_cnt;
	//int sun_deposit;
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
	void generate_sun();//refresh时更新太阳数，一开始按一定数目递增 增加向日葵后与向日葵数成正比
	void get_score(int score) { point_cnt += score; }//更新记分牌
	void zombie_counting(int cnt,int row){ zombie_cnt[row] += cnt; }//统计现有僵尸数
	bool random_generate_zom();//生成僵尸 可以根据总的僵尸数和每行的僵尸数调整
	void open_shop(HANDLE& hIn);//在捕获相应键盘之后 购买植物 这里的位置与row/col_total保持一致 是格子数的意思
};


class Plant
{
	friend class GardenBoard;
public:
	int row;
	int col;//位置信息
	int life;//生命值
	int sun_price;//购买所需阳光数
	char type;//供花园区分植物和僵尸
	string plant_name;//细分不同的植物类别
	Plant() :row(0), col(0), life(30), sun_price(0), type('p'), plant_name("plant") {};
	int get_col()const { return col; }//供外界调试时获取列坐标
	void get_hurt(Zombie* zombie);//被僵尸攻击
	int price()const { return sun_price; }//供外界调试时获取购买植物所需阳光数
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
	vector<Bullet*> bullet_set;//暂时public 之后给garden调用
public:
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
	int fly_speed;//每回合移动的格子数
public:
	Bullet() :row(0), col(0), attack(9),fly_speed(bullet_fly_speed) {};
	Bullet(Shooter* shot,int fly) 
	{ 
		row = shot->row; 
		col = shot->col; 
		attack = shot->attack;
		fly_speed= fly;
	}
	void move(vector<vector<void*>>& garden_pos);//子弹自己更新位置
	void print_bullet(HANDLE hOutput);//子弹自行打印位置
	void attacking(Zombie* zombie);//子弹攻击僵尸
	int get_col()const { return col; }//供外界调用
};

class Zombie
{
	friend class GardenBoard;
private:
	int life;//生命值
	int attack;//攻击力
	int speed;//移动速度 同样是一回合移动的格数
	//有多种僵尸之后再命名

public:
	int row;
	int col;
	char type;
	Zombie(int r, int c) :life(25), attack(5), speed(1), type('z')
	{
		row = r; col = c;
	}
	Zombie(int r, int c, int _life, int _att, int _speed) :type('z')
	{
		life = _life; attack = _att; speed = _speed; row = r; col = c;
	}
	void get_hurted(int attack_value);//便于之后增加其他带有攻击性的植物 所以只传入攻击值
	bool move(vector<vector<void*>>& garden_pos, int x0, int y0);//自行移动
	int attacking()const
	{
		return attack;
	}//获取攻击值
	int get_row()const { return row; }
	int get_col()const { return col; }//获取位置
};