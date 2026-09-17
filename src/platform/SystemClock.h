#pragma once
#include <ctime>
#include <chrono>

#include "../interfaces/IClock.h"

// time on both desktop linux and esp32 exposed via same functions, so this is common

class SystemClock : public IClock {
	public:
		TimePoint now() const override {
			return timePointFromTm(localNow());
		}

		Days::Day getCurrentDay() const override {
			return dayFromTm(localNow());
		}

		int getDaysSince1970() const override {
			return daysSince1970FromTm(localNow());
		}

		// for testing
		static TimePoint timePointFromTm(const std::tm& tm) {
			return TimePoint(tm.tm_hour, tm.tm_min);
		}

		// std::tm::tm_wday is 0 (Sunday) .. 6 (Saturday), matching Days::Day.
		static Days::Day dayFromTm(const std::tm& tm) {
			return static_cast<Days::Day>(tm.tm_wday);
		}

		static int daysSince1970FromTm(const std::tm& tm) {
			std::tm midnight = tm;
			// set hours, mins, secs to 0
			midnight.tm_hour = 0;
			midnight.tm_min = 0;
			midnight.tm_sec = 0;
			std::time_t t = std::mktime(&midnight);
			return static_cast<int>(t / SECONDS_PER_DAY);
		}

	private:
		static constexpr int SECONDS_PER_DAY = 24 * 60 * 60;

		static std::tm localNow() {
			std::time_t t = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
			std::tm tm{};
			localtime_r(&t, &tm);
			return tm;
		}
};
