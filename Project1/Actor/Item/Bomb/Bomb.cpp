#include <iostream>
#include <cmath>
#include <DxLib.h>
#include "../../../Manager/ImageManager.h"
#include "Bomb.h"
#include "../../map/map.h"

Bomb::Bomb():time_(0.f),bombState_(BOMB_STATE::COUNT_DOWN)
{
	Init();
}

Bomb::Bomb(Position2 pos):Item(pos), bombState_(BOMB_STATE::COUNT_DOWN)
{
	Init();
}

Bomb::~Bomb()
{

}

void Bomb::Update()
{
	
	auto now = std::chrono::system_clock::now();
	elapsedTime_ = now.time_since_epoch()- exTime_.time_since_epoch();
	//	åªç›éûçèÇ™îöî≠éûä‘ÇÊÇËëÂÇ´ÇØÇÍÇŒîöî≠Åv
#ifdef DEBUG
	std::cout << std::dec<< std::chrono::duration_cast<std::chrono::seconds>(elapsedTime_).count() << std::endl;
#endif // DEBUG
	if (elapsedTime_ >= std::chrono::seconds(0))
	{
		bombState_ = BOMB_STATE::IGNITED;
	}
	if (elapsedTime_ >= std::chrono::seconds(1))
	{
		bombState_ = BOMB_STATE::EXPROSION;
	}
	if (elapsedTime_ >= std::chrono::seconds(7))
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
	for (auto HDLDATA : handleData_)
	{
		std::cout << HDLDATA << std::endl;
	}
	bombState_ = BOMB_STATE::COUNT_DOWN;

	
}
