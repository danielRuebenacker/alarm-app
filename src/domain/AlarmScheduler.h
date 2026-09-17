#pragma once
#include "../interfaces/IScheduler.h"
#include "../interfaces/IClock.h"
#include "../domain/AlarmManager.h"
#include <chrono>
#include <functional>
#include <utility>


class AlarmScheduler {
public:
	AlarmScheduler(AlarmManager& alarmManager, IClock& clock, IScheduler& scheduler)
		: scheduler_(scheduler), alarmManager_(alarmManager), clock_(clock) {
		// re-arm automatically whenever alarm data changes
		alarmManager_.setOnAlarmsChanged([this]() { rescheduleNext(); });
	}

	void setOnAlarmDue(std::function<void(const Alarm&)> cb) { onAlarmDue_ = std::move(cb); }

	void start() { rescheduleNext(); }        
  

private:
	void rescheduleNext() {
		// make sure the current timer is off
		scheduler_.cancel(currentHandle_);
		currentHandle_ = IScheduler::kInvalidHandle;

		// the manager owns dismissed-for-the-day state, so ask it for the
		// actual delay rather than recomputing from the alarm alone
		auto delay = alarmManager_.getDurationUntilNextRing();
		if (delay == std::chrono::milliseconds::max()) return; // nothing coming

		currentHandle_ = scheduler_.scheduleOnce(delay, [this]() { onTimerFired(); });
	}

	void onTimerFired() {
		const Alarm* alarm = alarmManager_.getNextActiveAlarm();
		if (!alarm) {
			rescheduleNext();
			return;
		}

		// a due snooze fires even if the alarm's own time no longer matches
		// today's mask (it may have crossed midnight)
		if (alarmManager_.isSnoozing() ||
			alarm->shouldTrigger(clock_.now(), clock_.getCurrentDay())) {
			// mark it ringing so it is not re-armed until resolved
			alarmManager_.startRinging(alarm->getId());
			if (onAlarmDue_) onAlarmDue_(*alarm);
		}
		rescheduleNext();
	}    

	IScheduler& scheduler_;
	AlarmManager& alarmManager_;
	IClock& clock_;
	IScheduler::TimerHandle currentHandle_ = IScheduler::kInvalidHandle;
	std::function<void(const Alarm&)> onAlarmDue_;
};
