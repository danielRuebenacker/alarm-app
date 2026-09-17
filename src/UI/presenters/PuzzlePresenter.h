#pragma once
#include <chrono>
#include <memory>

#include "../views/IPuzzleView.h"
#include "../../interfaces/IRouter.h"
#include "../../interfaces/IScheduler.h"
#include "../../domain/AlarmManager.h"
#include "../../domain/PuzzleFactory.h"
#include "../../types/ScreenType.h"

#include "Presenter.h"

class PuzzlePresenter : public Presenter {
  private:
	IPuzzleView& view_;
	IRouter& router_;
	AlarmManager& alarmManager_;
	IScheduler& scheduler_;
	PuzzleFactory& puzzleFactory_;
	int alarmId_;

	int totalSeconds_;
	int secondsLeft_;
	std::unique_ptr<IPuzzle> puzzle_;
	IScheduler::TimerHandle tickHandle_ = IScheduler::kInvalidHandle;

	void scheduleTick() {
		// never keep two countdown timers alive at once
		scheduler_.cancel(tickHandle_);
		tickHandle_ = scheduler_.scheduleOnce(std::chrono::seconds(1),
											 [this]() { onCountdownTick(); });
	}

	// NOTE: deliberately not Presenter::onTick(): the router's clock tick calls
	// that every second and must not drive (or duplicate) the countdown
	void onCountdownTick() {
		--secondsLeft_;
		if (secondsLeft_ <= 0) {
			// no input in time: ring again
			scheduler_.cancel(tickHandle_);
			tickHandle_ = IScheduler::kInvalidHandle;
			router_.navigateTo(ScreenType::Ringing, alarmId_);
			return;
		}
		view_.updateTimeoutBar(secondsLeft_ * 100 / totalSeconds_);
		scheduleTick();
	}

	// any key press buys back the full solving time
	void onUserInput() {
		secondsLeft_ = totalSeconds_;
		view_.updateTimeoutBar(100);
		scheduleTick();
	}

	void onSubmit(const PuzzleResponse& response) {
		if (!puzzle_ || puzzle_->verifySolution(response)) {
			scheduler_.cancel(tickHandle_);
			tickHandle_ = IScheduler::kInvalidHandle;
			alarmManager_.dismissAlarm(alarmId_);
			router_.navigateTo(ScreenType::Home);
		}
		// wrong answer: stay on the puzzle, the timeout keeps running
	}

  public:
	PuzzlePresenter(IPuzzleView& view, IRouter& router, AlarmManager& alarmManager,
					IScheduler& scheduler, PuzzleFactory& puzzleFactory, int alarmId,
					int timeoutSeconds = 60)
		: view_(view), router_(router), alarmManager_(alarmManager), scheduler_(scheduler),
		  puzzleFactory_(puzzleFactory), alarmId_(alarmId), totalSeconds_(timeoutSeconds),
		  secondsLeft_(timeoutSeconds) {
		const Alarm* alarm = alarmManager_.getAlarmById(alarmId_);
		if (alarm) {
			puzzle_ = puzzleFactory_.createPuzzle(alarm->getPuzzleType());
		}
		if (puzzle_ && alarm) {
			view_.loadPuzzle(alarm->getPuzzleType(), *puzzle_);
		}

		view_.setOnSubmitCallback([this](const PuzzleResponse& response) { onSubmit(response); });
		view_.setOnAnyInputCallback([this]() { onUserInput(); });

		view_.updateTimeoutBar(100);
		scheduleTick();
	}

	~PuzzlePresenter() override {
		scheduler_.cancel(tickHandle_);
	}
};
