#pragma once
#include <vector>
#include "../Actor.h"

constexpr auto CHAR_WIDTH = 32;
constexpr auto CHAR_HEIGHT = 32;

class Map;

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
	void Update(std::shared_ptr<Map> map);
	void Draw();

	void Move(std::shared_ptr<Map>&& map);

	// Getter Setter
	void SetMap(std::weak_ptr<Map>&& map)
	{
		map_ = map;

	}
	void SetPos(Position2 pos)
	{
		pos_ = pos;
		AdjustPos();
	}

protected:
	Vector2Tmp<float> speed = { 1.f,1.f };
	std::vector<int> HandleId_;
	std::vector<int*> HandleData_;
	std::weak_ptr<Map> map_;
	MoveDir moveDir_ = MoveDir::Right;
	Position2 posUL_;
	Position2 posDR_;
private:
	int animcnt_ = 0;
	bool Init();

	// à íuí≤êÆ
	void AdjustPos()
	{
		posUL_ = pos_ - Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
		posDR_ = pos_ + Position2(CHAR_WIDTH / 2, CHAR_HEIGHT / 2);
	}
};

