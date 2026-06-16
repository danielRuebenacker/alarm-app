#pragma once
#include <vector>
#include "./Alarm.h"
#include "../interfaces/IStorage.h"
#include "../interfaces/IClock.h"

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
		void setAlarm(const Alarm& alarm, const IClock& clock);
		void getAlarmsFromStorage(const IStorage& storage, const IClock& clock);
		Alarm* getNextActiveAlarm(const TimePoint& now);
		void cancelAlarm(int alarmId);
		bool snoozeAlarm(Alarm& alarm);
};
