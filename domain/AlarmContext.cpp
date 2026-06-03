#include "AlarmContext.h"
#include "UIManager.h"

#include "../appStates/IdleState.h"
#include "../interfaces/IClock.h"
#include "../interfaces/ISound.h"
#include "../interfaces/IInput.h"
#include "../interfaces/IAlarmStorage.h"

AlarmContext::AlarmContext(std::shared_ptr<ISound> sound,
                           std::shared_ptr<IClock> clock,
                           std::shared_ptr<IInput> input,
                           std::shared_ptr<UIManager> ui,
                           std::shared_ptr<IAlarmStorage> storage)
    : sound_(sound), clock_(clock), input_(input), ui_(ui), storage_(storage) {}

void AlarmContext::changeState(std::unique_ptr<IAppState> newState) {
	if (newState) {
		currentState_ = std::move(newState);
	}
}

void AlarmContext::setup() {
	// initialise UI (delegate to UIManager), set idle state
	changeState(std::make_unique<IdleState>());
	// load alarms
	std::vector<Alarm> alarms = storage_->loadAlarms();
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
