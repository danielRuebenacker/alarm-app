#include "Alarm.h"

Alarm::Alarm(TimePoint t, PuzzleType puzzle, uint8_t days)
	: time(t), isActive(false), hasTriggered(false), snoozeMinutes(5), maxSnoozes(3), puzzleType(puzzle), dayMask(days) {}

TimePoint Alarm::getTime() {
	return time;
}

int Alarm::getMinutesUntilRing(const TimePoint currentTime) {
	int currentMins = currentTime.minutesSinceMidnight();
	// for now ignore daymask, just return until next day
	int totalMins = TimePoint::DAY_MINUTES;
	return (currentMins - time.minutesSinceMidnight() + totalMins) % totalMins;
}
