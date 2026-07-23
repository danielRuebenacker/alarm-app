#pragma once
#include <vector>
#include <functional>
#include "../../domain/Alarm.h"

#include "View.h"

class IAlarmListView : public View {
    public:
        virtual ~IAlarmListView() = default;
        // list all alarms (with info and toggles)
        virtual void displayListOfAlarms(const std::vector<Alarm> listOfAlarms) = 0;
        // toggle specific alarm
        virtual void setOnAlarmToggled(std::function<void(int)> callback) = 0;
        // button to take home
        virtual void setOnBackButtonPress(std::function<void()> callback) = 0;
        // when alarm pressed, take to its config (using int alarmID)
        virtual void setOnAlarmPressed(std::function<void(int)> callback) = 0;
};
