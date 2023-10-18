#include "Literal/Time.h"


float Lit::Time::as_seconds() const
{
	using namespace std::chrono;

	const auto time_point_secs{time_point_cast<milliseconds>(_stored_time)};
	const auto epoch = time_point_secs.time_since_epoch();
	const auto value = duration_cast<milliseconds>(epoch);
	return static_cast<float>(value.count()) / 1000.f;
}

size_t Lit::Time::as_milliseconds() const
{
	using namespace std;

	const auto time_point_secs{chrono::time_point_cast<chrono::milliseconds>(_stored_time)};
	const auto epoch = time_point_secs.time_since_epoch();
	const auto value = chrono::duration_cast<chrono::milliseconds>(epoch);
	return value.count();
}

size_t Lit::Time::as_microseconds() const
{
	using namespace std;

	const auto time_point_secs{chrono::time_point_cast<chrono::microseconds>(_stored_time)};
	const auto epoch = time_point_secs.time_since_epoch();
	const auto value = chrono::duration_cast<chrono::microseconds>(epoch);
	return value.count();
}

Lit::Time& Lit::Time::operator-=(const Time right)
{
	using namespace std;

	const auto new_time_duration = _stored_time - right._stored_time;
	const auto new_time_period = chrono::time_point<chrono::steady_clock>(new_time_duration);

	Time new_time{};
	_stored_time = new_time_period;
	return *this;
}

Lit::Time Lit::operator-(const Time left, const Time right)
{
	using namespace std;

	const auto new_time_duration = left._stored_time - right._stored_time;
	const auto new_time_period = chrono::time_point<chrono::steady_clock>(new_time_duration);

	Time new_time{};
	new_time._stored_time = new_time_period;
	return new_time;
}
