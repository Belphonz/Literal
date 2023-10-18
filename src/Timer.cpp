#include "Literal/Timer.h"

Lit::Time Lit::Timer::get_elapsed_time() const
{
	return Time() - _start_time;
}

Lit::Time Lit::Timer::restart()
{
	const Time elapsed_time = Time() - _start_time;
	_start_time = Time();
	return elapsed_time;
}
