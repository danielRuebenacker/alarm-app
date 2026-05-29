#include <ctime>

enum PuzzleType {
	MATHS,
	MEMORY,
	CAPTCHA
};

struct AlarmData {
	int hour;
	int min;
	bool isActive;
	int snoozeTimeMinutes;
	int maxSnoozes;
	PuzzleType puzzleType;

	bool shouldFire(const std::tm& localTime) const {
		if (!isActive) {
			return false;
		}
		return (localTime.tm_hour == hour && localTime.tm_min == min);
	}
};
