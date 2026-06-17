#include "../doctest.h"

#include "src/types/Days.h"

TEST_CASE("Test days bitmask (basic)") {
	Days days;

	SUBCASE("Check isActive and hasAnyDays return false when no days set") {
		CHECK(days.isActive(Days::Friday) == false);
		CHECK(days.hasAnyDays() == false);
	}

	SUBCASE("Check adding single day, isActive and hasAnyDays should return true") {
		days.set(Days::Friday);
		CHECK(days.hasAnyDays() == true);
		CHECK(days.isActive(Days::Friday) == true);
	}
}

TEST_CASE("Test daysUntilNextActive function") {
	Days days;

	SUBCASE("Test empty mask returns -1") {
		CHECK(days.daysUntilNextActive(Days::Monday) == -1);
	}

	SUBCASE("Check how many days to next active day (today Sunday, Monday active)") {
		days.set(Days::Monday);
		CHECK(days.daysUntilNextActive(Days::Sunday) == 1);
	}

	SUBCASE("Check days from Wednesday to Friday is 2") {
		days.set(Days::Friday);
		CHECK(days.daysUntilNextActive(Days::Wednesday) == 2);
	}
}

TEST_CASE("Test clearAll") {
	Days days;

	SUBCASE("Test clear all on no days (should do nothing)") {
		days.clearAll();
		CHECK(days.hasAnyDays() == false);
	}

	SUBCASE("Test clearAll on single day clears that day") {
		days.set(Days::Sunday);

		days.clearAll();
		CHECK(days.hasAnyDays() == false);
	}

	SUBCASE("Test clearAll on a set of days clears") {
		days.set(Days::Sunday);
		days.set(Days::Monday);
		days.set(Days::Tuesday);
		days.set(Days::Wednesday);

		days.clearAll();
		CHECK(days.hasAnyDays() == false);
	}
}
