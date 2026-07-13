#pragma once
#include <vector>
#include "../domain/Alarm.h"

// allows to save to/load from flash memory/SD card
class IStorage {
	public: 
		virtual ~IStorage() = default;
		virtual void saveAlarms(const std::vector<Alarm> alarms) = 0;
		virtual void saveDismissed(const std::vector<int> dismissedAlarmIds) = 0;
		virtual std::vector<Alarm> loadAlarms() const = 0;
        virtual std::vector<int> loadDismissedAlarmIds() = 0;
};
