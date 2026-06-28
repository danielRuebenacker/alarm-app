#pragma once
#include <memory>
#include <vector>
#include "./Alarm.h"
#include "../interfaces/IStorage.h"
#include "../interfaces/IClock.h"

// stores alarms, gets/sets, returns next active alarm, can cancel alarm
class AlarmManager {
	private:
		std::shared_ptr<IClock> clock_;
		std::shared_ptr<IStorage> storage_;

		// all: sorted by how many minutes until ring
		std::vector<Alarm> alarms;
		void makeActiveAlarms();
	public:
		AlarmManager(std::shared_ptr<IClock> clock, std::shared_ptr<IStorage> storage);

		std::vector<Alarm> getAlarms();
		Alarm* getAlarm(int alarmId);
		std::vector<Alarm> getActiveAlarms();
		void setAlarm(const Alarm& alarm);
		void getAlarmsFromStorage();
		Alarm* getNextActiveAlarm(const TimePoint& now);
		void dismissAlarm(int alarmId);
		void deleteAlarm(int alarmId);
		bool snoozeAlarm(Alarm& alarm);
};
