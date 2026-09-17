#pragma once
#include <chrono>
#include <functional>
#include <map>

#include "src/interfaces/IScheduler.h"

class MockScheduler : public IScheduler {
  public:
	struct Scheduled {
		std::chrono::milliseconds delay;
		std::function<void()> callback;
	};

	TimerHandle scheduleOnce(std::chrono::milliseconds delay,
							 std::function<void()> callback) override {
		TimerHandle handle = nextHandle_++;
		scheduled_[handle] = Scheduled{delay, std::move(callback)};
		return handle;
	}

	void cancel(TimerHandle handle) override {
		scheduled_.erase(handle);
	}

	size_t pendingCount() const {
		return scheduled_.size();
	}

	std::chrono::milliseconds nextDelay() const {
		return scheduled_.begin()->second.delay;
	}

	bool fireNext() {
		if (scheduled_.empty()) return false;

		auto it = scheduled_.begin();
		std::function<void()> callback = it->second.callback;
		scheduled_.erase(it);
		callback();
		return true;
	}

  private:
	TimerHandle nextHandle_ = 1;
	std::map<TimerHandle, Scheduled> scheduled_;
};
