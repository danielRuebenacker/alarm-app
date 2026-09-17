#pragma once
#include <functional>

#include "src/UI/views/IRingingView.h"

class MockRingingView : public IAlarmRingingView {
  public:
	int displayCount = 0;
	bool snoozeAvailable = false;

	std::function<void()> onSnoozeClicked;
	std::function<void()> onDismissedClicked;

	void displayAlarm(const Alarm&) override { ++displayCount; }

	void setSnoozeAvailable(bool available) override { snoozeAvailable = available; }

	void setOnSnoozeClicked(std::function<void()> callback) override {
		onSnoozeClicked = std::move(callback);
	}

	void setOnDismissedClicked(std::function<void()> callback) override {
		onDismissedClicked = std::move(callback);
	}

	void clickSnooze() {
		if (onSnoozeClicked) onSnoozeClicked();
	}

	void clickDismiss() {
		if (onDismissedClicked) onDismissedClicked();
	}
};
