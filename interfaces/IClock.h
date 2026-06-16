#pragma once
#include "../types/TimePoint.h"
#include "../types/Days.h"

// interface to supply the current time
class IClock {
	public:
		virtual ~IClock() = default;
		virtual TimePoint now() const = 0;
		virtual Days::Day getCurrentDay() const = 0;
};
