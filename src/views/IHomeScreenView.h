#pragma once
#include <string>
#include <functional>

class IHomeScreenView {
    public:
        virtual ~IHomeScreenView() = default;
        // display time
        virtual void updateTime(const std::string& newTime) = 0;
        // route to alarm list screen
        virtual void setOnClickAlarmList(std::function<void()> callback) = 0;
};
