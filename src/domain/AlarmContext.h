#pragma once
#include <memory>
#include <vector>
#include "./Alarm.h"
#include "PuzzleFactory.h"

class ISound;
class IClock;
class IInput;
class IAppState;
class IScreen;
class IStorage;
class AlarmManager;

// the app's state machine
class AlarmContext {
	private:
		std::unique_ptr<IAppState>	currentState_;
		std::unique_ptr<IPuzzle> currentPuzzle_;
		std::shared_ptr<ISound> 	sound_;
		std::shared_ptr<IClock> 	clock_;
		std::shared_ptr<IInput> 	input_;
		std::shared_ptr<AlarmManager> 	alarmManager_;
		std::shared_ptr<IStorage> 	storage_;
		std::shared_ptr<PuzzleFactory> 	puzzleFactory_;
		std::vector<Alarm> 			alarms_;
	public:
          AlarmContext(std::shared_ptr<ISound> sound,
                       std::shared_ptr<IClock> clock,
                       std::shared_ptr<IInput> input,
                       std::shared_ptr<IStorage> storage,
                       std::shared_ptr<PuzzleFactory> puzzleFactory,
                       std::shared_ptr<AlarmManager> alarmManager);

          ~AlarmContext() = default;
          // don't allow copying
          AlarmContext(const AlarmContext &) = delete;
          AlarmContext &operator=(const AlarmContext &) = delete;

          // disallow moving
          AlarmContext(AlarmContext &&) = default;
          AlarmContext &operator=(AlarmContext &&) = default;

          void changeState(std::unique_ptr<IAppState> newState);
          void setup();
          void update();
          IClock& getClock();
          ISound& getSound();
          IInput& getInput();
          AlarmManager& getAlarmManager();
		  void checkAndOrTrigger();
};
