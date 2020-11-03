#pragma once
#include <chrono>

using ClockPoint = std::chrono::system_clock::time_point;

class Timer
{
public:
	std::chrono::milliseconds IntervalMesurement();
	void StartMesurement();							// Get system_clock::time_point
	std::chrono::milliseconds GetInterval(){
		return interval_;
	}
private:
	std::chrono::milliseconds interval_;
	ClockPoint start_;
	ClockPoint end_;
};

