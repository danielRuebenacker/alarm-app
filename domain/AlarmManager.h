#pragma once
#include <vector>
#include "./Alarm.h"

// stores alarms, gets/sets, returns next active alarm, can cancel alarm
class AlarmManager {
	public:
		std::vector<Alarm> getAlarms();
		void setAlarm(Alarm alarm);
		Alarm getNextActiveAlarm();
		void cancelAlarm(Alarm alarm);
};
