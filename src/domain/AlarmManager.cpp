#include "AlarmManager.h"
#include <algorithm>

AlarmManager::AlarmManager(const IClock& clock, const IStorage& storage)
	: storage_(storage), clock_(clock) {}

std::vector<Alarm> AlarmManager::AlarmManager::getAlarms() {
	return alarms;
}

void AlarmManager::setAlarm(const Alarm& alarm) {
	// insert in sorted order according to how many minutes until alarm rings
	int minsUntilRing = alarm.getMinutesUntilRing(clock_.now(), clock_.getCurrentDay());

	// need to find lowest index, s.t. alarm can be inserted i.e. minsToRing < next.getMinutesUntilRing
	auto it = std::upper_bound(
		alarms.begin(), alarms.end(), minsUntilRing,
		[this](int targetMinutes, const Alarm &existingAlarm) {
		  return targetMinutes < existingAlarm.getMinutesUntilRing(clock_.now(), clock_.getCurrentDay());
		});
	alarms.insert(it, alarm);
}

void AlarmManager::getAlarmsFromStorage() {
	std::vector<Alarm> loadedAlarms = storage_.loadAlarms();
	// clear and insert manuallly
	alarms.clear();

    // need to also calculate next highest id (counter lost)
    int maxId = 0;
	for (const Alarm& alarm : loadedAlarms) {
		setAlarm(alarm);
        if (alarm.getId() > maxId) maxId = alarm.getId();
	}
    Alarm::setNextId(maxId + 1);

}

Alarm* AlarmManager::getNextActiveAlarm() {
    TimePoint now = clock_.now();
	return nullptr;
}

void AlarmManager::dismissAlarm(int alarmId) {
	Alarm* alarm = getAlarmById(alarmId);
	if (!alarm) return;
	alarm->turnOff();
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
