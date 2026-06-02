#include <ctime>
#include "../types/TimePoint.h"

// interface to supply the current time
class IClock {
	public:
		virtual ~IClock() = default;
		virtual TimePoint now() const = 0;
};
