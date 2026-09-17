#pragma once
#include <chrono>
#include <functional>
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

		// returns the soonest alarm and writes the actual minutes until it rings
		// (accounting for alarms dismissed for the day) into minutesOut
		const Alarm* findNextActiveAlarm(int& minutesOut);

		// notified whenever alarm data changes (so the scheduler can re-arm)
		std::function<void()> onChanged_;
		void notifyChanged();
	public:
		AlarmManager(const IClock& clock, IStorage& storage);

		std::vector<Alarm> getAlarms();
		Alarm* getAlarmById(int alarmId);
		std::vector<Alarm> getActiveAlarms();
		void getAlarmsFromStorage();
		void getDismissedAlarmIdsFromStorage();
		const Alarm* getNextActiveAlarm();
		std::chrono::milliseconds getDurationUntilNextRing();

		void setOnAlarmsChanged(std::function<void()> callback);

		void addAlarm(const Alarm& alarm);
		void dismissAlarm(int alarmId);
		void deleteAlarm(int alarmId);
		bool snoozeAlarm(Alarm& alarm);
        bool wasAlarmDismissed(int alarmId);
		void toggleAlarm(int alarmId);

        Alarm* getMostRecentlyMissedAlarm(int daysFrom1970ToSleepDay, const Days::Day& sleepDay);
};
