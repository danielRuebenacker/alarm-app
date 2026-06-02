#pragma once
#include <vector>
#include "./Alarm.h"

// stores alarms, gets/sets, returns next active alarm, can cancel alarm
class AlarmManager {
	private:
		// all
		std::vector<Alarm> alarms;
		// sorted, insertion needs to maintain sort order
		std::vector<Alarm> activeAlarms;
	public:
		std::vector<Alarm> getAlarms();
		std::vector<Alarm> getActiveAlarms();
		void setAlarm(Alarm alarm);
		Alarm getNextActiveAlarm();
		void cancelAlarm(Alarm alarm);
};
