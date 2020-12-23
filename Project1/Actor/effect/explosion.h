#pragma once
#include <chrono>
#include <vector>
#include "../Actor.h"

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

private:

	int ID_;
	int dst_;
	EXP_STATE state_;
	std::vector<int*> handle_;
	std::vector<int> handleData_;

	std::chrono::system_clock::time_point generateTime_;		// effectê∂ê¨éûä‘
	std::chrono::system_clock::time_point now;
	std::chrono::system_clock::time_point::duration elapsedTime_;	// åoâﬂéûä‘

	void Init();

};

