#pragma once
#include <cstdint>
#include <functional>
#include <chrono>

// abstract class for platform specific scheduler
class IScheduler {
 public:
	using TimerHandle = std::uintptr_t;
	static constexpr TimerHandle kInvalidHandle = 0;

	virtual ~IScheduler() = default;

	virtual TimerHandle scheduleOnce(std::chrono::milliseconds delay,
										std::function<void()> callback) = 0;

	virtual void cancel(TimerHandle handle) = 0;
};
