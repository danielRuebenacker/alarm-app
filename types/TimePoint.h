#pragma once
#include "types/Days.h"

class TimePoint {
	public:
		TimePoint(int hour, int min, Days::Day dayOfTheWeek);
		int minutesSinceMidnight() const;
		int hour() const;
		int min() const;
		int day() const;
		static constexpr int DAY_MINUTES = 1440;

	private:
		int hour_;
		int min_;
		Days::Day dayOfTheWeek_;

		bool validateHour(int hour);
		bool validateMin(int min);
};
bool operator<(const TimePoint& t1, const TimePoint& t2);
