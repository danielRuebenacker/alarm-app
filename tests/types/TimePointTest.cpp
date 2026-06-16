#include "../doctest.h"
#include <stdexcept>

#include "src/types/TimePoint.h"

TEST_CASE("Test basic constructor and hour, min, day methods") {
	SUBCASE("Test timepoint for: Sunday 0:30") { 
		TimePoint tp(0, 30);

		CHECK(tp.hour() == 0);
		CHECK(tp.min() == 30);
	}

	SUBCASE("Test timepoint for: Monday, 9:55") { 
		TimePoint tp(9, 55);

		CHECK(tp.hour() == 9);
		CHECK(tp.min() == 55);
	}
}
TEST_CASE("Test constructor argument validation") {
	SUBCASE("Check invalid hour throws invalid_argument exception") {
		CHECK_THROWS_AS(TimePoint tp(24, 55), std::invalid_argument);
	}

	SUBCASE("Check invalid min throws invalid_argument exception") {
		CHECK_THROWS_AS(TimePoint tp(9, 60), std::invalid_argument);
	}

	SUBCASE("Check invalid hour and min throw invalid_argument exception") {
		CHECK_THROWS_AS(TimePoint tp(24, 60), std::invalid_argument);
	}

	SUBCASE("Check invalid_argument exception thrown on negative numbers") {
		CHECK_THROWS_AS(TimePoint tp(-1, 55), std::invalid_argument);
	}
}

TEST_CASE("Test minutesSinceMidnight method") {
	SUBCASE("Check minutes since midnight for midnight time is 0") {
		TimePoint tp(0, 0);
		CHECK(tp.minutesSinceMidnight() == 0);
	}

	SUBCASE("Check minutes since midnight for midday time is 12 * 60 = 720") {
		TimePoint tp(12, 0);
		CHECK(tp.minutesSinceMidnight() == 720);
	}

	SUBCASE("Check minutesSinceMidnight one min before midnigh") {
		TimePoint tp(23, 59);
		CHECK(tp.minutesSinceMidnight() == TimePoint::DAY_MINUTES - 1);
	}

	SUBCASE("Check minutesSinceMidnight for 8:30 equals 510") {
		TimePoint tp(8, 30);
		CHECK(tp.minutesSinceMidnight() == 510);
	}
}
