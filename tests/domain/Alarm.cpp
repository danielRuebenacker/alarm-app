#include "../doctest.h"
#include <climits>

#include "domain/Alarm.h"

Alarm createMockAlarm(int hour, int min, const Days& schedule = Days{}) {
	TimePoint t(hour, min);
	return Alarm(t, PuzzleType::MATHS, schedule);
}

TEST_CASE("Test alarm construction") {
	// 12:30, day irrelevant
	Alarm alarm = createMockAlarm(12, 30);

	SUBCASE("Check time of initialised alarm is correct") {
		TimePoint time = alarm.getTime();
		CHECK(time.hour() == 12);
		CHECK(time.min() == 30);
	}

	SUBCASE("Check puzzletype of initialised alarm is correct") {
		CHECK(alarm.getPuzzleType() == PuzzleType::MATHS);
	}
}

TEST_CASE("Test two different alarms produce two different IDs") {

	Alarm alarm1 = createMockAlarm(12, 30);
	Alarm alarm2 = createMockAlarm(8, 30);

	REQUIRE(alarm1.getId() != alarm2.getId());
}

TEST_CASE("Test getMinutesUntilRing method") {
	Alarm alarm = createMockAlarm(8, 30);
	
	SUBCASE("Test inactive alarm getMinutesUntilRing returns INT_MAX") {
		// random timepoint/day
		TimePoint t(12, 30);
		CHECK(alarm.getMinutesUntilRing(t, Days::Friday) == INT_MAX);
	}

	// SUBCASE("Test from midnight to 8:30 is 8 * 60 + 30 = 510 minutes") {
	//
	// }
}
