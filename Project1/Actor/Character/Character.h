#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include "../Actor.h"

constexpr auto CHAR_WIDTH = 32;
constexpr auto CHAR_HEIGHT = 32;
constexpr auto UNIT_ID_BASE = 5;		// PlayerのiDを5刻みにする

class Map;
struct controller;
enum class NetWorkMode;

// 先頭のIDにdefをセットする
// host	0 def
//			5  net
//			10 auto
//			15 net 
//			20 auto
//			25 net

//gest 0  net
//       5  def
//		  10 net
//		  15 auto 
//		  20 net
//		  25 auto

// DEFF (5):入力 自分の場合(相手側からするとNET)
// NET(10~)  : ネットワーク

enum class AnimState
{
	Idel,
	Walk,
	Max
};

enum class MoveDir
{
	Down,
	Left,
	Right,
	Up,
	Deth,
	Max,
};

class Character :public Actor
{	

public:
	Character();
	Character(Position2);
	~Character();
	void Update(std::weak_ptr<Map> map);

	void DeffUpdate(std::weak_ptr<Map> map);			// コントローラーやキーボードで移動
	void NetUpdate(std::weak_ptr<Map> map);			// 上の情報をネットワークから受けとる
	void AutoUpdate(std::weak_ptr<Map> map);			// オートパイロット
	void Draw();

	void AutoMove(std::weak_ptr<Map>&& map);

	bool SendCharData();

	// Getter Setter

	void SetPos(Position2 pos)
	{
		pos_ = pos;
		AdjustPos();
	}

	void SetDir(MoveDir moveDir)
	{
		moveDir_ = moveDir;
	}

	Position2 GetPos()
	{
		return pos_;
	}

	MoveDir GetMoveDir()
	{
		return moveDir_;
	}

protected:
	Vector2Tmp<float> speed = { 1.f,1.f };
	std::vector<int> HandleId_;
	std::vector<int*> HandleData_;

	std::unique_ptr<controller> controller_=nullptr;

	MoveDir moveDir_ = MoveDir::Right;
	Position2 posUL_;
	Position2 posDR_;

	std::unordered_map<NetWorkMode,std::function<void()>> InitFunc;
	std::function<void(std::weak_ptr<Map >)> updateFunc_=nullptr;
private:
	static int Id_;
	int playerID_;
	int animcnt_ = 0;
	bool Init();

	// 位置調整
	void AdjustPos()
	{
		posUL_ = pos_ - Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
		posDR_ = pos_ + Position2(CHAR_WIDTH / 2-1, CHAR_HEIGHT / 2-1);
	}
};

