#include "../doctest.h"

#include <chrono>
#include <memory>

#include "../mock-interfaces/MockClock.h"
#include "../mock-interfaces/MockStorage.h"
#include "../mock-interfaces/MockScheduler.h"
#include "../mock-interfaces/MockRouter.h"
#include "../mock-interfaces/MockPuzzleView.h"
#include "../domain/MockAlarm.h"
#include "../domain/FixedPuzzle.h"
#include "../domain/MockPuzzleFactory.h"

#include "src/domain/AlarmManager.h"
#include "src/puzzles/RandomNumberGenerator.h"
#include "src/UI/presenters/PuzzlePresenter.h"

namespace {
Alarm ringingAlarm(MockClock& clock, AlarmManager& manager) {
	clock.setTime(9, 0);
	clock.setCurrentDay(Days::Monday);
	clock.setDaysSince1970(100);

	Alarm alarm = createMockAlarm(9, 0);
	alarm.turnOn();
	manager.addAlarm(alarm);
	manager.startRinging(alarm.getId());
	return alarm;
}
}  // namespace

TEST_CASE("PuzzlePresenter dismisses the alarm on a correct answer") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;
	MockRouter router;
	MockPuzzleView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager);
	RandomNumberGenerator rng;
	MockPuzzleFactory factory(std::make_unique<FixedPuzzle>(42), rng);

	PuzzlePresenter presenter(view, router, manager, scheduler, factory, alarm.getId(), 10);

	CHECK(view.loadCount == 1);
	CHECK(view.question == "2 + 3");
	CHECK(view.lastPercent == 100);
	REQUIRE(scheduler.pendingCount() == 1);
	CHECK(scheduler.nextDelay() == std::chrono::seconds(1));

	view.submit(PuzzleResponse(41));
	CHECK(router.navigations.empty());
	CHECK(manager.isRinging());

	view.submit(PuzzleResponse(42));
	REQUIRE(router.lastScreen() == ScreenType::Home);
	CHECK(manager.wasAlarmDismissed(alarm.getId()));
	CHECK_FALSE(manager.isRinging());
	CHECK(scheduler.pendingCount() == 0);
}

TEST_CASE("PuzzlePresenter returns to the ringing screen on timeout") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;
	MockRouter router;
	MockPuzzleView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager);
	RandomNumberGenerator rng;
	MockPuzzleFactory factory(std::make_unique<FixedPuzzle>(42), rng);

	PuzzlePresenter presenter(view, router, manager, scheduler, factory, alarm.getId(), 2);

	CHECK(view.lastPercent == 100);

	scheduler.fireNext();
	CHECK(view.lastPercent == 50);
	REQUIRE(scheduler.pendingCount() == 1);

	scheduler.fireNext();
	REQUIRE(router.lastScreen() == ScreenType::Ringing);
	CHECK(router.navigations.back().alarmId == alarm.getId());
	CHECK(scheduler.pendingCount() == 0);
}
