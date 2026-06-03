#include "AlarmContext.h"
#include "../appStates/IdleState.h"


AlarmContext::AlarmContext(std::shared_ptr<ISound> sound, std::shared_ptr<IClock> clock, std::shared_ptr<IInput> input, std::shared_ptr<UIManager> ui)
	: sound_(sound), clock_(clock), input_(input), ui_(ui) {}

void AlarmContext::changeState(std::unique_ptr<IAppState> newState) {
	if (newState) {
		currentState_ = std::move(newState);
	}
}

void AlarmContext::setup() {
	// initialise UI (delegate to UIManager), set idle state
	changeState(std::make_unique<IdleState>());
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
