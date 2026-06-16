#include "../doctest.h"

#include "types/Days.h"

TEST_CASE("Simple check setting days works") {
	Days days;
	days.set(Days::Friday);
	CHECK(days.isActive(Days::Friday) == 1);
}

