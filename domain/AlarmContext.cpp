#include "AlarmContext.h"


AlarmContext::AlarmContext(std::shared_ptr<ISound> sound, std::shared_ptr<IClock> clock, std::shared_ptr<IInput> input)
	: sound_(sound), clock_(clock), input_(input) {}

void AlarmContext::changeState(std::unique_ptr<IAppState> newState) {
	if (newState) {
		currentState_ = std::move(newState);
	}
}

