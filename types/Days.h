#pragma once
#include <bitset>
#include <iostream>

// wrapper struct wraps day mask
struct Days {
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

private:
	std::bitset<Count> mask;

	void set(Day day)						{ mask.set(day); }
	void flip(Day day)						{ mask.flip(day); }
	bool isActive(Day day) const			{ return mask.test(day); }

	bool hasAnyDays() const					{ return mask.any(); }
	bool isEveryDay() const					{ return mask.all(); }
	void clearAll()							{ mask.reset(); }
};
