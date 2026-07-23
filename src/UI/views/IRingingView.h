#pragma once
#include <functional>

#include "View.h"

class IAlarmRingingView : public View {
    public:
        virtual ~IAlarmRingingView() = default;
        // for button to snooze
        virtual void setOnSnoozeClicked(std::function<void()> callback) = 0;
        virtual void setOnDismissedClicked(std::function<void()> callback) = 0;
};
