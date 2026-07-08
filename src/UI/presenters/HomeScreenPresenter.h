#pragma once
#include "../views/IHomeScreenView.h"
#include "../../interfaces/IRouter.h"
#include "../../interfaces/IClock.h"
#include "../../types/ScreenType.h"

class HomeScreenPresenter {
    private:
        IHomeScreenView& view_;
        IRouter& router_;
        IClock& clock_;

    public:
        HomeScreenPresenter(IHomeScreenView& view, IRouter& router, IClock& clock)
        : view_(view), router_(router), clock_(clock) {
          // specify what view should do
            view.setOnClickAlarmList([this]() {
                    this->router_.navigateTo(ScreenType::AlarmList);
            });

            // initial update
            view_.updateTime(clock_.now().toString());
        };

        void onSystemClockTick() {
            view_.updateTime(clock_.now().toString());
        }
};
