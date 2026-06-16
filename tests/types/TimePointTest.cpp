#include "../doctest.h"
#include <stdexcept>

#include "types/TimePoint.h"
#include "types/Days.h"

TEST_CASE("Test basic constructor and hour, min, day methods") {
	SUBCASE("Test timepoint for: Sunday 14:30") { 
		TimePoint tp(14, 30, Days::Sunday);

		CHECK(tp.hour() == 14);
		CHECK(tp.min() == 30);
		CHECK(tp.day() == Days::Sunday);
	}

	SUBCASE("Test timepoint for: Monday, 9:55") { 
		TimePoint tp(9, 55, Days::Monday);

		CHECK(tp.hour() == 9);
		CHECK(tp.min() == 55);
		CHECK(tp.day() == Days::Monday);
	}
}
TEST_CASE("Test constructor argument validation") {
	SUBCASE("Check invalid hour throws invalid_argument exception") {
		CHECK_THROWS_AS(TimePoint tp(24, 55, Days::Sunday), std::invalid_argument);
	}

	SUBCASE("Check invalid min throws invalid_argument exception") {
		CHECK_THROWS_AS(TimePoint tp(9, 65, Days::Sunday), std::invalid_argument);
	}

	SUBCASE("Check invalid hour and min throw invalid_argument exception") {
		CHECK_THROWS_AS(TimePoint tp(24, 65, Days::Sunday), std::invalid_argument);
	}

	SUBCASE("Check invalid_argument exception thrown on negative numbers") {
		CHECK_THROWS_AS(TimePoint tp(-1, 55, Days::Sunday), std::invalid_argument);
	}
}

// TEST_CASE("Test minutesSinceMidnight method") {
// 	SUBCASE("Check minutes since midnight for midnight time is 0") {
//
// 	}
// }
