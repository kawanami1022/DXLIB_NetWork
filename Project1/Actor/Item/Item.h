#pragma once
#include "../Actor.h"
class Item :
	public Actor
{
public:
	Item();
	Item(Position2);
	~Item();
	void Update();
	void Draw();
};

