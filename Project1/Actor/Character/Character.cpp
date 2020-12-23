#include <list>
#include <functional>
#include <array>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <DxLib.h>
#include "../../NetWork/NetWork.h"
#include "../../NetWork/NetWorkState.h"
#include "../../input/controller.h"
#include "../../input/keyInput.h"
#include "../../input/Pad.h"
#include "../../Scene/BaseScene.h"
#include "../../Scene/GameScene.h"
#include "../map/map.h"
#include "Character.h"
int Character::Id_ = 0;

Character::Character()
{
	Init();

}

Character::Character(Position2 pos) : Actor(pos)
{
	Init();
}

Character::~Character()
{

}

void Character::Update(std::weak_ptr<Map> map)
{
	if (charState_ ==CharState::Alive)
	{
		updateFunc_(map);
	}
}

void Character::DeffUpdate(std::weak_ptr<Map> map)
{

	using PairID = std::pair<InputID, int>;
	std::vector<std::pair<InputID, int>> inputList;
	if (controller_ == nullptr)
	{
		controller_ = std::make_unique<KeyInput>();
		controller_->Setup(0);
	}

	(*controller_)();

	for (auto CONT_DATA : controller_->GetCntData())
	{
		inputList.emplace_back(CONT_DATA.first, CONT_DATA.second[static_cast<int>(Trg::Now)]);
	}

	std::sort(inputList.begin(), inputList.end(), [&](PairID x, PairID y) {return x.second < y.second;});

	// MapIdを取得する
	auto MapId = [&](Position2 pos) {
		return map.lock()->GetMapId(pos);
	};
	std::vector<Position2> posList;

	auto IsPos = [&](std::vector<Position2> list) {
		for (auto POS_LIST : list)
		{
			if (MapId(POS_LIST) != MAP_ID::NON)
			{
				return false;
			}
		}
		return true;
	};

	bool process = false;	// true:処理を継続させる
	std::for_each(inputList.crbegin(), inputList.crend(), [&](auto&& list) {
		if (process)return;
		Position2 pos= pos_;

		if (list.second == 0) { return; };
		if (list.first == InputID::Down)
		{
			MapId(pos);
			pos_.y += speed.y;
		}
		if (list.first == InputID::Up)
		{
			MapId(pos);
			pos_.y -= speed.y;
		}
		if (list.first == InputID::Left)
		{
			MapId(pos);
			pos_.x -= speed.x;
		}
		if (list.first == InputID::Right)
		{
			MapId(pos);
			pos_.x += speed.x;
		}
	});
	SendCharData();
}

void Character::NetUpdate(std::weak_ptr<Map> map)
{

}

void Character::AutoUpdate(std::weak_ptr<Map> map)
{
	AutoMove(std::move(map));

	SendCharData();
}


void Character::Draw()
{
	animcnt_++;
	if (charState_ == CharState::Alive)
	{
		DrawExtendGraph(pos_.x, pos_.y - 21, pos_.x + CHAR_WIDTH, pos_.y + CHAR_HEIGHT - 21, HandleData_[animcnt_ / 20 % 4][static_cast<int>(moveDir_)], true);
	}
}

void Character::AutoMove(std::weak_ptr<Map>&&  map)
{
	

	// 移動方向を決める
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
}

bool Character::SendCharData()
{
	// offlineMode だったら終了
	if (IpNetWork->GetNetWorkMode() == NetWorkMode::OFFLINE)return false;

	std::vector<int> sendData =
	{ static_cast<int>(MesType::POS),
		playerID_,
		pos_.x,
		pos_.y,
		static_cast<int>(moveDir_)
	};


	for (auto SEND_DATA : sendData)
	{
		IpNetWorkState->SetSendPacket(SEND_DATA);
	}

	return true;
}

void Character::SetUpdateFunc(std::function<void(std::weak_ptr<Map >)> func)
{
	updateFunc_ = func;
}

bool Character::Init()
{
	const int width = 5;
	const int height = 4;

	// Init Graphices handle
	HandleId_.resize(width * height);
	for (int idx = 0; idx < height; idx++)
	{HandleData_.emplace_back(&HandleId_[width * idx]);}
	LoadDivGraph("Image/bomberman.png", width * height, width, height, 20, 32, HandleData_[0], true);

	// Init ID
	//@param num	0偶数 1:奇数
	//@param deff:	入力用更新関数
	//@param func1	更新関数
	//@param func2	更新関数
	auto InitFunc = [&](int num, std::function<void(std::weak_ptr<Map>)> deff, std::function<void(std::weak_ptr<Map> )> func1, std::function<void(std::weak_ptr<Map> )> func2)
	{
		bool isPlayer =( (playerID_ / UNIT_ID_BASE) % 2 == num);
		if (playerID_ <= UNIT_ID_BASE)
		{
			updateFunc_ = (isPlayer) ? deff : func1;
			if (isPlayer) {
				controller_ = std::make_unique<KeyInput>(); 
				controller_->Setup(0);
			}
			return;
		}
		updateFunc_ = ((playerID_ / UNIT_ID_BASE) % 2 == num) ? func1 : func2;
	};

	playerID_ = Id_;
	
	// 旧式のキャラクターのupdate関数の初期化
	auto mode = IpNetWorkState->GetNetWorkMode();

	if (mode == NetWorkMode::OFFLINE) {
		InitFunc(0, std::bind(&Character::DeffUpdate,this, std::placeholders::_1), std::bind(&Character::AutoUpdate, this, std::placeholders::_1), std::bind(&Character::AutoUpdate, this, std::placeholders::_1));
	}
	else if (mode == NetWorkMode::GUEST)
	{
		auto id = IpNetWorkState->GetPlID(); 
		updateFunc_ = (id == IpNetWorkState->GetPlID()) ? std::bind(&Character::DeffUpdate, this, std::placeholders::_1) : std::bind(&Character::NetUpdate, this, std::placeholders::_1);
	}
	else if (mode == NetWorkMode::HOST)
	{
		updateFunc_ = std::bind(&Character::NetUpdate, this, std::placeholders::_1);
		if (Id_ == 0) 
		{
			updateFunc_ = std::bind(&Character::DeffUpdate, this, std::placeholders::_1);
			controller_ = std::make_unique<KeyInput>();
			controller_->Setup(0);
		};
	}

	Id_ += UNIT_ID_BASE;
	return false;
}
//@param plPos	playerの位置
void Character::MatchGridPos(Position2 plPos)
{

}
