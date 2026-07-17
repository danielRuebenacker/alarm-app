#include <vector>

#include "src/interfaces/IStorage.h"

class MockStorage : public IStorage {
private:
    std::vector<Alarm> storedAlarms;
    std::vector<int> storedDismissedAlarmIds;

public:
    void addAlarmToStoredAlarms(const Alarm& alarm) {
        storedAlarms.push_back(alarm);
    }

    void addToStoredDismissedIds(int id) {
        storedDismissedAlarmIds.push_back(id);
    }

    std::vector<Alarm> loadAlarms() const override {
        return storedAlarms;
    }

    std::vector<int> loadDismissedAlarmIds() override {
        return storedDismissedAlarmIds;
    }

    void saveAlarms(const std::vector<Alarm> alarms) override { storedAlarms = alarms; }
    void saveDismissed(const std::vector<int> dismissedAlarmIds) override { storedDismissedAlarmIds = dismissedAlarmIds; }

	int getDaysFrom1970ToSleepDay() const override { return -1; }
	int getSleepDay() const override { return -1; }
	void saveSleepStateInfo(int daysSince1970, const Days::Day currentDay) override {};
};
