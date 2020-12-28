#include <iostream>
#include <cmath>
#include <DxLib.h>
#include "../../../NetWork/NetWork.h"
#include "../../../NetWork/NetWorkState.h"
#include "../../../Manager/ImageManager.h"
#include "Bomb.h"
#include "../../map/map.h"

int Bomb::ID_ = 0;

Bomb::Bomb():time_(0.f),bombState_(BOMB_STATE::COUNT_DOWN)
{
	ID_ += 1;
	Init();
}

Bomb::Bomb(Position2 pos) : Item(pos), bombState_(BOMB_STATE::COUNT_DOWN)
{
	ID_ += 1;
	std::cout << "----------------------setBomb----------------------" << std::endl;

	Init();

}

Bomb::~Bomb()
{
	for (auto& HANDLE : handleData_)
	{
		DeleteGraph(HANDLE);
	}
}

void Bomb::Update()
{
	auto now = std::chrono::system_clock::now();
	elapsedTime_ = now.time_since_epoch()- exTime_.time_since_epoch();
	//	Œ»ÝŽž‚ª”š”­ŽžŠÔ‚æ‚è‘å‚«‚¯‚ê‚Î”š”­v
#ifdef DEBUG
	std::cout << std::dec<< std::chrono::duration_cast<std::chrono::seconds>(elapsedTime_).count() << std::endl;
#endif // DEBUG
	if (elapsedTime_ >= std::chrono::seconds(0))
	{
		bombState_ = BOMB_STATE::IGNITED;
	}
	if (elapsedTime_ >= std::chrono::milliseconds(500))
	{
		bombState_ = BOMB_STATE::EXPROSION;
	}
	if (elapsedTime_ >= std::chrono::seconds(6))
	{
		bombState_ = BOMB_STATE::DETH;
	}
}

void Bomb::Draw()
{
	animCnt_++;
	if (bombState_ == BOMB_STATE::COUNT_DOWN)
	{
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + 32, pos_.y + 32, handle_[animCnt_ / 15 % 2][static_cast<int>(BOMB_STATE::COUNT_DOWN)], true);
	}

	if (bombState_ == BOMB_STATE::IGNITED)
	{
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + 32, pos_.y + 32, handle_[animCnt_ / 30 % 2][static_cast<int>(BOMB_STATE::IGNITED)], true);
	}

	if (bombState_ == BOMB_STATE::EXPROSION)
	{
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + 32, pos_.y + 32, handle_[1][(std::abs(std::chrono::duration_cast<std::chrono::seconds>(elapsedTime_).count())) % 6 + 1], true);
	}
}

void Bomb::Init()
{
	handleData_.resize(2 * 7);
	for (int idx = 0; idx < 7; idx++)
	{
		handle_.emplace_back(&handleData_[idx * 2]);
	}
	
#ifdef DEBUG
	IpImage.LoadDivTexture("BOMB", "Image/bomb.png", 2, 7, 14);
#endif // DEBUG
	
	pos_ = pos_ / TileSize * TileSize;
	LoadDivGraph("Image/bomb.png", 14, 2, 7, 20, 20, &handleData_[0], true);
#ifdef DEBUG
	for (auto HDLDATA : handleData_)
	{
		std::cout << HDLDATA << std::endl;
	}
#endif // DEBUG
	bombState_ = BOMB_STATE::COUNT_DOWN;
	setTime_= std::chrono::system_clock::now();
	exTime_ = setTime_+std::chrono::seconds(3);
	elapsedTime_= setTime_.time_since_epoch() - exTime_.time_since_epoch();
}
