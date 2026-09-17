#pragma once
#include <lv/lv.hpp>
#include <chrono>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <utility>

#include "../../interfaces/IScheduler.h"

// IScheduler backed by LVGL's own timer system, so callbacks run on the same
// thread as lv_timer_handler() (LVGL is single-threaded).
class LvglScheduler : public IScheduler {
  private:
	struct Entry {
		std::function<void()> callback;
		lv::Timer timer;
	};

	std::map<TimerHandle, std::unique_ptr<Entry>> timers_;
	TimerHandle nextHandle_ = 1;  // 0 is kInvalidHandle

	static void onTimerFired(lv_timer_t* t) {
		auto* entry = static_cast<Entry*>(lv_timer_get_user_data(t));
		if (!entry || !entry->callback) return;
		// move the callback onto the stack: the callback may cancel (and thus
		// destroy) this entry while it is still running
		std::function<void()> callback = std::move(entry->callback);
		callback();
	}

  public:
	TimerHandle scheduleOnce(std::chrono::milliseconds delay,
							 std::function<void()> callback) override {
		auto entry = std::make_unique<Entry>();
		entry->callback = std::move(callback);

		int64_t count = delay.count();
		if (count < 0) count = 0;
		const auto maxMs = static_cast<int64_t>(std::numeric_limits<uint32_t>::max());
		uint32_t delayMs = static_cast<uint32_t>(count > maxMs ? maxMs : count);

		// keep the address stable: the entry lives on the heap and the timer
		// reads it back through LVGL's user_data.
		entry->timer = lv::timer_once(&LvglScheduler::onTimerFired, delayMs, entry.get());

		TimerHandle handle = nextHandle_++;
		timers_.emplace(handle, std::move(entry));
		return handle;
	}

	void cancel(TimerHandle handle) override {
		// erasing destroys the lv::Timer which deletes the underlying lv_timer_t
		timers_.erase(handle);
	}
};
