#pragma once
#include "../views/IRingingView.h"
#include "../../interfaces/IRouter.h"
#include "../../domain/AlarmManager.h"
#include "../../types/ScreenType.h"

#include "Presenter.h"

class RingingPresenter : public Presenter {
  private:
	IAlarmRingingView& view_;
	IRouter& router_;
	AlarmManager& alarmManager_;
	int alarmId_;

  public:
	RingingPresenter(IAlarmRingingView& view, IRouter& router,
					 AlarmManager& alarmManager, int alarmId)
		: view_(view), router_(router), alarmManager_(alarmManager), alarmId_(alarmId) {
		const Alarm* alarm = alarmManager_.getAlarmById(alarmId_);
		if (alarm) {
			view_.displayAlarm(*alarm);
			view_.setSnoozeAvailable(alarm->snoozePossible() && alarm->getMaxSnoozes() > 0);
		} else {
			view_.setSnoozeAvailable(false);
		}

		view_.setOnSnoozeClicked([this]() { onSnooze(); });
		view_.setOnDismissedClicked([this]() { onDismiss(); });
	}

	void onSnooze() {
		// snoozing re-arms the alarm for a few minutes; the scheduler reacts
		if (alarmManager_.snoozeAlarm(alarmId_)) {
			router_.navigateTo(ScreenType::Home);
		}
	}

	void onDismiss() {
		// solving the puzzle is what actually dismisses the alarm
		router_.navigateTo(ScreenType::Puzzle, alarmId_);
	}
};
