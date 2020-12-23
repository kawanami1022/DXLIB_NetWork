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
	explosion(Position2);
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

	EXP_STATE state_;
	std::vector<int*> handle_;
	std::vector<int> handleData_;

	std::chrono::system_clock::time_point generateTime_;		// effect��������
	std::chrono::system_clock::time_point now;
	std::chrono::system_clock::time_point::duration elapsedTime_;	// �o�ߎ���

	void Init();

};

