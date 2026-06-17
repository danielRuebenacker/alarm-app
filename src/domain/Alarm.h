#pragma once
#include "../types/TimePoint.h"
#include "../types/PuzzleType.h"
#include "../types/Days.h"

#include <atomic>

class Alarm {
	private:
		// id
		inline static std::atomic<int> nextId;
		int id;

		TimePoint time_;
		bool isActive_;
		bool hasTriggered_;
		int snoozeMinutes_;
		int currentNoSnoozes;
		int maxSnoozes_;
		PuzzleType puzzleType_;
		Days days_;

		// static
		static constexpr int DEFAULT_SNOOZES = 3;
		static constexpr int DEFAULT_SNOOZE_TIME = 5;

	public:
		Alarm(TimePoint time, PuzzleType puzzleType, Days days);
		// getters
		int getId() const;
		TimePoint getTime() const;
		bool isActive() const;
		PuzzleType getPuzzleType() const;
		int getCurrentNoSnoozes() const;
		Days getDays() const;

		// setters
		void setDays(const Days& days);

		// logic
		int getMinutesUntilRing(const TimePoint& now, const Days::Day currentDay) const;
		bool snoozePossible() const;
		void turnOff();
		void turnOn();
		bool shouldTrigger(const TimePoint& currentTime) const;
		bool snooze();
};
