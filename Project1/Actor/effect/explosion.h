#pragma once
#include <chrono>
#include <vector>
#include <memory>
#include "../Actor.h"

class Map;
using timePoint = std::chrono::system_clock::time_point;

enum class EXP_STATE
{
	ALIVE,
	DEAD,
};

enum class Dir
{
	Center_,
	Down,
	Left,
	Right,
	Up,
};

class explosion :public Actor
{
public:
	explosion();
	explosion(Position2,int, Dir dir);
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

	Dir GetDir()
	{
		return dir_;
	}

	int GetDst()
	{
		return dst_;
	}

	void SetIsGenerate(bool isGenerate)
	{
		isGenerate_ = isGenerate;
	}

	bool GetGenerate()
	{
		return isGenerate_;
	}

private:

	int ID_;
	int dst_;
	Dir dir_;
	bool isGenerate_;	// エフェクト生成 true:生成済み
	EXP_STATE state_;
	std::vector<int*> handle_;
	std::vector<int> handleData_;
	std::weak_ptr<Map> map_;
	timePoint generateTime_;		// effect生成時間
	timePoint now;
	timePoint::duration elapsedTime_;	// 経過時間
	std::vector<int> angle_;
	void Init();

};

