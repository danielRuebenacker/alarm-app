#pragma once
#include <ctime>
#include <chrono>

#include "../interfaces/IClock.h"

// Wall-clock time supplied by the host platform.
//
// LVGL only exposes a monotonic millisecond tick (lv_tick_get), which cannot
// tell us the time of day or the date, so wall-clock time has to come from the
// platform. Both desktop Linux (std::chrono / system time) and M5Stack (ESP32
// system clock, synced from the hardware RTC / NTP) expose the same POSIX time
// API, so a single implementation covers both targets. The only M5Stack-specific
// setup required is setting the system time at boot (e.g. configTzTime()).
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

		// -- pure conversions, exposed for testing --
		static TimePoint timePointFromTm(const std::tm& tm) {
			return TimePoint(tm.tm_hour, tm.tm_min);
		}

		// std::tm::tm_wday is 0 (Sunday) .. 6 (Saturday), matching Days::Day.
		static Days::Day dayFromTm(const std::tm& tm) {
			return static_cast<Days::Day>(tm.tm_wday);
		}

		static int daysSince1970FromTm(const std::tm& tm) {
			std::tm midnight = tm;
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
