#include "map/map.h"
#include "Actor.h"
Actor::Actor()
{
	pos_ = Position2(0, 0);
}

Actor::Actor(Position2 pos):pos_(pos)
{
}

Actor::~Actor()
{
}

void Actor::Update()
{
}

void Actor::Draw()
{
}
