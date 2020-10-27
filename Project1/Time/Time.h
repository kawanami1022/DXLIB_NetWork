#pragma once
#include <chrono>
#include <ctime>

class Timer
{
public:
	long double IntervalMesurement();
	void StartMesurement();
	long double GetInterval(){
		return interval_;
	}
private:
	long double interval_; 
	std::time_t	start_;
	std::time_t end_;
};

