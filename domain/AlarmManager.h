#pragma once
#include <vector>
#include "./Alarm.h"
#include "../interfaces/IStorage.h"

// stores alarms, gets/sets, returns next active alarm, can cancel alarm
class AlarmManager {
	private:
		// all
		std::vector<Alarm> alarms;
		// sorted, insertion needs to maintain sort order
		std::vector<Alarm> activeAlarms;
		void makeActiveAlarms();
		void removeAlarm(std::vector<Alarm>& alarmList, int alarmID);
	public:
		std::vector<Alarm> getAlarms();
		std::vector<Alarm> getActiveAlarms();
		void setAlarm(Alarm alarm);
		void getAlarmsFromStorage(IStorage& storage);
		Alarm* getNextActiveAlarm(TimePoint now);
		void cancelAlarm(int alarmID);
};
