#pragma once
#include <vector>
#include "../Actor.h"

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
	void Draw();
protected:
	std::vector<int> HandleId_;
	std::vector<int*> HandleData_;
private:
	int animcnt_ = 0;
	bool Init();
};

