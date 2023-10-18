#ifndef LITERAL_CLASS_TIMER_H
#define LITERAL_CLASS_TIMER_H

#include "Time.h"

namespace Lit
{
	class Timer
	{
		Time _start_time{};

	public:
		Timer() = default;
		//Returns the Amount of time that has elapsed since the last Restart 
		Time get_elapsed_time() const;
		//Restarts the Timer
		//Returns the Amount of time that has elapsed since the last Restart 
		Time restart();

	};
}

#endif
