#pragma once
#include "../types/TimePoint.h"
#include "../types/PuzzleType.h"
#include "../types/Days.h"

#include <atomic>

class Alarm {
	private:
		// id
		static std::atomic<int> nextId;
		int id;

		TimePoint time_;
		bool isActive_;
		bool hasTriggered_;
		int snoozeMinutes_;
		int currentNoSnoozes;
		int maxSnoozes_;
		PuzzleType puzzleType_;
		uint8_t dayMask_;

		// static
		static constexpr int DEFAULT_SNOOZES = 3;
		static constexpr int DEFAULT_SNOOZE_TIME = 5;

	public:
		// by default doesn't repeat on days
		Alarm(TimePoint time, PuzzleType puzzleType, uint8_t dayMask = 0);
		int getId() const;
		TimePoint getTime() const;
		bool isActive() const;

		int getMinutesUntilRing(const TimePoint& now) const;
		PuzzleType getPuzzleType() const;
		bool snoozePossible() const;
		void turnOff();
		void turnOn();
		bool shouldTrigger(const TimePoint& currentTime) const;
		bool snooze();
};
