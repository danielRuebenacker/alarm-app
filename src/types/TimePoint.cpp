#include "TimePoint.h"
#include <stdexcept>
#include <string>
#include <format>

TimePoint::TimePoint(int hour, int min) {
	if (!validateHour(hour)) throw std::invalid_argument("invalid hour");
	if (!validateMin(min)) throw std::invalid_argument("invalid min");

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

bool TimePoint::validateHour(int hour) {
	return 0 <= hour && hour <= 23;
}

bool TimePoint::validateMin(int min) {
	return 0 <= min && min < 60;
}

const std::string TimePoint::toString() {
	return std::format("{:02d}:{:02d}", hour_, min_);
}


void TimePoint::setHour(int hour) {
    hour_ = hour;
}

void TimePoint::setMin(int min) {
    min_ = min;
}
