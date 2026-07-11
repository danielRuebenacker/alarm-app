#pragma once
#include <memory>
#include <vector>
#include "./Alarm.h"
#include "../interfaces/IStorage.h"
#include "../interfaces/IClock.h"

// stores alarms, gets/sets, returns next active alarm, can cancel alarm
class AlarmManager {
	private:
        const IClock&  clock_;
        const IStorage&  storage_;

		// all: sorted by how many minutes until ring
		std::vector<Alarm> alarms;
		void makeActiveAlarms();
	public:
		AlarmManager(const IClock& clock, const IStorage& storage);

		std::vector<Alarm> getAlarms();
		Alarm* getAlarmById(int alarmId);
		std::vector<Alarm> getActiveAlarms();
		void setAlarm(const Alarm& alarm);
		void getAlarmsFromStorage();
		Alarm* getNextActiveAlarm();
		void dismissAlarm(int alarmId);
		void deleteAlarm(int alarmId);
		bool snoozeAlarm(Alarm& alarm);
};
