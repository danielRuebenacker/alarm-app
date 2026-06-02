#pragma once
#include <stdexcept>

class TimePoint {
	public:
		TimePoint(int hour, int min) {
			if (hour < 0 || hour > 23 ||
            min < 0 || min > 59)
            throw std::invalid_argument("invalid time");

			hour_ = hour;
			min_ = min;
		}

		int minutesSinceMidnight() {
			return hour_ * 60 + min_;
		};

		int hour() const { return hour_; }
		int min() const { return min_; }

	private:
		int hour_;
		int min_;
};
