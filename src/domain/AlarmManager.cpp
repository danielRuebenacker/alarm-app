#include "AlarmManager.h"
#include <algorithm>
#include <limits>

AlarmManager::AlarmManager(const IClock& clock, IStorage& storage)
	: storage_(storage), clock_(clock) {}

std::vector<Alarm> AlarmManager::getAlarms() {
	return alarms;
}

void AlarmManager::addAlarm(const Alarm& alarm) {
    // if alarm has the same id, this is treated as an overwrite!
    auto it = std::find_if(alarms.begin(), alarms.end(), [&](const Alarm &a) {
    return a.getId() == alarm.getId();
    });
    if (it != alarms.end()) *it = alarm;
    else alarms.push_back(alarm);

    // save!
    storage_.saveAlarms(alarms);
}

void AlarmManager::getAlarmsFromStorage() {
	std::vector<Alarm> loadedAlarms = storage_.loadAlarms();
	// clear and insert manuallly
	alarms.clear();

    // need to also calculate next highest id (counter lost)
    int maxId = 0;
	for (const Alarm& alarm : loadedAlarms) {
        alarms.push_back(alarm);
        if (alarm.getId() > maxId) maxId = alarm.getId();
	}
    Alarm::setNextId(maxId + 1);
}

void AlarmManager::getDismissedAlarmIdsFromStorage() {
	std::vector<int> loadedDismissed = storage_.loadDismissedAlarmIds();
	// replace
	dismissedAlarmIds = loadedDismissed;
}

const Alarm* AlarmManager::getNextActiveAlarm() {
    TimePoint now = clock_.now();
    Days::Day currentDay = clock_.getCurrentDay();
    int shortestTimeDiff = std::numeric_limits<int>::max();
    const Alarm* nextAlarm = nullptr;

    for (const Alarm &alarm : alarms) {
	  // if an alarm is closer, put it as the shortest
	  // have to factor in dismissed alarms!! alarms are only dismissed for one day!!
	  int minsUntilRingNoDismiss = alarm.getMinutesUntilRing(now, currentDay);
	  int actualTimeUntilRing = wasAlarmDismissed(alarm.getId())
		? minsUntilRingNoDismiss + TimePoint::DAY_MINUTES
		: minsUntilRingNoDismiss;
			
        if (alarm.isActive() &&
            actualTimeUntilRing < shortestTimeDiff) {
            shortestTimeDiff = actualTimeUntilRing;
            nextAlarm = &alarm;
        }
    }
    return nextAlarm;
}

void AlarmManager::dismissAlarm(int alarmId) {
	Alarm* alarm = getAlarmById(alarmId);
	if (!alarm) return;

    dismissedAlarmIds.push_back(alarmId);
}

void AlarmManager::deleteAlarm(int alarmId) {
	auto it = alarms.begin();
	while (it != alarms.end()) {
		if (it->getId() == alarmId) {
			it = alarms.erase(it);
			// since id unique
			break;
		} else {
			++it;
		}
	}
}

Alarm* AlarmManager::getAlarmById(int alarmId) {
	for (auto& alarm : alarms) {
		if (alarm.getId() == alarmId) return &alarm;
	}
	return nullptr;
}

bool AlarmManager::snoozeAlarm(Alarm& alarm) {
	return alarm.snooze();
}

bool AlarmManager::wasAlarmDismissed(int alarmId) {
    for (int id : dismissedAlarmIds) {
        if (alarmId == id) return true;
    }
    return false;
}

Alarm* AlarmManager::getMostRecentlyMissedAlarm(int daysFrom1970ToSleepDay, const Days::Day& sleepDay) {
  // NOTE: this method is to be run at startup, when the dismissedAlarmIds
  // vector is stale from the day when the system fell alseep
  // follow a call to this method with refreshing (optionally clearning) the dismissed vector
    Alarm* mostRecentlyMissed = nullptr;
    int narrowestMissedMargin = std::numeric_limits<int>::max();
    // we assume we went to sleep in the past xD
    int dayDifference = clock_.getDaysSince1970() - daysFrom1970ToSleepDay ;
    TimePoint wakeTime = clock_.now();
    Days::Day currentDay = clock_.getCurrentDay();

    // we ignore alarms if they are older than one day (could display alarm missed message)
    if (dayDifference > 1) {
        return nullptr;
    }

    // if it is a different day we discard the
    // loop through alarms
    for (Alarm &alarm : alarms) {
        // ignore inactive alarms
        if (!alarm.isActive()) continue;

        int missMargin = -1;
        int alarmMins = alarm.getTime().minutesSinceMidnight();

        if (dayDifference == 0) {
            // same day (dismissedAlarmIds matters!)
            // if not handled
            if (!wasAlarmDismissed(alarm.getId()) &&
                alarm.shouldTrigger(wakeTime, currentDay)) {
                missMargin = wakeTime.minutesSinceMidnight() - alarmMins;
            }
        }
        else if (dayDifference == 1) {
            // crossed one midnight boundary
            // did we miss an alarm yesterday?
            bool activeYesterday = !alarm.getDays().hasAnyDays() || alarm.getDays().isActive(sleepDay);
            // NOTE: dismissedAlarmIds is then stale from yesterday!!!!
            if (activeYesterday && !wasAlarmDismissed(alarm.getId())) {
                missMargin = wakeTime.minutesSinceMidnight() + (TimePoint::DAY_MINUTES - alarmMins);
            }

            // did we miss an alarm this morning but fell alseep last night?
            if (alarm.shouldTrigger(wakeTime, currentDay)) {
                missMargin = wakeTime.minutesSinceMidnight() - alarmMins;
            }
        }

        if (missMargin >= 0 && missMargin < narrowestMissedMargin) {
            narrowestMissedMargin = missMargin;
            mostRecentlyMissed = &alarm;
        }
    }

	// make the choice here as its already calculated: > 15 minutes and we ignore!
	if (narrowestMissedMargin > 15) return nullptr;
    return mostRecentlyMissed;
}
