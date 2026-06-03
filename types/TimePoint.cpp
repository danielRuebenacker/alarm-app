#include "TimePoint.h"

TimePoint::TimePoint(int hour, int min) {
	if (hour < 0 || hour > 23 || min < 0 || min > 59) {
		throw std::invalid_argument("invalid time");
	}

	hour_ = hour;
	min_ = min;
}

int TimePoint::minutesSinceMidnight() const {
	return hour_ * 60 + min_;
};

int TimePoint::hour() const { return hour_; }
int TimePoint::min() const { return min_; }

bool operator<(const TimePoint& t1, const TimePoint& t2) {
	if (t1.hour() != t2.hour()) {
		return t1.hour() < t2.hour();
	}
	return t1.min() < t2.min();
}
