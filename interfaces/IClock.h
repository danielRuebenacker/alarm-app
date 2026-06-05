#pragma once
#include <ctime>
#include "../types/TimePoint.h"
#include "../types/DayMask.h"

// interface to supply the current time
class IClock {
	public:
		virtual ~IClock() = default;
		virtual TimePoint now() const = 0;
		virtual DayMask getCurrentDay() const = 0;
};
