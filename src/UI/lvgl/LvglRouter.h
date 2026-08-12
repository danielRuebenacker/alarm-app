#include <memory>
#include <lv/lv.hpp>

#include "../../interfaces/IRouter.h"
#include "../../interfaces/IClock.h"
#include "../../types/ScreenType.h"
#include "../../types/TimePoint.h"
#include "../../domain/AlarmManager.h"

// views
#include "../views/View.h"
#include "LvglHomeScreenView.h"
#include "LvglAlarmListView.h"
#include "LvglAlarmConfigView.h"

// presenters
#include "../presenters/Presenter.h"
#include "../presenters/HomeScreenPresenter.h"
#include "../presenters/AlarmListPresenter.h"
#include "../presenters/AlarmConfigPresenter.h"

class LvglRouter : public IRouter {
  private:
	IClock& clock_;
	AlarmManager& manager_;

	std::unique_ptr<View> currentView_;
	std::unique_ptr<Presenter> currentPresenter_;

  public:
	LvglRouter(IClock& clock, AlarmManager& manager) : clock_(clock), manager_(manager) {}

    void navigateTo(ScreenType screen, int targetAlarmId = -1) override {
		currentView_.reset();
		currentPresenter_.reset();
		lv::screen_active().clean();

		switch (screen) {
			case ScreenType::Home: {
				auto homeView = std::make_unique<LvglHomeScreenView>();
				auto homePresenter = std::make_unique<HomeScreenPresenter>(*homeView, *this, clock_);

				currentView_ = std::move(homeView);
				currentPresenter_ = std::move(homePresenter);
				break;
            }
			case ScreenType::AlarmList: {
				auto alarmListView = std::make_unique<LvglAlarmListView>();
				auto alarmListPresenter = std::make_unique<AlarmListPresenter>(*alarmListView, *this, manager_);

				currentView_ = std::move(alarmListView);
				currentPresenter_ = std::move(alarmListPresenter);
				break;
			}
			case ScreenType::AlarmConfig: {
				Alarm alarm(clock_.now(), PuzzleType::MATHS, {}); 

				if (targetAlarmId != -1) {
					const Alarm *pAlarm = manager_.getAlarmById(targetAlarmId);
					if (pAlarm) {
						alarm = *pAlarm;
					}
				}

				auto alarmConfigView = std::make_unique<LvglAlarmConfigView>(alarm);
				auto alarmConfigPresenter = std::make_unique<AlarmConfigPresenter>(*alarmConfigView, *this, manager_);

				currentView_ = std::move(alarmConfigView);
				currentPresenter_ = std::move(alarmConfigPresenter);
				break;
			}
            case ScreenType::Ringing:
                break;
            default:
				break;
		}
    }
};
