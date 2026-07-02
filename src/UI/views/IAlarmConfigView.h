#pragma once
#include "../domain/Alarm.h"
#include <functional>

class IAlarmConfigView {
    public:
        virtual ~IAlarmConfigView() = default;
        // display information
        virtual void displayAlarmInformation(const Alarm& alarm) = 0;
        // updating alarm details
        virtual void setOnUpdateConfigChangedCallback(std::function<void(const Alarm& newAlarmConfig)> callback) = 0;
        // navigate back (to alarm list)
        virtual void setOnBackClicked(std::function<void()> callback) = 0;
};
