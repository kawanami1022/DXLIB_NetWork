#pragma once
#include <chrono>
#include <vector>
#include "../Actor.h"

using timePoint = std::chrono::system_clock::time_point;

enum class EXP_STATE
{
	ALIVE,
	DEAD,
};

class explosion :public Actor
{
public:
	explosion();
	explosion(Position2,int);
	~explosion();

	void Update();
	void Draw();

	// Getter
	EXP_STATE GetState()
	{
		return state_;
	}

	timePoint::duration GetElTime()
	{
		return elapsedTime_;
	}

	int GetDst()
	{
		return dst_;
	}

private:

	int ID_;
	int dst_;
	EXP_STATE state_;
	std::vector<int*> handle_;
	std::vector<int> handleData_;

	timePoint generateTime_;		// effectê∂ê¨éûä‘
	timePoint now;
	timePoint::duration elapsedTime_;	// åoâﬂéûä‘

	void Init();

};

