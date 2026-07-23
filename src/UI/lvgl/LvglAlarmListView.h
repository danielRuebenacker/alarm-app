#include <lv/lv.hpp>
#include <functional>

#include "../views/IAlarmListView.h"
#include "../../domain/Alarm.h"


class LvglAlarmListView : public IAlarmListView {
 private:
	lv::ObjectView root_;
	lv::ObjectView alarmsContainer_;

	std::function<void()> onBackButtonClickedCallback_;
	std::function<void(int)> onAlarmToggledCallback_;
	std::function<void(int)> onAlarmClickedCallback_;
	std::function<void(int)> onAddAlarmClickedCallback_;

	void handleBackButtonClicked() {
		if (onBackButtonClickedCallback_) onBackButtonClickedCallback_();
	}

	void handleAlarmToggled(lv::Event e) {
		uintptr_t alarmId = (uintptr_t)e.target().get_user_data();
		if (onAlarmToggledCallback_) onAlarmToggledCallback_(alarmId);
	}

	void handleAlarmClicked(lv::Event e) {
		uintptr_t alarmId = (uintptr_t)e.target().get_user_data();
		if (onAlarmClickedCallback_) onAlarmClickedCallback_(alarmId);
	}

	void handleBackButtonClicked() {
		if (onAddAlarmClickedCallback_) onAddAlarmClickedCallback_();
	}

public:
	LvglAlarmListView(lv::ObjectView parent = lv::screen_active()) {
		// create root element
		root_ = lv::vbox(parent).fill().center_content();
		lv::Label::create(root_).text("Alarms:");
		// back button
		lv::Button::create(root_)
			.text(lv::symbol::left)
			.on_click<&LvglAlarmListView::handleBackButtonClicked>(this);
		// create vbox for alarms
		alarmsContainer_ = lv::vbox(root_).fill().center_content();
	}

    void displayListOfAlarms(const std::vector<Alarm>& listOfAlarms) override {
		for (const Alarm& alarm : listOfAlarms) {
			auto alarmCard = lv::Button::create(alarmsContainer_);
			alarmCard
				.fill_width()
				.bg_color(lv::rgb(0x2196F3))
				.bg_opa(255);
			// add alarm id

			auto leftTextGroup = lv::vbox(alarmCard);
			leftTextGroup.align(LV_ALIGN_LEFT_MID, 10, 0);

			// display time
			auto timeLabel = lv::Label::create(leftTextGroup).text(alarm.getTime().toString())
				.text_font(&lv_font_montserrat_24);

			auto detailsLabel = lv::Label::create(leftTextGroup)
				.text(std::string(to_string(alarm.getPuzzleType()))
					  + " | Zz: "
					  + std::to_string(alarm.getMaxSnoozes())
					  + "x" + std::to_string(alarm.getSnoozeMinutes()) + "m");

				// 4. Add the Toggle Switch (The second clickable target)
			auto alarmSwitch = lv::Switch::create(alarmCard)
				.align(LV_ALIGN_RIGHT_MID, -10, 0);;

			if (alarm.isActive()) {
				alarmSwitch.add_state(LV_STATE_CHECKED);
			}

			// add data to card and switch
			alarmCard.user_data((void*)(uintptr_t)alarm.getId());
			alarmSwitch.user_data((void*)(uintptr_t)alarm.getId());

			alarmCard.on_click<&LvglAlarmListView::handleAlarmClicked>(this);
			alarmSwitch.on_click<&LvglAlarmListView::handleAlarmToggled>(this);
		}
    }

    void setOnAlarmToggled(std::function<void(int)> callback) override {
		onAlarmToggledCallback_ = callback;
    }

    void setOnBackButtonPress(std::function<void()> callback) override {
		onBackButtonClickedCallback_ = callback;
    }

    void setOnAlarmPressed(std::function<void(int)> callback) override {
		onAlarmClickedCallback_ = callback;
    }

	void setOnAddAlarmClicked(std::function<void> callback) override {
		onAddAlarmClickedCallback_ = callback;
	}
};
