#pragma once
#include <vector>
#include <chrono>
#include "../Item.h"
enum class BOMB_STATE
{
	COUNT_DOWN,		// ”š”­‘O
	IGNITED,					// ’…‰Î
	EXPROSION,				// ”š”­’†
	DETH,						// ‚¯‚µ‚Ä‚¨‚­
	MAX
};

class Bomb :
	public Item
{
public:
	Bomb();
	Bomb(Position2);
	~Bomb();

	void Update();
	void Draw();

	// Getter
	BOMB_STATE GetBombState()
	{
		return bombState_;
	}

	// Setter

	// ”š”­ŽžŠÔ
	void SetExTime(int exTime)
	{
		
		exTime_ = setTime_ + std::chrono::seconds(exTime);
	}

	void SetSetTime(std::chrono::system_clock::time_point setTime)
	{
		setTime_ = setTime;
	}

	std::chrono::system_clock::time_point::duration GetElapsedTime()
	{
		return elapsedTime_;
	}

private:
	float time_ = 0;
	std::vector<int*> handle_;
	std::vector<int> handleData_;
	BOMB_STATE bombState_ = BOMB_STATE::COUNT_DOWN;
	int animCnt_ = 0;
	std::chrono::system_clock::time_point exTime_;		// ”š”­
	std::chrono::system_clock::time_point setTime_;
	std::chrono::system_clock::time_point::duration elapsedTime_;	// Œo‰ßŽžŠÔ
	void Init();
};

