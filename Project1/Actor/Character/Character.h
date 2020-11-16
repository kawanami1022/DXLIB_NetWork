#pragma once
#include <vector>
#include "../Actor.h"

constexpr auto CHAR_WIDTH = 32;
constexpr auto CHAR_HEIGHT = 32;

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
	void Update();
	void Draw();

	void Move();

protected:
	Vector2Tmp<float> speed = { 1.f,1.f };
	std::vector<int> HandleId_;
	std::vector<int*> HandleData_;
	MoveDir moveDir_ = MoveDir::Right;
	Position2 posUL_;
	Position2 posDR_;
private:
	int animcnt_ = 0;
	bool Init();
	
};

