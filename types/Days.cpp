#include "Days.h"

int Days::daysUntilNextActive(int currentDay) const {
	// current day is just index e.g. 0 = Sunday, 1 = Monday, ...
	
	// loop through, checking each day starting from today
	for (int i = 0; i < 7; i++) {
		int nextDayIndex = (currentDay + i) % 7;

		Mask nextDayMask = static_cast<Mask>(1 << nextDayIndex);

		if (this->value & nextDayMask) {
			return i;
		}
	}
	// not found
	return -1;
}
