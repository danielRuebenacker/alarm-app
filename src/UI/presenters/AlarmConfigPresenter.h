#include "../views/IAlarmConfigView.h"
#include "../../domain/AlarmManager.h"
#include "../../interfaces/IRouter.h"

#include "Presenter.h"

class AlarmConfigPresenter : public Presenter {
	IAlarmConfigView &view_;
	AlarmManager &manager_;
	IRouter &router_;
 public:
	AlarmConfigPresenter(IAlarmConfigView &view, IRouter &router, AlarmManager &manager)
		: view_(view), manager_(manager), router_(router) {
		// 
		view_.setOnBackClicked([this] () {
			router_.navigateTo(ScreenType::AlarmList);
		});

		view_.setOnUpdateConfigChangedCallback([this] (const Alarm &alarm) {
			manager_.addAlarm(alarm);
			router_.navigateTo(ScreenType::AlarmList);
		});
		
	}
};
