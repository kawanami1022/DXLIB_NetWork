#include "Time.h"

std::chrono::milliseconds Timer::IntervalMesurement()
{
	end_ = std::chrono::system_clock::now();
	interval_ = std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
	return interval_;
}

void Timer::StartMesurement()
{
	start_ = std::chrono::system_clock::now();
}
