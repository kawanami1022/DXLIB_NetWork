#include <functional>
#include <array>
#include <iostream>
#include <iomanip>
#include <DxLib.h>
#include "../../NetWork/NetWork.h"
#include "../../NetWork/NetWorkState.h"
#include "../../Scene/BaseScene.h"
#include "../../Scene/GameScene.h"
#include "../map/map.h"
#include "Character.h"

int Character::Id_ = 0;

Character::Character()
{
	Init();

}

Character::Character(Position2 pos)
{
	pos_ = pos + 32 / 2;
	Init();
}

Character::~Character()
{

}

void Character::Update(std::weak_ptr<Map> map)
{
	Move(std::move(map));
}

void Character::DeffUpdate()
{
}

void Character::NetUpdate()
{
}

void Character::AutoUpdate()
{
}


void Character::Draw()
{
	animcnt_++;
	DrawRotaGraph(pos_.x, pos_.y,1,0, HandleData_[animcnt_/20%4][static_cast<int>(moveDir_)], true);
}

void Character::Move(std::weak_ptr<Map>&&  map)
{
	

	// ˆÚ“®•ûŒü‚ðŒˆ‚ß‚é
	std::unordered_map<MoveDir, std::pair<Position2,Position2>> tmpPos = 
	{ {MoveDir::Down,std::make_pair<Position2,Position2>(Position2(pos_.x,posDR_.y),(Position2(pos_.x,pos_.y)))},
	{MoveDir::Left,std::make_pair<Position2,Position2>(Position2(posUL_.x,pos_.y),(Position2(pos_.x,pos_.y)))},
	{MoveDir::Up,std::make_pair<Position2,Position2>(Position2(pos_.x,posUL_.y),(Position2(pos_.x,pos_.y)))},
	{MoveDir::Right,std::make_pair<Position2,Position2>(Position2(posDR_.x,pos_.y),(Position2(pos_.x,pos_.y)))}};

	//auto moveFunc = [&](MoveDir dir, std::pair<Position2, Position2> pos) {
	//	pos.first.y = pos.first.y + static_cast<int>(speed.y);
	//	pos.second.y = pos.first.y + static_cast<int>(speed.y);
	//};

	if (moveDir_ == MoveDir::Down) {
		tmpPos[moveDir_].first.y += static_cast<int>(speed.y);
		tmpPos[moveDir_].second.y += static_cast<int>(speed.y);
	}
	else if (moveDir_ == MoveDir::Left)
	{
		tmpPos[moveDir_].first.x -= static_cast<int>(speed.x);
		tmpPos[moveDir_].second.x -= static_cast<int>(speed.x);
	}
	else if (moveDir_ == MoveDir::Right)
	{
		tmpPos[moveDir_].first.x += static_cast<int>(speed.x);
		tmpPos[moveDir_].second.x += static_cast<int>(speed.x);
	}
	else if (moveDir_ == MoveDir::Up)
	{
		tmpPos[moveDir_].first.y -= static_cast<int>(speed.y);
		tmpPos[moveDir_].second.y -= static_cast<int>(speed.y);
	}
	if (map.lock()->IsTurnRight(tmpPos[moveDir_].first))
	{
		moveDir_ = static_cast<MoveDir>((static_cast<int>(moveDir_) + 1) % (static_cast<int>(MoveDir::Deth)));
	}
	else { pos_ = tmpPos[moveDir_].second; };
	AdjustPos();
}

bool Character::Init()
{
	const int width = 5;
	const int height = 4;
	AdjustPos();

	// Init Graphices handle
	HandleId_.resize(width * height);
	for (int idx = 0; idx < height; idx++)
	{HandleData_.emplace_back(&HandleId_[width * idx]);}
	LoadDivGraph("Image/bomberman.png", width * height, width, height, 20, 32, HandleData_[0], true);

	// Init ID

	auto InitFunc = [&](int num, std::function<void()> deff, std::function<void()> func1, std::function<void()> func2) {
		if (playerID_ <= UNIT_ID_BASE)
		{
			updateFunc_ = ((playerID_ % UNIT_ID_BASE) % 2 == num) ? std::bind(&Character::NetUpdate, this) : std::bind(&Character::AutoUpdate, this);
			return;
		}

		if ((playerID_ % UNIT_ID_BASE) % 2 == num)
		{
			updateFunc_ = func1;
		}
		else {
			updateFunc_ = func2;
		}
	};

	playerID_ = Id_;




	Id_ += UNIT_ID_BASE;
	return false;
}
