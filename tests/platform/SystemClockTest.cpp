#include "../doctest.h"

#include <ctime>

#include "src/platform/SystemClock.h"

TEST_CASE("SystemClock converts a tm to a TimePoint") {
	std::tm tm{};
	tm.tm_hour = 9;
	tm.tm_min = 30;

	TimePoint tp = SystemClock::timePointFromTm(tm);
	CHECK(tp.hour() == 9);
	CHECK(tp.min() == 30);
	CHECK(tp.toString() == "09:30");
}

TEST_CASE("SystemClock maps tm_wday onto Days") {
	SUBCASE("every weekday maps to the matching Days value") {
		for (size_t weekday = 0; weekday < Days::Count; ++weekday) {
			std::tm tm{};
			tm.tm_wday = static_cast<int>(weekday);
			CHECK(SystemClock::dayFromTm(tm) == static_cast<Days::Day>(weekday));
		}
	}

	SUBCASE("spot check Sunday and Saturday") {
		std::tm sunday{};
		sunday.tm_wday = 0;
		CHECK(SystemClock::dayFromTm(sunday) == Days::Sunday);

		std::tm saturday{};
		saturday.tm_wday = 6;
		CHECK(SystemClock::dayFromTm(saturday) == Days::Saturday);
	}
}

TEST_CASE("SystemClock counts days since 1970 in whole local days") {
	// 2024-01-01 23:59 and 2024-01-02 00:00 should differ by exactly one day,
	// regardless of the host timezone.
	std::tm lateNight{};
	lateNight.tm_year = 124;  // years since 1900 -> 2024
	lateNight.tm_mon = 0;     // January
	lateNight.tm_mday = 1;
	lateNight.tm_hour = 23;
	lateNight.tm_min = 59;

	std::tm nextMidnight = lateNight;
	nextMidnight.tm_mday = 2;
	nextMidnight.tm_hour = 0;
	nextMidnight.tm_min = 0;

	int before = SystemClock::daysSince1970FromTm(lateNight);
	int after = SystemClock::daysSince1970FromTm(nextMidnight);
	CHECK(after - before == 1);
}
