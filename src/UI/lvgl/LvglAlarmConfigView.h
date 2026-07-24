#include <lv/lv.hpp>
#include <functional>

#include "../views/IAlarmConfigView.h"
#include "../../domain/Alarm.h"


class LvglAlarmConfigView : public IAlarmConfigView {
private:
    // Model State
    Alarm currentAlarm_;

    // Presenter Callbacks
    std::function<void(const Alarm &)> onUpdateConfigChangedCallback_;
    std::function<void()> onBackButtonClickedCallback_;
public:
    // Router-compatible Constructor
    LvglAlarmConfigView(const Alarm &currentAlarm, lv::ObjectView parent = lv::screen_active())
        : parent_(parent), currentAlarm_(currentAlarm) {
    }
    void setOnUpdateConfigChangedCallback(std::function<void(const Alarm &)> callback) override {
        onUpdateConfigChangedCallback_ = callback;
    }

    void setOnBackClicked(std::function<void()> callback) override {
        onBackButtonClickedCallback_ = callback;
    }
};
