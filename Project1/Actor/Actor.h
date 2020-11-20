#pragma once
#include "../Lib/Vector2/Vector2.h"

class Actor
{
public:
	Actor();
	~Actor();

	

	Position2 GetPos()
	{
		return pos_;
	}
protected:
	Position2 pos_;
};

