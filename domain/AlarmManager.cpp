#include "AlarmManager.h"

std::vector<Alarm> AlarmManager::AlarmManager::getAlarms() {
	return alarms;
}

std::vector<Alarm> AlarmManager::getActiveAlarms() {
	return activeAlarms;
}

void AlarmManager::setAlarm(const Alarm alarm) {
	// need to keep sort order
	auto it = alarms.begin();

	while (it != alarms.end()) {
		if (it->getTime() < alarm.getTime()) {
			break;
		}
		++it;
	}
	alarms.insert(it, alarm);
}

void AlarmManager::getAlarmsFromStorage(IStorage& storage) {
	std::vector<Alarm> loadedAlarms = storage.loadAlarms();
	// replace
	alarms = loadedAlarms;
}

Alarm* AlarmManager::getNextActiveAlarm(TimePoint now) {
	// prototype
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
