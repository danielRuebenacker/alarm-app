#include <lv/lv.hpp>
#include <functional>
#include <algorithm>
#include <array>

#include "../views/IAlarmConfigView.h"
#include "../../domain/Alarm.h"


class LvglAlarmConfigView : public IAlarmConfigView {
private:
	lv::ObjectView parent_;
    lv::ObjectView root_;
    
    // UI Widgets
    lv::Roller hourRoller_;
    lv::Roller minuteRoller_;
    std::array<lv::Button, Days::Count> dayButtons_;
    lv::Dropdown puzzleDropdown_;
    lv::Roller snoozeMinRoller_;
    lv::Roller maxSnoozeRoller_;  // Model State
    Alarm currentAlarm_;

    // Presenter Callbacks
    std::function<void(const Alarm &)> onUpdateConfigChangedCallback_;
    std::function<void()> onBackButtonClickedCallback_;

    void handleBackButtonClicked() {
        if (onBackButtonClickedCallback_) onBackButtonClickedCallback_();
    }

	void handleSaveButtonClicked() {
		// get time
		currentAlarm_.setTime(hourRoller_.selected(), minuteRoller_.selected());

		// get days
		Days updatedDays;
		for (size_t i = 0; i < Days::Count; ++i) {
			if (dayButtons_[i].has_state(LV_STATE_CHECKED)) {
				updatedDays.set(static_cast<Days::Day>(i));
			}
		}
		currentAlarm_.setDays(updatedDays);

		// get puzzle type
		currentAlarm_.setPuzzleType(static_cast<PuzzleType>(puzzleDropdown_.selected()));

		// get snoozes
		int snoozeMinutes = snoozeMinRoller_.selected() + 1; // Index 0 = 1 min
		int maxSnoozes = maxSnoozeRoller_.selected();         // Index 0 = 0 snoozes

		currentAlarm_.setSnoozeMinutes(snoozeMinutes);
		currentAlarm_.setMaxSnoozes(maxSnoozes);

		// pass back to presenter
		if (onUpdateConfigChangedCallback_) {
			onUpdateConfigChangedCallback_(currentAlarm_);
		}
	}

    void buildUI() {
		root_ = lv::vbox(parent_)
				.fill()
				.gap(12)
				.add_flag(LV_OBJ_FLAG_SCROLLABLE);		// make scrollable

		// header (text and back button)
        auto header = lv::hbox(root_).fill_width().align(LV_ALIGN_TOP_MID);
        
        lv::Button::create(header)
            .text(lv::symbol::left)
            .on_click<&LvglAlarmConfigView::handleBackButtonClicked>(this);

        lv::Label::create(header)
            .text("Configure Alarm")
            .font(&lv_font_montserrat_24);

		// time picker
        auto timeContainer = lv::hbox(root_).center_content().gap(10);

        hourRoller_ = lv::Roller::create(timeContainer)
            .options(
					 "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23",
					 LV_ROLLER_MODE_INFINITE
			)
            .visible_rows(3);

        lv::Label::create(timeContainer).text(":").font(&lv_font_montserrat_24);

        std::string minOptions;
        for (int i = 0; i < 60; ++i) {
            if (i < 10) minOptions += "0";
            minOptions += std::to_string(i) + (i == 59 ? "" : "\n");
        }
        minuteRoller_ = lv::Roller::create(timeContainer)
            .options(minOptions.c_str(), LV_ROLLER_MODE_INFINITE)
            .visible_rows(3);

		// days selector
        lv::Label::create(root_).text("Repeat Days:");
        auto daysContainer = lv::hbox(root_).fill_width().gap(4);
        
        const std::array<const char*, Days::Count> dayLabels = {"S", "M", "T", "W", "T", "F", "S"};
        for (size_t i = 0; i < Days::Count; ++i) {
            dayButtons_[i] = lv::Button::create(daysContainer)
                .size(38, 38)
                .add_flag(LV_OBJ_FLAG_CHECKABLE);
            
            lv::Label::create(dayButtons_[i]).text(dayLabels[i]).center();
        }

		// puzzle 
        lv::Label::create(root_).text("Puzzle Type:");
        puzzleDropdown_ = lv::Dropdown::create(root_).fill_width();
        puzzleDropdown_.options("MATHS\nMEMORY\nCAPTCHA");

		// snoozes
		auto snoozeGroup = lv::hbox(root_).fill_width().center_content().gap(20);

		auto minCol = lv::vbox(snoozeGroup).center_content();
		lv::Label::create(minCol).text("Snooze (min):");

		std::string snoozeMinOptions;
		for (int i = 1; i <= 30; ++i) {
			snoozeMinOptions += std::to_string(i) + (i == 30 ? "" : "\n");
		}

		snoozeMinRoller_ = lv::Roller::create(minCol)
			.options(snoozeMinOptions.c_str(), LV_ROLLER_MODE_NORMAL)
			.visible_rows(3);

		// max snoozes
		auto maxCol = lv::vbox(snoozeGroup).center_content();
		lv::Label::create(maxCol).text("Max Snoozes:");

		std::string maxSnoozeOptions;
		for (int i = 0; i <= 10; ++i) {
			maxSnoozeOptions += std::to_string(i) + (i == 10 ? "" : "\n");
		}

		maxSnoozeRoller_ = lv::Roller::create(maxCol)
			.options(maxSnoozeOptions.c_str(), LV_ROLLER_MODE_NORMAL)
			.visible_rows(3);

		// save buttons
		lv::Button::create(root_)
			.fill_width()
			.text("Save Alarm")
			.on_click<&LvglAlarmConfigView::handleSaveButtonClicked>(this);
    }

public:
    // Router-compatible Constructor
    LvglAlarmConfigView(const Alarm &currentAlarm, lv::ObjectView parent = lv::screen_active())
        : parent_(parent), currentAlarm_(currentAlarm) {
        buildUI();
        displayAlarmInformation(currentAlarm_); // populate with alarm info
    }

	void displayAlarmInformation(const Alarm& alarm) override {
		currentAlarm_ = alarm;

		// add hour roller info
		hourRoller_.selected(alarm.getTime().hour());
		minuteRoller_.selected(alarm.getTime().min());

		// sync days
		const Days& days = alarm.getDays();
		for (size_t i = 0; i < Days::Count; ++i) {
			if (days.isActive(static_cast<Days::Day>(i))) {
				dayButtons_[i].add_state(LV_STATE_CHECKED);
			} else {
				dayButtons_[i].remove_state(LV_STATE_CHECKED);
			}
		}

		// sync dropdown
		puzzleDropdown_.selected(static_cast<uint16_t>(alarm.getPuzzleType()));


		// sync snooze info
		int snoozeMinIdx = std::clamp(alarm.getSnoozeMinutes() - 1, 0, 29);
		snoozeMinRoller_.selected(snoozeMinIdx);

		int maxSnoozeIdx = std::clamp(alarm.getMaxSnoozes(), 0, 10);
		maxSnoozeRoller_.selected(maxSnoozeIdx);
	}

    void setOnUpdateConfigChangedCallback(std::function<void(const Alarm &)> callback) override {
        onUpdateConfigChangedCallback_ = callback;
    }

    void setOnBackClicked(std::function<void()> callback) override {
        onBackButtonClickedCallback_ = callback;
    }
};
