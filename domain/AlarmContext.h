#pragma once
#include <memory>
#include <vector>
#include "../interfaces/IAppState.h"

using std::unique_ptr;
using std::shared_ptr;
using std::vector;

class ISound;
class IClock;
class IHomeScreen;
class IRingingScreen;
struct AlarmData;

// the app's state machine
class AlarmContext {
	private:
		unique_ptr<IAppState> currentState;
		shared_ptr<ISound> soundSys;
		shared_ptr<IClock> clockSys;
		shared_ptr<IHomeScreen> homeScreen;
		shared_ptr<IRingingScreen> ringingScreen;
		vector<AlarmData> alarms;
	public:
          AlarmContext(shared_ptr<ISound> sound, shared_ptr<IClock> clock,
                       shared_ptr<IHomeScreen> home,
                       shared_ptr<IRingingScreen> ringing);

		  void changeState(unique_ptr<IAppState> newState);
		  void update();
};
