#ifndef LITERAL_CLASS_TIME_H
#define LITERAL_CLASS_TIME_H

#include <chrono>

namespace Lit
{
	//Object that stores time
	class Time
	{
		std::chrono::time_point<std::chrono::steady_clock> _stored_time{ std::chrono::steady_clock::now() };

	public:
		Time() = default;
		//Returns the stored time in seconds
		[[nodiscard]] float as_seconds() const;
		//Returns the stored time in milliseconds
		[[nodiscard]] size_t as_milliseconds() const;
		//Returns the stored time in microseconds
		[[nodiscard]] size_t as_microseconds() const;

		Time& operator -=(Time right);
		friend Time operator -(Time left, Time right);
	};

	Time operator -(Time left, Time right);
}

#endif
