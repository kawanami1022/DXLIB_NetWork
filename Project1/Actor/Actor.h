#pragma once
#include <memory>
#include "../Lib/Vector2/Vector2.h"

class Actor
{
public:
	Actor();
	Actor(Position2 pos);
	~Actor();

	virtual void Update();
	virtual void Draw();
	
	Position2 GetPos()
	{
		return pos_;
	}
protected:
	Position2 pos_;
};

