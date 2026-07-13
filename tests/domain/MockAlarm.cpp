#include "MockAlarm.h"

Alarm createMockAlarm(int hour, int min, const Days& schedule) {
	TimePoint t(hour, min);
	return Alarm(t, PuzzleType::MATHS, schedule);
}
