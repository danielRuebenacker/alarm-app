#pragma once
#include "../../domain/AlarmManager.h"
#include "../views/IAlarmListView.h"
#include "../../interfaces/IRouter.h"
#include "src/types/ScreenType.h"

class AlarmListPresenter {
private:
    IAlarmListView& view_;
    IRouter& router_;
    AlarmManager& alarmManager_;

public:
  AlarmListPresenter(IAlarmListView &view, IRouter& router, AlarmManager &alarmManager)
      : view_(view), router_(router), alarmManager_(alarmManager) {

        // what to do when we press a single alarm in the list
        view_.setOnAlarmPressed([this](int alarmId) {
                router_.navigateTo(ScreenType::AlarmConfig, alarmId);
        });

        // what to do when an alarm is toggled
        view_.setOnAlarmToggled([this](int alarmId) {
                Alarm* alarm = alarmManager_.getAlarmById(alarmId);
                if (!alarm) return;
                alarm->toggle();
        });

        // only one way to get to alarm list is from home, so send back there
        view_.setOnBackButtonPress([this]() {
                router_.navigateTo(ScreenType::Home);
        });

  } 
};
