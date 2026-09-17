#pragma once
#include <functional>

#include "../../domain/Alarm.h"

#include "View.h"

class IAlarmRingingView : public View {
    public:
        virtual ~IAlarmRingingView() = default;
        // show which alarm is currently ringing
        virtual void displayAlarm(const Alarm& alarm) = 0;
        // enable/disable the snooze button
        virtual void setSnoozeAvailable(bool available) = 0;
        // for button to snooze
        virtual void setOnSnoozeClicked(std::function<void()> callback) = 0;
        virtual void setOnDismissedClicked(std::function<void()> callback) = 0;
};
