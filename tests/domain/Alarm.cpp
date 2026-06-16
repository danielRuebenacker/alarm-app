#include "../doctest.h"
#include <climits>

#include "src/domain/Alarm.h"

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

	SUBCASE("Check the alarm is inactive") {
		CHECK(alarm.isActive() == false);
	}
}

TEST_CASE("Test two different alarms produce two different IDs") {

	Alarm alarm1 = createMockAlarm(12, 30);
	Alarm alarm2 = createMockAlarm(8, 30);

	REQUIRE(alarm1.getId() != alarm2.getId());
}

TEST_CASE("Test getMinutesUntilRing method with basic alarm with empty days schedule") {
	Alarm alarm = createMockAlarm(8, 30);
	
	SUBCASE("Test inactive alarm getMinutesUntilRing returns INT_MAX") {
		// random timepoint/day
		TimePoint t(12, 30);
		CHECK(alarm.getMinutesUntilRing(t, Days::Friday) == INT_MAX);
	}

	SUBCASE("Test from midnight to 8:30 is 8 * 60 + 30 = 510 minutes") {
		TimePoint t(0, 0);
		// make sure to active the alarm!!
		alarm.turnOn();
		// again random day (since day mask is 0 shouldn't matter)
		CHECK(alarm.getMinutesUntilRing(t, Days::Sunday) == 510);
	}

	SUBCASE("Test from 8:31 to 8:30 should be DAY_MINUTES - 1") {
		TimePoint t(8, 31);
		// make sure to active the alarm!!
		alarm.turnOn();
		// again random day (since day mask is 0 shouldn't matter)
		CHECK(alarm.getMinutesUntilRing(t, Days::Sunday) == TimePoint::DAY_MINUTES - 1);
	}
}
