#include "TimePoint.h"
#include <stdexcept>

TimePoint::TimePoint(int hour, int min, int dayOfTheWeek) {
	if (!(validateHour(hour) && validateMin(min) && validateDay(dayOfTheWeek))) {
		throw std::invalid_argument("invalid time");
	}

	hour_ = hour;
	min_ = min;
	dayOfTheWeek_ = dayOfTheWeek; 
}

int TimePoint::minutesSinceMidnight() const {
	return hour_ * 60 + min_;
};

int TimePoint::hour() const { return hour_; }
int TimePoint::min() const { return min_; }
int TimePoint::day() const { return dayOfTheWeek_; }

bool operator<(const TimePoint& t1, const TimePoint& t2) {
	if (t1.hour() != t2.hour()) {
		return t1.hour() < t2.hour();
	}
	return t1.min() < t2.min();
}

bool TimePoint::validateHour(int hour) {
	return 0 < hour && hour < 23;
}

bool TimePoint::validateMin(int min) {
	return 0 < min && min < 60;
}

bool TimePoint::validateDay(int dayOfTheWeek) {
	return 0 < dayOfTheWeek && dayOfTheWeek < 7;
}
