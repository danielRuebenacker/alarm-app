#pragma once
#include <functional>

class IAlarmRingingView {
    public:
        virtual ~IAlarmRingingView() = default;
        // for button to snooze
        virtual void setOnSnoozeClicked(std::function<void()> callback) = 0;
        virtual void setOnDismissedClicked(std::function<void()> callback) = 0;
};
