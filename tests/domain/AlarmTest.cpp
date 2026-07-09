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

	SUBCASE("Test from midnight to 8:30") {
		constexpr int expectedMinutes = 8*60 + 30;
		TimePoint t(0, 0);
		// make sure to active the alarm!!
		alarm.turnOn();
		// again random day (since day mask is 0 shouldn't matter)
		CHECK(alarm.getMinutesUntilRing(t, Days::Sunday) == expectedMinutes);
	}

	SUBCASE("Test from 8:31 to 8:30 should be DAY_MINUTES - 1") {
		constexpr int expectedMinutes = TimePoint::DAY_MINUTES - 1;
		TimePoint t(8, 31);
		// make sure to active the alarm!!
		alarm.turnOn();
		// again random day (since day mask is 0 shouldn't matter)
		CHECK(alarm.getMinutesUntilRing(t, Days::Sunday) == expectedMinutes);
	}
}

TEST_CASE("Test getMinutesUntilRing method with proper Days schedule") {
	Alarm alarm = createMockAlarm(12, 30);
	// set some days and add to alarm
	Days days =  { Days::Monday, Days::Friday };
	alarm.setDays(days);
	// activate!!
	alarm.turnOn();

	SUBCASE("Test inactive alarm with days mask") {
		// random timepoint/day
		TimePoint t(0, 0);
		// deactivate alarm for this test
		alarm.turnOff();
		CHECK(alarm.getMinutesUntilRing(t, Days::Sunday) == INT_MAX);
	}

	SUBCASE("Test getMinutesUntilRing when today is Wednesday 0:00") {
		// so Wednesday to Friday + 12:30
		constexpr int expectedMinutes = 12 * 60 + 30 + 2 * 24 * 60;
		TimePoint t(0, 0);
		CHECK(alarm.getMinutesUntilRing(t, Days::Wednesday) == expectedMinutes);
	}
}

TEST_CASE("Test should ring method") {
    Alarm alarm = createMockAlarm(12, 30);
    TimePoint t(12, 31);

    SUBCASE("Test should ring when alarm is turned off") {
        CHECK(alarm.shouldTrigger(t) == false);
    }

    SUBCASE("Test should ring when alarm is turned on and the time is passed") {
        // note the alarm has not triggered here yet
        alarm.turnOn();
        CHECK(alarm.shouldTrigger(t) == true);
    }
}
