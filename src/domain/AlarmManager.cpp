#include "AlarmManager.h"
#include <algorithm>
#include <limits>

AlarmManager::AlarmManager(const IClock& clock, IStorage& storage)
	: storage_(storage), clock_(clock) {}

std::vector<Alarm> AlarmManager::getAlarms() {
	return alarms;
}

void AlarmManager::setAlarm(const Alarm& alarm) {
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
