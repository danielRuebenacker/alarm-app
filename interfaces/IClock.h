#include <ctime>

// interface to supply the current time
class IClock {
	public:
		virtual ~IClock() = default;
		virtual std::tm now() const = 0;
};
