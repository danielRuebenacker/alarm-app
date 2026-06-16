#pragma once
#include <bitset>

// wrapper struct wraps day mask
class Days {
public:
	enum Day : size_t {
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
		Count
	};

	Days() = default;

	void set(Day day)						{ mask_.set(day); }
	void flip(Day day)						{ mask_.flip(day); }
	bool isActive(Day day) const			{ return mask_.test(day); }

	bool hasAnyDays() const					{ return mask_.any(); }
	bool isEveryDay() const					{ return mask_.all(); }
	void clearAll()							{ mask_.reset(); }

	int daysUntilNextActive(int currentDay) const;

private:
	std::bitset<Count> mask_;
};
