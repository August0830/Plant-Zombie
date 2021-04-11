#pragma once
#include "Zombie.h"
//#include "GardenBoard.h"
#include <iostream>
#include <string>
class Zombie;
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
	Plant() :row(0), col(0), life(0), sun_price(0),type('p'), plant_name("plant") {};
	int get_col()const { return col; }
};
class Shooter:public Plant//
{
	friend class GardenBoard;
private:
	int attack;
public:
	Shooter(int r, int c) :attack(8) 
	{
		row = r; 
		col = c; 
		life = 25; 
		sun_price = 10; 
		plant_name = "Shooter";
	}
	void attacking(Zombie* zombie);
	int get_row()const { return row; }
	int get_col()const { return col; }
	void get_hurted(Zombie* zombie);
};