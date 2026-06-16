#include "Days.h"

int Days::daysUntilNextActive(int currentDay) const {
	for (int offset = 0; offset < Count; offset++) {
		int dayToCheck = (currentDay + offset) % Count;

		if (mask_.test(dayToCheck)) return offset;
	}

	// no days
	return -1;
}
