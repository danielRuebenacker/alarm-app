#pragma once
#include "../types/TimePoint.h"
#include "../types/PuzzleType.h"
#include "../types/Days.h"

#include <atomic>
#include <chrono>

class Alarm {
	private:
		// id
		inline static std::atomic<int> nextId;
		int id;

		TimePoint time_;
		bool isActive_;
		int snoozeMinutes_;
		int maxSnoozes_;
		PuzzleType puzzleType_;
		Days days_;

		int currentNoSnoozes;

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
		int getSnoozeMinutes() const;
		int getMaxSnoozes() const;
		int getCurrentNoSnoozes() const;
		Days getDays() const;

		// setters
		void setDays(const Days& days);
        void setTime(int hour, int min);
        void setSnoozeMinutes(int minutes);
        void setMaxSnoozes(int maxSnoozes);
        void setPuzzleType(PuzzleType type);
		void setDay(Days::Day day);

        // for setting nextId after startup
        static void setNextId(int id);

		// logic
		int getMinutesUntilRing(const TimePoint& now, const Days::Day currentDay) const;
		std::chrono::milliseconds getDurationUntilRing(const TimePoint& now, const Days::Day currentDay) const;
		bool snoozePossible() const;
		void turnOff();
		void turnOn();
        void toggle();
		bool shouldTrigger(const TimePoint& currentTime, Days::Day currentDay = Days::Day::Count) const;
		bool snooze();
        void dismiss();
};
