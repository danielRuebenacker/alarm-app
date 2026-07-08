#pragma once
#include <string>

class TimePoint {
	public:
		TimePoint(int hour, int min);
		int minutesSinceMidnight() const;
		int hour() const;
		int min() const;
		static constexpr int DAY_MINUTES = 1440;
        const std::string toString();

	private:
		int hour_;
		int min_;

		bool validateHour(int hour);
		bool validateMin(int min);
};
bool operator<(const TimePoint& t1, const TimePoint& t2);
