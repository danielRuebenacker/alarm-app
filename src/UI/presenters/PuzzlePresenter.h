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
	// set once the puzzle is solved: the screen is on its way out, so further
	// input must not restart the countdown
	bool solved_ = false;

	// how long the "correct" flash stays up before we leave the puzzle screen
	static constexpr int kCorrectFlashMs = 800;

	void cancelTick() {
		scheduler_.cancel(tickHandle_);
		tickHandle_ = IScheduler::kInvalidHandle;
	}

	void scheduleTick() {
		// never keep two countdown timers alive at once
		cancelTick();
		tickHandle_ = scheduler_.scheduleOnce(std::chrono::seconds(1),
											 [this]() { onCountdownTick(); });
	}

	// NOTE: deliberately not Presenter::onTick(): the router's clock tick calls
	// that every second and must not drive (or duplicate) the countdown
	void onCountdownTick() {
		--secondsLeft_;
		if (secondsLeft_ <= 0) {
			// no input in time: ring again
			cancelTick();
			router_.navigateTo(ScreenType::Ringing, alarmId_);
			return;
		}
		view_.updateTimeoutBar(secondsLeft_ * 100 / totalSeconds_);
		scheduleTick();
	}

	// any key press buys back the full solving time
	void onUserInput() {
		if (solved_) return;
		secondsLeft_ = totalSeconds_;
		view_.updateTimeoutBar(100);
		scheduleTick();
	}

	void finishSolved() {
		tickHandle_ = IScheduler::kInvalidHandle;
		alarmManager_.dismissAlarm(alarmId_);
		router_.navigateTo(ScreenType::Home);
	}

	void onSubmit(const PuzzleResponse& response) {
		// a puzzle that could not be built can never be solved, so it must not
		// count as a pass: keep ringing instead of dismissing the alarm
		if (!puzzle_) {
			view_.showAnswerFeedback(false);
			return;
		}

		if (!puzzle_->verifySolution(response)) {
			view_.showAnswerFeedback(false);
			return;
		}

		// correct: freeze the countdown and let the view show the success flash
		// before we tear the screen down
		solved_ = true;
		cancelTick();
		view_.showAnswerFeedback(true);
		tickHandle_ = scheduler_.scheduleOnce(std::chrono::milliseconds(kCorrectFlashMs),
											 [this]() { finishSolved(); });
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
			const std::string hint = puzzle_->toHint();
			if (!hint.empty()) {
				view_.showHint(hint);
			}
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
