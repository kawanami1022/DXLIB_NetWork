#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include "../Actor.h"

constexpr auto CHAR_WIDTH = 32;
constexpr auto CHAR_HEIGHT = 32;
constexpr auto UNIT_ID_BASE = 5;		// PlayerのiDを5刻みにする

class Map;
enum class NetWorkMode;

// 先頭のIDにdefをセットする
// host 0 def
//        10 net
//        20 net

//gest 5  def
//		  15 auto 
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

	void DeffUpdate();			// コントローラーやキーボードで移動
	void NetUpdate();				// オートパイロットで移動
	void AutoUpdate();			// 上の情報をネットワークから受けとる
	void Draw();

	void Move(std::weak_ptr<Map>&& map);

	// Getter Setter

	void SetPos(Position2 pos)
	{
		pos_ = pos;
		AdjustPos();
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
	MoveDir moveDir_ = MoveDir::Right;
	Position2 posUL_;
	Position2 posDR_;

	std::unordered_map<NetWorkMode,std::function<void()>> InitFunc;
	std::function<void()> updateFunc;
private:
	static int Id_;
	int animcnt_ = 0;
	bool Init();

	// 位置調整
	void AdjustPos()
	{
		posUL_ = pos_ - Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
		posDR_ = pos_ + Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
	}
};

