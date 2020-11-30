#pragma once
#include "../Actor.h"
class Item :
	public Actor
{
public:
	Item();
	~Item();
	void Update();
	void Draw();
};

