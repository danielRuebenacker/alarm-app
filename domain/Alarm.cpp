#include "Alarm.h"

Alarm::Alarm(TimePoint t, PuzzleType puzzle, uint8_t days)
	: time(t), isActive(false), hasTriggered(false), snoozeMinutes(5), maxSnoozes(3), puzzleType(puzzle), dayMask(days) {}

TimePoint Alarm::getTime() {
	return time;
}

int Alarm::getMinutesUntilRing() {
	return time.min;
}
