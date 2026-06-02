#include "Alarm.h"

Alarm::Alarm(TimePoint time, PuzzleType puzzleType, uint8_t dayMask)
    : time_(time), isActive_(false), hasTriggered_(false),
      snoozeMinutes_(Alarm::DEFAULT_SNOOZES),
      maxSnoozes_(Alarm::DEFAULT_SNOOZE_TIME), puzzleType_(puzzleType),
      dayMask_(dayMask) {}

TimePoint Alarm::getTime() {
	return time_;
}

int Alarm::getMinutesUntilRing(const TimePoint currentTime) {
	int currentMins = currentTime.minutesSinceMidnight();
	// for now ignore daymask, just return until next day
	int totalMins = TimePoint::DAY_MINUTES;
	return (currentMins - time_.minutesSinceMidnight() + totalMins) % totalMins;
}

bool Alarm::snoozePossible(int currentSnoozes) {
	return (currentSnoozes <= maxSnoozes_);
};

void Alarm::turnOff() {
	isActive_ = false;
	hasTriggered_ = true;
}

void Alarm::turnOn() {
	isActive_ = true;
	hasTriggered_ = false;
}

bool Alarm::shouldTrigger(const TimePoint currentTime, int currentSnoozes) {
	// alarm must be active, not have rung or snoozable, and past the current time point
	return isActive_ &&
		   (!hasTriggered_ || snoozePossible(currentSnoozes)) &&
		   (currentTime.minutesSinceMidnight() >= time_.minutesSinceMidnight());
}
