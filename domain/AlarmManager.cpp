#include "AlarmManager.h"
#include <algorithm>

std::vector<Alarm> AlarmManager::AlarmManager::getAlarms() {
	return alarms;
}

std::vector<Alarm> AlarmManager::getActiveAlarms() {
	return activeAlarms;
}

void AlarmManager::setAlarm(const Alarm& alarm, const TimePoint& now) {
	// insert in sorted order according to how many minutes until alarm rings
	int minsUntilRing = alarm.getMinutesUntilRing(now);

	// need to find lowest index, s.t. alarm can be inserted i.e. minsToRing < next.getMinutesUntilRing
	auto it = std::upper_bound(
		alarms.begin(), alarms.end(), minsUntilRing,
		[&now](int targetMinutes, const Alarm &existingAlarm) {
		  return targetMinutes < existingAlarm.getMinutesUntilRing(now);
		});
	alarms.insert(it, alarm);
}

void AlarmManager::makeActiveAlarms() {
	for (const auto& alarm : alarms) {
		if (alarm.isActive()) {
			activeAlarms.push_back(alarm);
		}
	}
}

void AlarmManager::getAlarmsFromStorage(const IStorage& storage) {
	std::vector<Alarm> loadedAlarms = storage.loadAlarms();
	// replace
	alarms = loadedAlarms;
	// make active ones
	makeActiveAlarms();
}

Alarm* AlarmManager::getNextActiveAlarm(const TimePoint& now) {
	// first one in list
	for (auto& alarm : activeAlarms) {
		if (now < alarm.getTime()) {
			return &alarm;
		}
	}
	return nullptr;
}

void AlarmManager::removeAlarm(std::vector<Alarm>& alarmList, int alarmID) {
	auto it = alarmList.begin();
	while (it != alarmList.end()) {
		if (it->getId() == alarmID) {
			it = alarmList.erase(it);
			// since id unique
			break;
		} else {
			++it;
		}
	}
}

void AlarmManager::cancelAlarm(int alarmID) {
	// normal list
	removeAlarm(alarms, alarmID);
	removeAlarm(activeAlarms, alarmID);
}
