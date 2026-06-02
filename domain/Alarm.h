#include <cstdint>
#include "../structs/TimePoint.h"
#include "../interfaces/IClock.h"

enum PuzzleType {
	MATHS,
	MEMORY,
	CAPTCHA
};

enum DayMask : uint8_t
{
    Sunday    = 1 << 0, // 00000001
    Monday    = 1 << 1, // 00000010
    Tuesday   = 1 << 2, // 00000100
    Wednesday = 1 << 3, // 00001000
    Thursday  = 1 << 4, // 00010000
    Friday    = 1 << 5, // 00100000
    Saturday  = 1 << 6  // 01000000
};

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
		int getMinutesUntilRing();
		bool snooze();
		bool turnOff();
		bool turnOn();
		bool shouldTrigger(const TimePoint currentTime);
};
