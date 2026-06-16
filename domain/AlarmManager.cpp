#include "AlarmManager.h"
#include <algorithm>

std::vector<Alarm> AlarmManager::AlarmManager::getAlarms() {
	return alarms;
}

void AlarmManager::setAlarm(const Alarm& alarm, const IClock& clock) {
	// insert in sorted order according to how many minutes until alarm rings
	int minsUntilRing = alarm.getMinutesUntilRing(clock);

	// need to find lowest index, s.t. alarm can be inserted i.e. minsToRing < next.getMinutesUntilRing
	auto it = std::upper_bound(
		alarms.begin(), alarms.end(), minsUntilRing,
		[&clock](int targetMinutes, const Alarm &existingAlarm) {
		  return targetMinutes < existingAlarm.getMinutesUntilRing(clock);
		});
	alarms.insert(it, alarm);
}

void AlarmManager::getAlarmsFromStorage(const IStorage& storage, const IClock& clock) {
	std::vector<Alarm> loadedAlarms = storage.loadAlarms();
	// clear and insert manuallly
	alarms.clear();

	for (const auto& alarm : loadedAlarms) {
		setAlarm(alarm, clock);
	}

}

Alarm* AlarmManager::getNextActiveAlarm(const TimePoint& now) {
	// first one in list
	for (auto& alarm : alarms) {
		if (alarm.isActive()) {
			return &alarm;
		}
	}
	return nullptr;
}

void AlarmManager::removeAlarm(std::vector<Alarm>& alarmList, int alarmId) {
	auto it = alarmList.begin();
	while (it != alarmList.end()) {
		if (it->getId() == alarmId) {
			it = alarmList.erase(it);
			// since id unique
			break;
		} else {
			++it;
		}
	}
}

void AlarmManager::cancelAlarm(int alarmId) {
	removeAlarm(alarms, alarmId);
}

Alarm* AlarmManager::getAlarm(int alarmId) {
	for (auto& alarm : alarms) {
		if (alarm.getId() == alarmId) return &alarm;
	}
	return nullptr;
}

bool AlarmManager::snoozeAlarm(Alarm& alarm) {
	return alarm.snooze();
}
