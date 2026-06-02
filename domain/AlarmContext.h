#pragma once
#include <memory>
#include <vector>

class ISound;
class IClock;
class IAppState;
struct AlarmData;

// the app's state machine
class AlarmContext {
	private:
		std::unique_ptr<IAppState> currentState;
		std::shared_ptr<ISound> soundSys;
		std::shared_ptr<IClock> clockSys;
		std::vector<AlarmData> alarms;
	public:
          AlarmContext(std::shared_ptr<ISound> sound, std::shared_ptr<IClock> clock);

		  void changeState(std::unique_ptr<IAppState> newState);
		  void update();
	std::shared_ptr<IClock> getClock();
};
