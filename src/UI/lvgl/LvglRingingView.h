#pragma once

#include "../views/IRingingView.h"
#include <lv/lv.hpp>
#include <functional>

class LvglRingingView : public IAlarmRingingView {
  private:
	lv::Label timeLabel_;
	lv::Button snoozeButton_;
	lv::Button dismissButton_;

	std::function<void()> onSnoozeClickedCallback_;
	std::function<void()> onDismissedClickedCallback_;

	void handleSnooze() {
		// copy before invoking: the callback may destroy this view
		auto callback = onSnoozeClickedCallback_;
		if (callback) callback();
	}

	void handleDismiss() {
		auto callback = onDismissedClickedCallback_;
		if (callback) callback();
	}

  public:
	LvglRingingView(lv::ObjectView parent = lv::screen_active()) {
		auto root = lv::vbox(parent).fill().center_content().gap(24);

		lv::Label::create(root)
			.text("ALARM RINGING")
			.font(&lv_font_montserrat_24);

		timeLabel_ = lv::Label::create(root)
			.text("00:00")
			.font(&lv_font_montserrat_24);

		snoozeButton_ = lv::Button::create(root)
			.size(200, 50)
			.text("Snooze")
			.on_click<&LvglRingingView::handleSnooze>(this);

		dismissButton_ = lv::Button::create(root)
			.size(200, 50)
			.text("Dismiss")
			.on_click<&LvglRingingView::handleDismiss>(this);
	}

	void displayAlarm(const Alarm& alarm) override {
		timeLabel_.text(alarm.getTime().toString().c_str());
	}

	void setSnoozeAvailable(bool available) override {
		snoozeButton_.enabled(available);
	}

	void setOnSnoozeClicked(std::function<void()> callback) override {
		onSnoozeClickedCallback_ = std::move(callback);
	}

	void setOnDismissedClicked(std::function<void()> callback) override {
		onDismissedClickedCallback_ = std::move(callback);
	}
};
