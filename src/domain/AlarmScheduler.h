#pragma once
#include "../interfaces/IScheduler.h"
#include "../interfaces/IClock.h"
#include "../domain/AlarmManager.h"
#include <functional>


class AlarmScheduler {
public:
	AlarmScheduler(AlarmManager& alarmManager, IClock& clock, IScheduler& scheduler)
		: alarmManager_(alarmManager), clock_(clock), scheduler_(scheduler) {

		}

	void setOnAlarmDue(std::function<void(const Alarm&)> cb) { onAlarmDue_ = std::move(cb); }

	void start() { rescheduleNext(); }        
  

private:
	void rescheduleNext() {
		// make sure the current timer is off
		scheduler_.cancel(currentHandle_);
		currentHandle_ = IScheduler::kInvalidHandle;

		const Alarm* next = alarmManager_.getNextActiveAlarm();
		if (!next) return; // nothing coming

		// convert to milliseconds
		auto delay = next->getDurationUntilRing(clock_.now(), clock_.getCurrentDay());
		currentHandle_ = scheduler_.scheduleOnce(delay, [this]() { onTimerFired(); });
	}

	void onTimerFired() {
		const Alarm* alarm = alarmManager_.getNextActiveAlarm();
		if (alarm && alarm->shouldTrigger(clock_.now())) {
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
