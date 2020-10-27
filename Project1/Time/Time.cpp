#include "Time.h"

long double Timer::IntervalMesurement()
{
	end_ = std::time(nullptr);
	return std::difftime(end_, start_);
}

void Timer::StartMesurement()
{
	start_ = std::time(nullptr);
}
