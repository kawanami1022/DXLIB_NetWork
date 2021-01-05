#include <iostream>
#include <DxLib.h>
#include "../map/map.h"
#include "explosion.h"

explosion::explosion():Actor(),dir_(Dir::Down)
{
	Init();
}

explosion::explosion(Position2 pos, int dst, Dir dir):Actor(pos), dst_(dst), dir_(dir),isGenerate_(false)
{
	Init();
}

explosion::~explosion()
{
}

void explosion::Update()
{
	now= std::chrono::system_clock::now();
	elapsedTime_ = now - generateTime_;


	if (elapsedTime_ >= std::chrono::milliseconds(1000/6*7))
	{
		state_ = EXP_STATE::DEAD;
	}
	
}

void explosion::Draw()
{

	DrawExtendGraph(pos_.x, pos_.y, pos_.x + 32, pos_.y + 32, handle_[0][0], true);

}

void explosion::Init()
{
	std::cout << "explosion" << std::endl;
	handleData_.resize(3 * 4);
	for (int idx = 0; idx < 4; idx++)
	{
		handle_.emplace_back(&handleData_[idx * 3]);
	}
	LoadDivGraph("Image/fire.png", 3 * 4, 3, 4, 20, 20, &handleData_[0], true);

	generateTime_ = std::chrono::system_clock::now();

	pos_ = pos_ / TileSize * TileSize;
	state_ = EXP_STATE::ALIVE;

	if (dst_ >= 2)
	{
		isGenerate_ = true;
	}
}
