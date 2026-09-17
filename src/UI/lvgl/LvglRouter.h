#include <memory>
#include <lv/lv.hpp>

#include "../../interfaces/IRouter.h"
#include "../../interfaces/IClock.h"
#include "../../interfaces/IScheduler.h"
#include "../../types/ScreenType.h"
#include "../../types/TimePoint.h"
#include "../../domain/AlarmManager.h"
#include "../../domain/PuzzleFactory.h"

// views
#include "../views/View.h"
#include "LvglHomeScreenView.h"
#include "LvglAlarmListView.h"
#include "LvglAlarmConfigView.h"
#include "LvglRingingView.h"
#include "LvglPuzzleView.h"

// presenters
#include "../presenters/Presenter.h"
#include "../presenters/HomeScreenPresenter.h"
#include "../presenters/AlarmListPresenter.h"
#include "../presenters/AlarmConfigPresenter.h"
#include "../presenters/RingingPresenter.h"
#include "../presenters/PuzzlePresenter.h"

class LvglRouter : public IRouter {
  private:
	IClock& clock_;
	AlarmManager& manager_;
	IScheduler& scheduler_;
	PuzzleFactory& puzzleFactory_;

	std::unique_ptr<View> currentView_;
	std::unique_ptr<Presenter> currentPresenter_;

	// ticks the active presenter (e.g. to update the home screen clock)
	lv::Timer clockTimer_;

	void onClockTick() {
		if (currentPresenter_) currentPresenter_->onTick();
	}

  public:
	LvglRouter(IClock& clock, AlarmManager& manager, IScheduler& scheduler, PuzzleFactory& puzzleFactory)
		: clock_(clock), manager_(manager), scheduler_(scheduler), puzzleFactory_(puzzleFactory) {
		clockTimer_ = lv::Timer::create<&LvglRouter::onClockTick>(1000, this);
	}

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
			case ScreenType::Ringing: {
				auto ringingView = std::make_unique<LvglRingingView>();
				auto ringingPresenter = std::make_unique<RingingPresenter>(
					*ringingView, *this, manager_, targetAlarmId);

				currentView_ = std::move(ringingView);
				currentPresenter_ = std::move(ringingPresenter);
				break;
			}
			case ScreenType::Puzzle: {
				auto puzzleView = std::make_unique<LvglPuzzleView>();
				auto puzzlePresenter = std::make_unique<PuzzlePresenter>(
					*puzzleView, *this, manager_, scheduler_, puzzleFactory_, targetAlarmId);

				currentView_ = std::move(puzzleView);
				currentPresenter_ = std::move(puzzlePresenter);
				break;
			}
            default:
				break;
		}
    }
};
