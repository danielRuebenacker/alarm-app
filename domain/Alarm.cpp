#include "Alarm.h"
#include <climits>

Alarm::Alarm(TimePoint time, PuzzleType puzzleType, uint8_t dayMask)
    : time_(time), isActive_(false), hasTriggered_(false),
      snoozeMinutes_(Alarm::DEFAULT_SNOOZE_TIME),
      maxSnoozes_(Alarm::DEFAULT_SNOOZES), currentNoSnoozes(0), puzzleType_(puzzleType),
      dayMask_(dayMask) {
		  id = nextId++;
	  }

int Alarm::getId() const { return id; }

TimePoint Alarm::getTime() const {
	return time_;
}

bool Alarm::isActive() const {
	return isActive_;
}

int Alarm::getMinutesUntilRing(const TimePoint& currentTime) const {
	if (!isActive_) return INT_MAX;

	int currentMins = currentTime.minutesSinceMidnight();
	// for now ignore daymask, just return until next day
	int totalMins = TimePoint::DAY_MINUTES;
	return (currentMins - time_.minutesSinceMidnight() + totalMins) % totalMins;
}

bool Alarm::snoozePossible() const {
	return (currentNoSnoozes <= maxSnoozes_);
};

void Alarm::turnOff() {
	isActive_ = false;
	hasTriggered_ = true;
}

void Alarm::turnOn() {
	isActive_ = true;
	hasTriggered_ = false;
}

bool Alarm::shouldTrigger(const TimePoint& currentTime, int currentSnoozes) const {
	// alarm must be active, not have rung or snoozable, and past the current time point
	return isActive_ &&
		   (!hasTriggered_ || snoozePossible(currentSnoozes)) &&
		   (currentTime.minutesSinceMidnight() >= time_.minutesSinceMidnight());
}

PuzzleType Alarm::getPuzzleType() const {
	return puzzleType_;
}

