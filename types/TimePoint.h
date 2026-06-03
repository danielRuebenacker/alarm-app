#pragma once
#include <stdexcept>

class TimePoint {
	public:
		TimePoint(int hour, int min);
		int minutesSinceMidnight() const;
		int hour() const;
		int min() const;
		static constexpr int DAY_MINUTES = 1440;

	private:
		int hour_;
		int min_;
};
bool operator<(const TimePoint& t1, const TimePoint& t2);
