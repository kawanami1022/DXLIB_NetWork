#pragma once
#include "../Item.h"
class Bomb :
	public Item
{
public:
	Bomb();
	~Bomb();

	void Update();
	void Draw();
private:
	float time_ = 0;
};

