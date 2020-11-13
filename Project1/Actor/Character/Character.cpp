#include <iostream>
#include <iomanip>
#include <DxLib.h>
#include "Character.h"
Character::Character()
{
	Init();

}

Character::Character(Position2 pos)
{
	pos_=pos;
	Init();
}

Character::~Character()
{

}

void Character::Draw()
{
	animcnt_++;
	DrawGraph(0, 0, HandleData_[animcnt_/20%4][static_cast<int>(MoveDir::Down)], true);
}

bool Character::Init()
{
	const int width = 5;
	const int height = 4;
	HandleId_.resize(width * height);
	for (int idx = 0; idx < height; idx++)
	{
		HandleData_.emplace_back(&HandleId_[width * idx]);
	}
	LoadDivGraph("Image/bomberman.png", width * height, width, height, 20, 32, HandleData_[0], true);

	for (auto HANDLE : HandleId_)
	{
		std::cout << HANDLE << std::endl;
	}
	return false;
}
