#pragma once
#include <vector>
#include <chrono>
#include "../Item.h"
enum class BOMB_STATE
{
	COUNT_DOWN,		// 爆発前
	IGNITED,					// 着火
	EXPROSION,				// 爆発中
	DETH,						// けしておく
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

	// 爆発時間
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
	std::chrono::system_clock::time_point exTime_;		// 爆発
	std::chrono::system_clock::time_point setTime_;
	std::chrono::system_clock::time_point::duration elapsedTime_;	// 経過時間
	void Init();
};

