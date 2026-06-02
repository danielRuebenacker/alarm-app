#pragma once
#include <memory>
#include <vector>
#include "../interfaces/IAppState.h"

class ISound;
class IClock;
class IHomeScreen;
class IRingingScreen;
struct AlarmData;

// the app's state machine
class AlarmContext {
	private:
		std::unique_ptr<IAppState> currentState;
		std::shared_ptr<ISound> soundSys;
		std::shared_ptr<IClock> clockSys;
		std::shared_ptr<IHomeScreen> homeScreen;
		std::shared_ptr<IRingingScreen> ringingScreen;
		std::vector<AlarmData> alarms;
	public:
          AlarmContext(std::shared_ptr<ISound> sound, std::shared_ptr<IClock> clock,
                       std::shared_ptr<IHomeScreen> home,
                       std::shared_ptr<IRingingScreen> ringing);

		  void changeState(std::unique_ptr<IAppState> newState);
		  void update();
	std::shared_ptr<IClock> getClock();
};
