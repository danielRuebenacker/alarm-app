#include "../doctest.h"

#include <chrono>

#include "../mock-interfaces/MockClock.h"
#include "../mock-interfaces/MockStorage.h"
#include "../mock-interfaces/MockRouter.h"
#include "../mock-interfaces/MockRingingView.h"
#include "../domain/MockAlarm.h"

#include "src/domain/AlarmManager.h"
#include "src/UI/presenters/RingingPresenter.h"

namespace {
Alarm ringingAlarm(MockClock& clock, AlarmManager& manager, int maxSnoozes = 3) {
	clock.setTime(9, 30);
	clock.setCurrentDay(Days::Monday);
	clock.setDaysSince1970(100);

	Alarm alarm = createMockAlarm(9, 30);
	alarm.turnOn();
	alarm.setSnoozeMinutes(5);
	alarm.setMaxSnoozes(maxSnoozes);
	manager.addAlarm(alarm);
	manager.startRinging(alarm.getId());
	return alarm;
}
}  // namespace

TEST_CASE("RingingPresenter snoozes an alarm and returns home") {
	MockClock clock;
	MockStorage storage;
	MockRouter router;
	MockRingingView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager);
	RingingPresenter presenter(view, router, manager, alarm.getId());

	CHECK(view.displayCount == 1);
	CHECK(view.snoozeAvailable);

	view.clickSnooze();

	CHECK(manager.isSnoozing());
	CHECK(manager.getDurationUntilNextRing() == std::chrono::minutes(5));
	REQUIRE(router.navigatedTo(ScreenType::Home));
}

TEST_CASE("RingingPresenter dismiss leads to the puzzle screen") {
	MockClock clock;
	MockStorage storage;
	MockRouter router;
	MockRingingView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager);
	RingingPresenter presenter(view, router, manager, alarm.getId());

	view.clickDismiss();

	REQUIRE(router.lastScreen() == ScreenType::Puzzle);
	CHECK(router.navigations.back().alarmId == alarm.getId());
}

TEST_CASE("RingingPresenter disables snooze when no snoozes remain") {
	MockClock clock;
	MockStorage storage;
	MockRouter router;
	MockRingingView view;
	AlarmManager manager(clock, storage);

	Alarm alarm = ringingAlarm(clock, manager, 0);
	RingingPresenter presenter(view, router, manager, alarm.getId());

	CHECK_FALSE(view.snoozeAvailable);

	view.clickSnooze();
	CHECK_FALSE(manager.isSnoozing());
}
