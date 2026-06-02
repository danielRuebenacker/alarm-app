#include "../types/TimePoint.h"
#include "../types/PuzzleType.h"
#include "../types/DayMask.h"

class Alarm {
	private:
		TimePoint time;
		bool isActive;
		bool hasTriggered;
		int snoozeMinutes;
		int maxSnoozes;
		PuzzleType puzzleType;
		uint8_t dayMask;

	public:
		Alarm(TimePoint t, PuzzleType puzzle, uint8_t days);
		TimePoint getTime();
		int getMinutesUntilRing(const TimePoint currentTime);
		bool snooze();
		bool turnOff();
		bool turnOn();
		bool shouldTrigger(const TimePoint currentTime);
};
