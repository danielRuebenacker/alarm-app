#include "AlarmContext.h"
#include "AlarmManager.h"

#include "../interfaces/ISound.h"
#include "../interfaces/IClock.h"
#include "../interfaces/IInput.h"
#include "../interfaces/IStorage.h"

AlarmContext::AlarmContext(std::shared_ptr<ISound> sound,
                           std::shared_ptr<IClock> clock,
                           std::shared_ptr<IInput> input,
                           std::shared_ptr<IStorage> storage,
                           std::shared_ptr<PuzzleFactory> puzzleFactory,
                           std::shared_ptr<AlarmManager> alarmManager)
    : sound_(sound), clock_(clock), input_(input), storage_(storage),
      puzzleFactory_(puzzleFactory), alarmManager_(alarmManager) {}

void AlarmContext::setup() {
	// have alarm manager load alarms
	alarmManager_->getAlarmsFromStorage();
}

IClock& AlarmContext::getClock(){
	return *clock_;
}

ISound& AlarmContext::getSound(){
	return *sound_;
}

IInput& AlarmContext::getInput(){
	return *input_;
}

AlarmManager& AlarmContext::getAlarmManager(){
	return *alarmManager_;
}

void AlarmContext::checkAndOrTrigger() {
	const Alarm* nextAlarm = alarmManager_->getNextActiveAlarm();
	// if doesn't exist, not alarms are set
	if (!nextAlarm) return;

	// get time
	TimePoint now = clock_->now();
	if (nextAlarm->shouldTrigger(now)) {
		sound_->ring();
	}
}
