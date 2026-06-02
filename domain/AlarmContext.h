#pragma once
#include <memory>
#include <vector>
#include "./Alarm.h"
#include "./UIManager.h"

class ISound;
class IClock;
class IInput;
class IAppState;
class IScreen;

// the app's state machine
class AlarmContext {
	private:
		std::unique_ptr<IAppState> currentState_;
		std::shared_ptr<ISound> sound_;
		std::shared_ptr<IClock> clock_;
		std::shared_ptr<IInput> input_;
		std::vector<Alarm> alarms_;
		std::shared_ptr<UIManager> ui_;
	public:
		AlarmContext(std::shared_ptr<ISound> sound, std::shared_ptr<IClock> clock, std::shared_ptr<IInput> input);

		void changeState(std::unique_ptr<IAppState> newState);
		void setup();
		void update();
		std::shared_ptr<IClock> getClock();
		std::shared_ptr<ISound> getSound();
		std::shared_ptr<IInput> getInput();
		std::shared_ptr<UIManager> getUI();
};
