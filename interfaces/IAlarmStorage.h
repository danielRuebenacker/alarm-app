#include <vector>
#include "../domain/Alarm.h"
using std::vector;

// allows to save to/load from flash memory/SD card
class IAlarmStorage {
	public: 
		virtual ~IAlarmStorage() = default;
		virtual void saveAlarms(const vector<AlarmData> alarms) = 0;
		virtual vector<AlarmData> loadAlarms() = 0;
};
