#include "Literal/Time.h"

float Lit::Time::Seconds() const
{
	using namespace std::chrono;

	const auto time_point_secs{ time_point_cast<milliseconds>(_storedTime) };
	const auto epoch = time_point_secs.time_since_epoch();
	const auto value = duration_cast<milliseconds>(epoch);
	return static_cast<float>(value.count()) / 1000.f;
}

size_t Lit::Time::Milliseconds() const
{
	using namespace std;

	const auto time_point_secs{ chrono::time_point_cast<chrono::milliseconds>(_storedTime) };
	const auto epoch = time_point_secs.time_since_epoch();
	const auto value = chrono::duration_cast<chrono::milliseconds>(epoch);
	return value.count();
}

size_t Lit::Time::Microseconds() const
{
	using namespace std;

	const auto time_point_secs{ chrono::time_point_cast<chrono::microseconds>(_storedTime) };
	const auto epoch = time_point_secs.time_since_epoch();
	const auto value = chrono::duration_cast<chrono::microseconds>(epoch);
	return value.count();
}

Lit::Time& Lit::Time::operator-=(const Time right)
{
	using namespace std;

	const auto new_time_duration = _storedTime - right._storedTime;
	const auto new_time_period = chrono::time_point<chrono::steady_clock>(new_time_duration);

	Time new_time{};
	_storedTime = new_time_period;
	return *this;
}

Lit::Time Lit::operator-(const Time left, const Time right)
{
	using namespace std;

	const auto new_time_duration = left._storedTime - right._storedTime;
	const auto new_time_period = chrono::time_point<chrono::steady_clock>(new_time_duration);

	Time new_time{};
	new_time._storedTime = new_time_period;
	return new_time;
}