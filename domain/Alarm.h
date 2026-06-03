#pragma once
#include "../types/TimePoint.h"
#include "../types/PuzzleType.h"
#include "../types/DayMask.h"

class Alarm {
	private:
		TimePoint time_;
		bool isActive_;
		bool hasTriggered_;
		int snoozeMinutes_;
		int maxSnoozes_;
		PuzzleType puzzleType_;
		uint8_t dayMask_;

		// static
		static constexpr int DEFAULT_SNOOZES = 3;
		static constexpr int DEFAULT_SNOOZE_TIME = 5;

	public:
		// by default doesn't repeat on days
		Alarm(TimePoint time, PuzzleType puzzleType, uint8_t dayMask = 0);
		TimePoint getTime();
		int getMinutesUntilRing(const TimePoint currentTime);
		bool snoozePossible(int currentSnoozes = 0);
		void turnOff();
		void turnOn();
		bool shouldTrigger(const TimePoint currentTime, int currentSnoozes = 0);
};
