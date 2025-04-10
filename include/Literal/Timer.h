#ifndef LITERAL_CLASS_TIMER_H
#define LITERAL_CLASS_TIMER_H

#include <Literal/Time.h>

namespace Lit
{
	//Object that stores Elapsed time
	class Timer
	{
		Time _startTime{};

	public:
		Timer() = default;
		//Returns the Amount of time that has elapsed since the last Restart
		[[nodiscard]] Time GetElapsedTime() const;
		//Restarts the Timer
		//Returns the Amount of time that has elapsed since the last Restart
		Time Restart();
	};
}

#endif
