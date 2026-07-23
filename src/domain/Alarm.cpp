#include "Alarm.h"
#include <climits>

Alarm::Alarm(TimePoint time, PuzzleType puzzleType, Days days)
    : time_(time), isActive_(false),
      snoozeMinutes_(Alarm::DEFAULT_SNOOZE_TIME),
      currentNoSnoozes(0), maxSnoozes_(Alarm::DEFAULT_SNOOZES), puzzleType_(puzzleType),
      days_(days) {
		  id = nextId++;
	  }
// -------------- getters -------------------
int Alarm::getId() const { return id; }

TimePoint Alarm::getTime() const {
	return time_;
}

bool Alarm::isActive() const {
	return isActive_;
}

int Alarm::getCurrentNoSnoozes() const {
	return currentNoSnoozes;
}

Days Alarm::getDays() const {
	return days_;
}

PuzzleType Alarm::getPuzzleType() const {
	return puzzleType_;
}

int Alarm::getSnoozeMinutes() const {
	return snoozeMinutes_;
}

int Alarm::getMaxSnoozes() const {
	return maxSnoozes_;
}
// -----------------------------------------------

// setters
void Alarm::setDays(const Days& days) {
	this->days_ = days;
}

void Alarm::setNextId(int id) {
    Alarm::nextId = id;
}

void Alarm::setTime(int hour, int min) {
    time_.setHour(hour);
    time_.setMin(min);
}

void Alarm::setSnoozeMinutes(int minutes) {
    snoozeMinutes_ = minutes;
}

void Alarm::setMaxSnoozes(int maxSnoozes) {
    maxSnoozes_ = maxSnoozes;
}

void Alarm::setPuzzleType(PuzzleType type) {
    puzzleType_ = type;
}

void Alarm::setDay(Days::Day day) {
	days_.set(day);
}

int Alarm::getMinutesUntilRing(const TimePoint& now, const Days::Day currentDay) const {
	if (!isActive_) return INT_MAX;

	int currentMins = now.minutesSinceMidnight();
	int alarmMins = time_.minutesSinceMidnight();
	int dayMins = TimePoint::DAY_MINUTES;

	// if the mask is empty, default's to next day
	if (!days_.hasAnyDays()) {
		return (alarmMins - currentMins + dayMins) % dayMins;
	}

	int daysUntil = days_.daysUntilNextActive(currentDay);

	// if alarm was today but has already passed
	if (daysUntil == 0 && currentMins >= alarmMins) {
		// move along
		int tomorrow = (currentDay + 1) % Days::Count;
		// add 1 to compensate for starting from tomorrow
		daysUntil = days_.daysUntilNextActive(tomorrow) + 1;
	}

	return (daysUntil * dayMins) + ((alarmMins - currentMins + dayMins) % dayMins);
}

std::chrono::milliseconds Alarm::getDurationUntilRing(const TimePoint& now, const Days::Day currentDay) const {
	int minutes = getMinutesUntilRing(now, currentDay);

	if (minutes == INT_MAX) {
		return std::chrono::milliseconds::max();
	}
	
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::minutes(minutes));
}

bool Alarm::snoozePossible() const {
	return (currentNoSnoozes <= maxSnoozes_);
};

void Alarm::turnOff() {
    isActive_ = false;
}

void Alarm::turnOn() {
	isActive_ = true;
}

void Alarm::toggle() {
    if (isActive_) {
        turnOff();
    } else {
        turnOn();
    }
}

bool Alarm::shouldTrigger(const TimePoint& currentTime, Days::Day currentDay) const {
    if (!isActive_) return false;

    // otherwise time must have passed and active today or no days
    return currentTime.minutesSinceMidnight() >= time_.minutesSinceMidnight() &&
        (!days_.hasAnyDays() || days_.isActive(currentDay));
}

bool Alarm::snooze() {
	if (snoozePossible()) {
		this->currentNoSnoozes += 1;
		return true;
	}
	return false;
}
