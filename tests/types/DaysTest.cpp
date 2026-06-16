#include "../doctest.h"

#include "types/Days.h"

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
}
