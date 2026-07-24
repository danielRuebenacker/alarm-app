#include "../views/IHomeScreenView.h"
#include <lv/lv.hpp>
#include <functional>
#include <string>


class LvglHomeScreenView : public IHomeScreenView {
private:
	lv::Label timeLabel_;
	lv::Button alarmListButton_;

    // callback to go to alarm list
	std::function<void()> onClickAlarmListCallback_;

	void handleAlarmListClick(lv::Event e) {
		if (onClickAlarmListCallback_) onClickAlarmListCallback_();
	}    

public:
  LvglHomeScreenView(lv::ObjectView parent = lv::screen_active()) {
	  auto root = lv::vbox(parent).fill().center_content();

	  timeLabel_ = lv::Label::create(root).font(&lv_font_montserrat_24).text("Time: 00:00");	

      alarmListButton_ = lv::Button::create(root).text("View Alarms")
		.on_click<&LvglHomeScreenView::handleAlarmListClick>(this);						
	}

    void updateTime(const std::string &newTime) override {
		timeLabel_.text(newTime);
    }

    void setOnClickAlarmList(std::function<void()> callback) override {
		onClickAlarmListCallback_ = callback;
    }
};
