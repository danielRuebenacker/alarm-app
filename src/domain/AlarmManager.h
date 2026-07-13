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
        IStorage&  storage_;

		std::vector<Alarm> alarms;
        std::vector<int> dismissedAlarmIds;
		void makeActiveAlarms();
	public:
		AlarmManager(const IClock& clock, IStorage& storage);

		std::vector<Alarm> getAlarms();
		Alarm* getAlarmById(int alarmId);
		std::vector<Alarm> getActiveAlarms();
		void addAlarm(const Alarm& alarm);
		void getAlarmsFromStorage();
		const Alarm* getNextActiveAlarm();
		void dismissAlarm(int alarmId);
		void deleteAlarm(int alarmId);
		bool snoozeAlarm(Alarm& alarm);
        bool wasAlarmDismissed(int alarmId);
};
