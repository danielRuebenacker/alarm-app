#pragma once
#include <memory>
#include <vector>
#include "./Alarm.h"

class ISound;
class IClock;
class IAppState;

// the app's state machine
class AlarmContext {
	private:
		std::unique_ptr<IAppState> currentState;
		std::shared_ptr<ISound> soundSys;
		std::shared_ptr<IClock> clockSys;
		std::vector<Alarm> alarms;
	public:
		AlarmContext(std::shared_ptr<ISound> sound, std::shared_ptr<IClock> clock);

		void changeState(std::unique_ptr<IAppState> newState);
		void setup();
		void update();
		std::shared_ptr<IClock> getClock();
		std::shared_ptr<ISound> getSound();
};
