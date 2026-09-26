#include "../doctest.h"

#include <chrono>
#include <memory>
#include <vector>

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

	// correct: the view is told the verdict, but the countdown has to be given
	// time to show the success flash before the screen goes away
	view.submit(PuzzleResponse(42));
	CHECK(view.verdicts == std::vector<bool>{false, true});
	CHECK(router.navigations.empty());
	CHECK(manager.isRinging());
	REQUIRE(scheduler.pendingCount() == 1);
	CHECK(scheduler.nextDelay() == std::chrono::milliseconds(800));

	scheduler.fireNext();
	REQUIRE(router.lastScreen() == ScreenType::Home);
	CHECK(manager.wasAlarmDismissed(alarm.getId()));
	CHECK_FALSE(manager.isRinging());
	CHECK(scheduler.pendingCount() == 0);
}

TEST_CASE("PuzzlePresenter does not count input after a correct answer") {
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

	view.submit(PuzzleResponse(42));
	REQUIRE(scheduler.pendingCount() == 1);

	// mashing the keypad during the success flash must not revive the countdown
	view.pressAnyKey();
	CHECK(view.lastPercent == 100);
	CHECK(scheduler.pendingCount() == 1);
	CHECK(router.navigations.empty());
}

TEST_CASE("PuzzlePresenter will not dismiss an alarm with no puzzle to solve") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;
	MockRouter router;
	MockPuzzleView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager);
	RandomNumberGenerator rng;
	// a factory that cannot build the requested puzzle hands back nothing,
	// which used to be read as "solved"
	MockPuzzleFactory factory(nullptr, rng);

	PuzzlePresenter presenter(view, router, manager, scheduler, factory, alarm.getId(), 10);

	CHECK(view.loadCount == 0);
	view.submit(PuzzleResponse(0));
	CHECK(view.verdicts == std::vector<bool>{false});
	CHECK(router.navigations.empty());
	CHECK_FALSE(manager.wasAlarmDismissed(alarm.getId()));
	CHECK(manager.isRinging());
}

TEST_CASE("PuzzlePresenter shows the hint of a puzzle that needs one") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;
	MockRouter router;
	MockPuzzleView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager);
	RandomNumberGenerator rng;
	MockPuzzleFactory factory(std::make_unique<FixedPuzzle>(7, "4 8 1\n6 3 9\n2 7 5"), rng);

	PuzzlePresenter presenter(view, router, manager, scheduler, factory, alarm.getId(), 10);

	CHECK(view.hintCount == 1);
	CHECK(view.hint == "4 8 1\n6 3 9\n2 7 5");
}

TEST_CASE("PuzzlePresenter shows no hint for a puzzle that needs none") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;
	MockRouter router;
	MockPuzzleView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager);
	RandomNumberGenerator rng;
	MockPuzzleFactory factory(std::make_unique<FixedPuzzle>(7), rng);

	PuzzlePresenter presenter(view, router, manager, scheduler, factory, alarm.getId(), 10);

	CHECK(view.hintCount == 0);
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

TEST_CASE("PuzzlePresenter ignores the router clock tick") {
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
	REQUIRE(scheduler.pendingCount() == 1);

	// the router ticks the active presenter every second; this must not drive
	// (or duplicate) the puzzle countdown
	presenter.onTick();

	CHECK(view.lastPercent == 100);
	CHECK(scheduler.pendingCount() == 1);
	CHECK(router.navigations.empty());
}

TEST_CASE("PuzzlePresenter resets the timeout on any key press") {
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

	scheduler.fireNext();
	CHECK(view.lastPercent == 50);

	view.pressAnyKey();
	CHECK(view.lastPercent == 100);
	REQUIRE(scheduler.pendingCount() == 1);

	// the clock has been reset, so two fresh ticks are needed to time out
	scheduler.fireNext();
	CHECK(view.lastPercent == 50);
	CHECK(router.navigations.empty());

	scheduler.fireNext();
	REQUIRE(router.lastScreen() == ScreenType::Ringing);
	CHECK(scheduler.pendingCount() == 0);
}
