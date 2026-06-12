#include "AlarmContext.h"
#include "UIManager.h"
#include "AlarmManager.h"

#include "../appStates/IdleState.h"
#include "../interfaces/ISound.h"
#include "../interfaces/IClock.h"
#include "../interfaces/IInput.h"
#include "../interfaces/IStorage.h"
#include "../interfaces/IAppState.h"

AlarmContext::AlarmContext(std::shared_ptr<ISound> sound,
                           std::shared_ptr<IClock> clock,
                           std::shared_ptr<IInput> input,
                           std::shared_ptr<UIManager> ui,
                           std::shared_ptr<IStorage> storage,
                           std::shared_ptr<PuzzleFactory> puzzleFactory,
                           std::shared_ptr<AlarmManager> alarmManager)
    : sound_(sound), clock_(clock), input_(input), ui_(ui), storage_(storage),
      puzzleFactory_(puzzleFactory), alarmManager_(alarmManager) {}

void AlarmContext::changeState(std::unique_ptr<IAppState> newState) {
	if (newState) {
		currentState_ = std::move(newState);
	}
}

void AlarmContext::setup() {
	// initialise UI (delegate to UIManager), set idle state
	changeState(std::make_unique<IdleState>());
	// have alarm manager load alarms
	alarmManager_->getAlarmsFromStorage(*storage_, clock_->now());
	// go to home screen
	ui_->loadScreen("home");
}

void AlarmContext::update() {
	ui_->update();
	currentState_->update(this);
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

UIManager& AlarmContext::getUI(){
	return *ui_;
}

AlarmManager& AlarmContext::getAlarmManager(){
	return *alarmManager_;
}

void AlarmContext::checkAndOrTrigger() {
	// get time 
	TimePoint now = clock_->now();
	Alarm* nextAlarm = alarmManager_->getNextActiveAlarm(now);
	// if doesn't exist, not alarms are set
	if (!nextAlarm) return;

	if (nextAlarm->shouldTrigger(now)) {
		sound_->ring();
		ui_->loadScreen("ringing");
	}
}

void AlarmContext::onUserSnoozePressed() {
	sound_->stopRinging();

	TimePoint now = clock_->now();
	Alarm* nextAlarm = alarmManager_->getNextActiveAlarm(now);
	if (!nextAlarm) return;

	alarmManager_->snoozeAlarm(*nextAlarm);
	ui_->loadScreen("home");
}

void AlarmContext::onUserDismissedPressed() {
	TimePoint now = clock_->now();
	Alarm* nextAlarm = alarmManager_->getNextActiveAlarm(now);
	if (!nextAlarm) return;

	this->currentPuzzle_ = puzzleFactory_->createPuzzle(nextAlarm->getPuzzleType());

	ui_->loadPuzzleWithWrapper(std::move(this->currentPuzzle_));
}

void AlarmContext::onUserSubmitAnswer() {
	if ()
}
