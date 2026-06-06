#pragma once
#include <vector>
#include "./Alarm.h"
#include "../interfaces/IStorage.h"

// stores alarms, gets/sets, returns next active alarm, can cancel alarm
class AlarmManager {
	private:
		// all: sorted by how many minutes until ring
		std::vector<Alarm> alarms;
		void makeActiveAlarms();
		void removeAlarm(std::vector<Alarm>& alarmList, int alarmId);
	public:
		std::vector<Alarm> getAlarms();
		Alarm* getAlarm(int alarmId);
		std::vector<Alarm> getActiveAlarms();
		void setAlarm(const Alarm& alarm, const TimePoint& now);
		void getAlarmsFromStorage(const IStorage& storage, TimePoint now);
		Alarm* getNextActiveAlarm(const TimePoint& now);
		void cancelAlarm(int alarmId);
};
