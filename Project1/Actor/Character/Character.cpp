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
	pos_=pos+32/2;
	Init();
}

Character::~Character()
{

}

void Character::Update()
{
	Move();
}


void Character::Draw()
{
	animcnt_++;
	DrawRotaGraph(pos_.x, pos_.y,1,0, HandleData_[animcnt_/20%4][static_cast<int>(MoveDir::Down)], true);
}

void Character::Move()
{
	if (moveDir_ == MoveDir::Down) {
		pos_.y += static_cast<int>(speed.y);
	}
	else if (moveDir_ == MoveDir::Left)
	{
		pos_.x -= static_cast<int>(speed.x);
	}
	else if (moveDir_ == MoveDir::Right)
	{
		pos_.x += static_cast<int>(speed.x);
	}
	else if (moveDir_ == MoveDir::Up)
	{
		pos_.y -= static_cast<int>(speed.y);
	}
	posUL_ = pos_ - Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
	posDR_ = pos_ + Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
}

bool Character::Init()
{
	const int width = 5;
	const int height = 4;
	posUL_ = pos_ - Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
	posDR_ = pos_ + Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
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
