#include "Literal/Timer.h"

Lit::Time Lit::Timer::GetElapsedTime() const
{
	return Time() - _startTime;
}

Lit::Time Lit::Timer::Restart()
{
	const Time elapsed_time = Time() - _startTime;
	_startTime = Time();
	return elapsed_time;
}